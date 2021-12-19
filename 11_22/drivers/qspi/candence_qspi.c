/***************************************************************************
  Copyright (c) 2021 Huajie IMI Technology Co., Ltd.
  All rights reserved.

  @brief  candence   qspi chip support

**************************************************************************/
#include <string.h>
#include "imi_type.h"
#include "elog.h"
#include "hellboy_map.h"
#include "candence_qspi.h"
#include "initcalls.h"
#include "delay.h"
#include "imi_io.h"
#include "linux_compat.h"
#include "errno.h"
#include "board.h"
#include "dmaengine.h"

#ifdef QSPI_USE_DMA
static int use_dma = 1;
#else
static int use_dma = 0;
#endif

struct dma_chan *rx_chan;
struct dma_chan *tx_chan;
dma_cookie_t cookie_qspi;
FRTOS_completion_t	qspi_dma_complete;

#define PL330_MAX_BURST			16
#define CQSPI_STIG_DATA_LEN_MAX		(8)

#define CADENCE_QSPI_MAX_DELAY		(0xffffff)

static inline void candence_qspi_controller_enable(void)
{
	writel(readl(QSPI_CFG) | 0x01, QSPI_CFG); //enable controller
}

static inline void candence_qspi_controller_disable(void)
{
	writel(readl(QSPI_CFG) & (~0x00000001), QSPI_CFG); //disable controller
}

void candence_qspi_cs_open(void)
{
	volatile unsigned int val;

	val = readl(QSPI_CFG) & (~(0x01 << 10));

	writel(val, QSPI_CFG);
}

void candence_qspi_cs_close(void)
{
	volatile unsigned int val;

	val = readl(QSPI_CFG) | (0x01 << 10);

	writel(val, QSPI_CFG);
}

static  int candence_qspi_wait_cmd_idle(void)
{
	unsigned int time_out = CADENCE_QSPI_MAX_DELAY;
	volatile unsigned int val;

	do {
			val = readl(QSPI_FCMD) & 0x02;
			time_out--;
	} while (val && time_out);  // until Flash Command Execution NOT in Progress

	if (!time_out) {
		log_e("wait cmd timeout\n");
		return -EIO;
	}

	return 0;
}

static  int candence_qspi_wait_spi_idle(void)
{
	unsigned int time_out = CADENCE_QSPI_MAX_DELAY * 2;
	volatile unsigned int val;

	do {
			val = !(readl(QSPI_CFG) & QSPI_CFG_IDLE);
			time_out--;
	} while (val && time_out) ;  // until SPI Interface Idle

	if (!time_out) {
		log_e("wait spi timeout\n");
		return -EIO;
	}

	return 0;
}

static  int candence_qspi_wait_cmd(void)
{
	int ret;

	ret = candence_qspi_wait_cmd_idle();
	if (ret)
		return ret;

	ret = candence_qspi_wait_spi_idle();
	if (ret)
		return ret;

	return 0;
}

static void cadence_qspi_config_baudrate(unsigned int sclk_mhz)
{
	volatile unsigned int val;
	unsigned int div = DIV_ROUND_UP(QSPI_REF_CLK_FREQ, 2 * sclk_mhz) - 1;

	candence_qspi_controller_disable();

	if (div > 0xf)
		div = 0xf;

	val = readl(QSPI_CFG) & (~(0xf << 19));
	val = val | (div << 19);

	writel(val, QSPI_CFG);

	candence_qspi_controller_enable();
}

static void cadence_qspi_config_delay(void)
{
	unsigned int val;

	candence_qspi_controller_disable();

	val = 0x01 << 24 | 0x01 << 16 | 0x01 << 8 | 0x01;
	writel(val, QSPI_DELAY);

	candence_qspi_controller_enable();
}

//brom, we set bypass = 0
static void cadence_qspi_config_read_capture(unsigned int bypass, unsigned int delay)
{
	volatile unsigned int val;

	if (delay > 0xf)
		delay = 0xf;

	candence_qspi_controller_disable();

	val = readl(QSPI_RDDATCAP);

	val = val & (~0x1f);

	if (bypass)  //disable loopback clock
		val = val | 0x01;

	val = val | delay << 1;

	writel(val, QSPI_RDDATCAP);

	candence_qspi_controller_enable();
}

/*
	page_size: num of bytes per device page
	blk_size: num of bytes per block
*/
void cadence_qspi_controller_init(unsigned int page_size, unsigned int blk_size)
{
	volatile unsigned int val;
	int index;

	for (index = 0; index < 17; index++) {
		if ((1 << index) & blk_size)
			break;
	}

	candence_qspi_wait_spi_idle();

	candence_qspi_controller_disable();

	val = readl(QSPI_DSIZE);
	val = val & 0x0f;
	val = val | (index << 16) | (page_size << 4);
	writel(val, QSPI_DSIZE);

	writel(0x0, QSPI_REMAP); //remap addr is 0
	writel(readl(QSPI_CFG) | 0x10000, QSPI_CFG);//enable remap

	/* Indirect mode configurations */
	writel(0x40, QSPI_SRAMPA);
	writel(QSPI0_AHB_BASE_ADDR, QSPI_INDTRIG);  // 1c set INDAC Trigger Address
	writel(0xf, QSPI_INDTRIGAD); // 80 set INDAC Trigger Range to 2 ** 6 = 64 bytes = AHB Burst 16 words for DMA

	writel(0x0, QSPI_IM);//disable interrupt

	candence_qspi_controller_enable();
}

static int candence_qspi_calibration(unsigned int sclk_mhz)
{
#define CQSPI_READ_CAPTURE_MAX_DELAY 12
	unsigned char opcode_rdid = 0x9F;
	unsigned int idcode = 0, temp = 0;
	int err = 0, i, range_lo = -1, range_hi = -1;


	temp = 1;
	cadence_qspi_config_read_capture(1, temp);
	return 0;


	cadence_qspi_config_baudrate(1);
	cadence_qspi_config_read_capture(1, 1);

	err = cadence_qspi_command_read(1, &opcode_rdid, 3, (u8 *)&idcode);
	if (err) {
		log_e("Calibration failed\n");
		return err;
	}
//	printf("id: %x %x %x\n", idcode & 0xff, 0xff & (idcode >> 8 ), 0xff & (idcode >> 16));

	cadence_qspi_config_baudrate(sclk_mhz);
	for (i = 0; i < CQSPI_READ_CAPTURE_MAX_DELAY; i++) {
		cadence_qspi_config_read_capture(1, i);
		err = cadence_qspi_command_read(1, &opcode_rdid, 3, (u8 *)&temp);
		if (err) {
			log_e("Cal... failed\n");
			return err;
		}

		if (range_lo == -1 && temp == idcode) {
			range_lo = i;
			continue;
		}

		if (range_lo != -1 && temp != idcode) {
			range_hi = i - 1;
			break;
		}
		range_hi = i;
	}

	if (range_lo == -1) {
		log_e("Calibration failed (low range)\n");
		return err;
	}

	temp = (range_hi + range_lo) / 2;
//	printf("cap delay = %x\n", temp);
	cadence_qspi_config_read_capture(1, temp);

	return 0;
}


void  candence_qspi_controller_setup(U32 sclk_mhz, unsigned char loopback_clk_bypass, unsigned int page_size, unsigned int blk_size)
{
	int ret;
	candence_qspi_controller_disable();

	cadence_qspi_config_baudrate(sclk_mhz);

	cadence_qspi_config_delay();

	cadence_qspi_controller_init(page_size, blk_size);

	// disable polling 05 command.
	writel(readl(QSPI_WRCCTRL) | (1 << 14), QSPI_WRCCTRL);
	candence_qspi_controller_enable();

	if (loopback_clk_bypass) { //spi read data use ref_clk
		candence_qspi_cs_open();
		ret = candence_qspi_calibration(sclk_mhz);
		if (ret) {
			cadence_qspi_config_read_capture(1, 1);
		}
		candence_qspi_cs_close();
	} else { //spi read data use SCLK
		cadence_qspi_config_read_capture(0, QSPI_REF_CLK_FREQ / QSPI_FLASH_CLK_FREQ);
	}
}

// ----------------------------------------------------------------------------
static unsigned int cadence_qspi_cmd2addr(const unsigned char *addr_buf,
	unsigned int addr_width)
{
	unsigned int addr = 0;

	if (addr_width == 1)
		addr = addr_buf[0];

	if (addr_width == 2)
		addr = (addr_buf[0] << 8) | addr_buf[1];

	if (addr_width >= 3)
		addr = (addr_buf[0] << 16) | (addr_buf[1] << 8) | addr_buf[2];

	if (addr_width == 4)
		addr = (addr << 8) | addr_buf[3];

	return addr;
}

/* Opcode + Address (3/4 bytes) + dummy bytes (0-4 bytes) */
int  cadence_qspi_indirect_read_setup(const u8 *cmdbuf, unsigned int cmdlen)
{
	unsigned int addr_bytes;
	volatile unsigned int val;
	int read_data_dummy_num;
	int bootmode = get_bootmode();

	addr_bytes = cmdlen - 1;

	if (bootmode == NAND_BOOT || bootmode == NAND_BOOT_DISABLE_LOOPBACK) { //nand
			read_data_dummy_num = 1;

	} else { //nor
		if (cmdbuf[0] == 0x6B)
			read_data_dummy_num = 1;
		else
			read_data_dummy_num = 0;
	}

	candence_qspi_wait_cmd();
	candence_qspi_controller_disable();

	writel(32, QSPI_INDRDWM);  // set watermark to half size of the reading partition
	writel(32, QSPI_INDWRWM);  // set watermark to half size of the reading partition

	//change addr size for dummy
	val = readl(QSPI_DSIZE);
	val = val & (~0x0f);
	val = val | (addr_bytes - 1);
	writel(val, QSPI_DSIZE);

	candence_qspi_controller_enable();

	val = cmdbuf[0]; // cmd

	if (cmdbuf[0] == 0x6b) { //read from cachex4
		 val = val | (2 << 16); //if you want data tr use D0~D3, open this line
	}

	if (read_data_dummy_num) {
		val = val | ((read_data_dummy_num * 8) << 24);
	}

	writel(val, QSPI_RDINS); //cmd use D0 addr use D0, data default use D0

	val = cadence_qspi_cmd2addr(&cmdbuf[1], addr_bytes);
	writel(val, QSPI_INDRDSA);

	return 0;
}


// --------DMA--------------------------------------------------------------------
void cqspi_dma_tx_callback(void *param)
{
	FRTOS_complete(qspi_dma_complete);
	dma_release_channel((struct dma_chan *)param);
}

static int cqspi_indirect_setup_dma(u8 *buf, u32 bytes, u8 dir)
{
	struct dma_async_tx_descriptor *tx = NULL;
	dma_cookie_t cookie;
	unsigned int reg = 0;
	struct dma_chan *dmachan;

	if (dir == DMA_FROM_DEVICE) {
		dmachan = rx_chan;
	} else {
		dmachan = tx_chan;
	}

	dmachan = dma_get_slave_channel(dmachan);
	if (dmachan == NULL) {
		return -1;
	}

	tx = dmaengine_prep_slave_single(dmachan, (dma_addr_t)buf, bytes, dir,
							DMA_CTRL_ACK | DMA_PREP_INTERRUPT);
	if (!tx) {
		log_e("%s %d err\n", __func__, __LINE__);
		return -ENOMEM;
	}

	FRTOS_completion_reinit(qspi_dma_complete);
	tx->callback = cqspi_dma_tx_callback;
	tx->callback_param = dmachan;

	cookie = dmaengine_submit(tx);
	if (dma_submit_error(cookie)) {
		log_e("dma_submit_error\n");
		return -EIO;
	}
	cookie_qspi = cookie;

	reg = readl(QSPI_CFG);
	reg |= QSPI_REG_CONFIG_DMA_MASK;
	writel(reg, QSPI_CFG);

	return 0;
}

static int cqspi_indirect_exe_dma(u8 *buf, u32 bytes, u8 dir)
{
	enum dma_status status;
	int ret = 0;
	int time_out = 1000;
	struct dma_chan *dmachan;

	if (dir == DMA_FROM_DEVICE) {
		dmachan = rx_chan;
	} else {
		dmachan = tx_chan;
	}

	dma_async_issue_pending(dmachan);
	if (FRTOS_completion_wait(qspi_dma_complete, time_out)) {
		log_e("dma wait time out\n");
		ret = -1;
	}

	status = dma_async_is_tx_complete(dmachan, cookie_qspi, NULL, NULL);
	if (status != DMA_COMPLETE) {
		log_e( "%s %d (status != DMA_COMPLETE) %d\n",
			__func__, __LINE__, status);
		ret = -EIO;
		goto err_unmap;
	}

err_unmap:
	return ret;
}


// ----------------------------------------------------------------------------
// rdata array should be pre-allocated with enough memory space.
// ----------------------------------------------------------------------------
int  cadence_qspi_indirect_read_execute(unsigned int n_rx, unsigned char *rxbuf)
{
	int remaining = n_rx;
	int bytes_to_read = 0;
	volatile unsigned int val;
	int time_out;
	int ret;

	writel(n_rx, QSPI_INDRDNB);   // 6c

	if (use_dma) {
		/*try to setuup dma*/
		ret = cqspi_indirect_setup_dma(rxbuf, n_rx, DMA_FROM_DEVICE);

		if (!ret) {
			/* start indirect read. */
			writel(QSPI_REG_INDIRECTRD_START_MASK, QSPI_INDRD);
			ret = cqspi_indirect_exe_dma(rxbuf, n_rx, DMA_FROM_DEVICE);
			if (ret)
				log_e("cqspi_indirect_exe_dma error\n");
		} else {
			log_e("setup dma error\n");
		}

	} else {

		writel(0x01, QSPI_INDRD);   // 60 start indac reading op

		while (remaining > 0) {
			time_out = CADENCE_QSPI_MAX_DELAY;
			do {
				val = readl(QSPI_SRAMFLVL) & 0xFFFF;
				time_out--;
			} while(!val && time_out);

			if (!time_out) {
				log_e("Indirect read timed out\n");
				goto fail;
			}

			bytes_to_read = val;
			while (bytes_to_read != 0 && remaining > 0) {
				bytes_to_read *= 4;			//sram in units of 4 bytes
				bytes_to_read = bytes_to_read > remaining ?
						remaining : bytes_to_read;

				/*
				 * Handle non-4-byte aligned access to avoid
				 * data abort.
				 */
				if (((unsigned int)rxbuf % 4) || (bytes_to_read % 4))
					readsb((const void*)QSPI0_AHB_BASE_ADDR, rxbuf, bytes_to_read);
				else
					readsl((const void*)QSPI0_AHB_BASE_ADDR, rxbuf,
						   bytes_to_read >> 2);

				rxbuf += bytes_to_read;
				remaining -= bytes_to_read;
				bytes_to_read = readl(QSPI_SRAMFLVL) & 0xFFFF;
			}
		}
	}

	time_out = CADENCE_QSPI_MAX_DELAY;
	while ((readl(QSPI_INDRD) & 0x20) != 0x20) {
		if (--time_out <= 0) {
			log_e("wait read timeout\n");
			goto fail;
		}
	}

	/* Clear indirect completion status */
	writel(BIT(5), QSPI_INDRD);

	return 0;

fail:
	writel(BIT(1), QSPI_INDRD); //Cancel indirect read
	return -EIO;
}


/* Opcode + Address (3/4 bytes) */
int cadence_qspi_indirect_write_setup(const u8 *cmdbuf, unsigned int cmdlen)
{
	unsigned int addr_bytes = cmdlen - 1;
	volatile unsigned int val;

	if (cmdlen < 3 || cmdbuf == NULL) {
		log_e("invalid input\n");
		return -EINVAL;
	}

	candence_qspi_wait_cmd();
	candence_qspi_controller_disable();

	writel(32, QSPI_INDRDWM);  // set watermark to half size of the reading partition
	writel(32, QSPI_INDWRWM);  // set watermark to half size of the reading partition

	//change addr size for dummy
	val = readl(QSPI_DSIZE);
	val = val & (~0x0f);
	val = val | (addr_bytes - 1);
	writel(val, QSPI_DSIZE);

	candence_qspi_controller_enable();

	writel(readl(QSPI_RDINS) & ~0x0300, QSPI_RDINS);   // cmd use D0 to transfer

	val = cmdbuf[0]; // cmd
	//val = val | (1 << 8); //turn off 0x06 cmd before DAC or INDAC

	if (cmdbuf[0] == 0x32) { //program loadx4
		 val = val | (2 << 16); //if you want data tr use D0~D3, open this line
	}

	writel(val, QSPI_WRINS); //addr use D0, data default use D0

	val = cadence_qspi_cmd2addr(&cmdbuf[1], addr_bytes);
	writel(val, QSPI_INDWRSA);

	return 0;
}


// ----------------------------------------------------------------------------
// txbuf should aligned to 4 bytes.
// ----------------------------------------------------------------------------
int cadence_qspi_indirect_write_execute(const unsigned char *txbuf, int n_tx, unsigned int page_size)
{
	unsigned int remaining = n_tx;
	const u8 *bb_txbuf = txbuf;
	unsigned int write_bytes, tmp, time_out;
	volatile unsigned int value;
	u8 *buf = (u8 *)txbuf;
	int ret;

	if (((unsigned int)txbuf) % 4) {
		log_e("tx start addr not aligned\n");
	}

	/* Configure the indirect read transfer bytes */
	writel(n_tx, QSPI_INDWRCT);

	if (use_dma) {
		ret = cqspi_indirect_setup_dma(buf, n_tx, DMA_TO_DEVICE);
		if (!ret) {
			/* Start the indirect write transfer */
			writel(0x01, QSPI_INDWR);

			ret = cqspi_indirect_exe_dma(buf, n_tx, DMA_TO_DEVICE);
			if (ret) {
				log_e("exe dma error\n");
				goto fail;
			}
		} else {
			log_e("cqspi_indirect_setup_dma fail\n");
		}

	} else {

		/* Start the indirect write transfer */
		writel(0x01, QSPI_INDWR);

		while (remaining > 0) {
			write_bytes = remaining > page_size ? page_size : remaining;

			writesl((const void*)QSPI0_AHB_BASE_ADDR, bb_txbuf, write_bytes >> 2);
			if (write_bytes % 4) {
				tmp = write_bytes - (write_bytes % 4);
				writesb((const void*)QSPI0_AHB_BASE_ADDR,  bb_txbuf + tmp, write_bytes % 4);
			}

			time_out = CADENCE_QSPI_MAX_DELAY;
			do {
				value = (readl(QSPI_SRAMFLVL) >> 16) & 0xFFFF;
				time_out--;
			} while(value && time_out);

			if (!time_out) {
				log_e("Indirect wr timed out\n");
				goto fail;
			}

			bb_txbuf += write_bytes;
			remaining -= write_bytes;
		}
	}

	/* Check indirect done status */
	time_out = CADENCE_QSPI_MAX_DELAY;
	while ((readl(QSPI_INDWR) & 0x20) != 0x20) {
		if (--time_out <= 0) {
			log_e("wait read timeout\n");
			goto fail;
		}
	}

	/* Clear indirect completion status */
	writel(BIT(5), QSPI_INDWR);

	return 0;

fail:
	/* Cancel the indirect write */
	writel(BIT(1),    QSPI_INDWR);
	return -EIO;
}

/* For command RDID, RDSR. */
int cadence_qspi_command_read(unsigned int cmdlen, const u8 *cmdbuf,
	 unsigned int rxlen, u8 *rxbuf)
{
	unsigned int reg;
	unsigned int read_len;
	unsigned int addr_value;

	if (!cmdlen || rxlen > CQSPI_STIG_DATA_LEN_MAX) {
		log_e("Inval read param\n");
		return -EINVAL;
	}

	candence_qspi_wait_spi_idle();
	candence_qspi_wait_cmd_idle();

	reg = cmdbuf[0] << 24; //opcode

	if (rxlen > 0) {
		reg |= (0x1 << 23); //read data enable

		/* 0 means 1 byte. */
		reg |= ((rxlen - 1) & 0x07) << 20; //read data len
	}

	if (cmdlen == 2 || cmdlen == 4 || cmdlen == 5) {
		reg |= (0x1 << 19); //address enable
		/* Number of bytes to write. */
		reg |= ((cmdlen - 2) & 0x03) << 16;

		addr_value = cadence_qspi_cmd2addr(&cmdbuf[1], cmdlen - 1);

		writel(addr_value, QSPI_FCMDADDR);
	}

	writel(reg, QSPI_FCMD);
	reg = reg | BIT(0); //start execute
	writel(reg, QSPI_FCMD);

	candence_qspi_wait_cmd();

	if (!rxbuf || !rxlen)
		return 0;

	reg = readl(QSPI_FCMDRDL);

	/* Put the read value into rx_buf */
	read_len = (rxlen > 4) ? 4 : rxlen;
	memcpy(rxbuf, (unsigned char *)&reg, read_len);
	rxbuf += read_len;

	if (read_len == 1)
		*rxbuf = reg & 0xff;

	if (rxlen > 4) {
		reg = readl(QSPI_FCMDRDU);
		read_len = rxlen - read_len;
		memcpy(rxbuf, (unsigned char *)&reg, read_len);
	}
	return 0;
}

/* For commands: WRSR, WREN, WRDI, CHIP_ERASE, BE, etc. */
int cadence_qspi_command_write(unsigned int cmdlen,
	const u8 *cmdbuf, unsigned int txlen,  const u8 *txbuf)
{
	unsigned int reg = 0;
	unsigned int addr_value;
	unsigned int wr_data;
	unsigned int wr_len;

	if (!cmdlen || cmdlen > 5 || txlen > 8 || cmdbuf == NULL) {
		log_e("Inval wr param\n");
		return -EINVAL;
	}

	candence_qspi_wait_spi_idle();
	candence_qspi_wait_cmd_idle();


	reg = cmdbuf[0] << 24; //opcode

	if (cmdlen == 2 || cmdlen == 4 || cmdlen == 5) {
		reg |= (0x1 << 19);
		/* Number of bytes to write. */
		reg |= ((cmdlen - 2) & 0x03) << 16;
		/* Get address */
		addr_value = cadence_qspi_cmd2addr(&cmdbuf[1], cmdlen - 1);

		writel(addr_value, QSPI_FCMDADDR);
	}

	if (txlen) {
		/* writing data = yes */
		reg |= (0x1 << 15);
		reg |= ((txlen - 1) & 0x07) << 12; //data len

		wr_len = txlen > 4 ? 4 : txlen;
		memcpy((unsigned char *)&wr_data, txbuf, wr_len);
		writel(wr_data, QSPI_FCMDWDL);

		if (txlen > 4) {
			txbuf += wr_len;
			wr_len = txlen - wr_len;
			memcpy((unsigned char *)&wr_data, txbuf, wr_len);
			writel(wr_data, QSPI_FCMDWDU);
		}
	}

	/* Execute the command */
	writel(reg, QSPI_FCMD);
	reg = reg | BIT(0); //start execute
	writel(reg, QSPI_FCMD);

	candence_qspi_wait_cmd();

	return 0;
}

void candence_clk_delay_config(int div, int delay)
{
	unsigned int val;

	candence_qspi_controller_disable();

	//div
	candence_qspi_controller_disable();
	if (div > 0xf)
		div = 0xf;
	val = readl(QSPI_CFG) & (~(0xf << 19));
	val = val | (div << 19);
	writel(val, QSPI_CFG);
	candence_qspi_controller_enable();

	//delay
	cadence_qspi_config_read_capture(1, delay);

	return;
}

int cadence_qspi_setup_dma(void)
{
	int ret;
	struct dma_slave_config cfg;

#ifdef CHIP_IMI_2280
	/*--- tx----- */
	tx_chan = dma_request_chan(NULL, "chan_7");
	memset(&cfg, 0, sizeof(cfg));
	cfg.direction = DMA_MEM_TO_DEV;
	cfg.dst_addr = QSPI0_AHB_BASE_ADDR;
	cfg.dst_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
	cfg.dst_maxburst = PL330_MAX_BURST;
	ret = dmaengine_slave_config(tx_chan, &cfg);
	if (ret < 0)
		return -ENOMEM;

	/*--- rx ---------*/
	rx_chan = dma_request_chan(NULL, "chan_6");
	memset(&cfg, 0, sizeof(cfg));
	cfg.direction = DMA_DEV_TO_MEM;
	cfg.src_addr = QSPI0_AHB_BASE_ADDR;
	cfg.src_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
	cfg.src_maxburst = PL330_MAX_BURST;
	ret = dmaengine_slave_config(rx_chan, &cfg);
	if (ret < 0)
		return -ENOMEM;

#else

	/*--- tx----- */
	tx_chan = dma_request_chan(NULL, "chan_6");
	memset(&cfg, 0, sizeof(cfg));
	cfg.direction = DMA_MEM_TO_DEV;
	cfg.dst_addr = QSPI0_AHB_BASE_ADDR;
	cfg.dst_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
	cfg.dst_maxburst = PL330_MAX_BURST;
	ret = dmaengine_slave_config(tx_chan, &cfg);
	if (ret < 0)
		return -ENOMEM;

	/*--- rx ---------*/
	rx_chan = dma_request_chan(NULL, "chan_7");
	memset(&cfg, 0, sizeof(cfg));
	cfg.direction = DMA_DEV_TO_MEM;
	cfg.src_addr = QSPI0_AHB_BASE_ADDR;
	cfg.src_addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
	cfg.src_maxburst = PL330_MAX_BURST;
	ret = dmaengine_slave_config(rx_chan, &cfg);
	if (ret < 0)
		return -ENOMEM;
#endif

	qspi_dma_complete = FRTOS_completion_init();

	return 0;
}

