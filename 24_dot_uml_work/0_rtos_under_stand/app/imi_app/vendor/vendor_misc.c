#include <soc/imi_video.h>
#include <usb/linux/gadget.h>
#include <errno.h>
#include <linux_compat.h>
#include <imi_dev.h>

#include "../include/vendor_device.h"
#include "../include/vendor_cmd.h"
#include "../include/vendor_misc.h"
#include "../include/vendor_sub_system.h"

#define LOG_TAG "vendor_misc"
#include "elog.h"

struct fw_ver {
	unsigned char major;
	unsigned char minor;
	unsigned short ver;
}__attribute__((packed));

struct hw_ver {
	unsigned char hw;
	unsigned char chip;
	unsigned char ap_hw;
}__attribute__((packed));

struct protocol_ver {
	struct fw_ver fw;
	struct hw_ver hw;
}__attribute__((packed));

struct sensor_info {
	unsigned char ep_num;
	unsigned char sensor_type;
}__attribute__((packed));

struct device_param {
	unsigned char               serial[32];
	unsigned char               usb_speed;
	struct protocol_ver         versions;
	struct sensor_info          sensor_list[16];
	unsigned char               unused[184];
}__attribute__((packed));

static struct vendor_file_info upload_file_info;
static struct vendor_file_info download_file_info;

static const char* vendor_file_path_upload[] = {  //same as write
	"/amt/imi_firmwwre.bin",    //VENDOR_FILE_FIRMWARE
	"/amt/imi_ref_image.bin",   //VENDOR_FILE_REF_IMAGE
	"/amt/imi_ar_params.bin",   //VENDOR_FILE_AR_PARAMS
	"/amt/imi_camera_params.bin",   //VENDOR_FILE_CAMERA_PARAMS
	"/amt/imi_dev_params.bin",      //VENDOR_FILE_DEVEICE_PARAMS
	"/amt/imi_color.bin",   //VENDOR_FILE_COLOR_FRAME_MODE_PARAMS
	"/amt/imi_depth.bin",   //VENDOR_FILE_DEPTH_FRAME_MODE_PARAMS
	"/amt/imi_ir.bin",  //VENDOR_FILE_IR_FRAME_MODE_PARAMS
	"/data/update_package.zip",  //VENDOR_RECOVERY
	NULL, //VENDOR_PAY_AES_KEY
	NULL, //VENDOR_PAY_RSA_KEY
	NULL, //VENDOR_PAY_FW
	"/amt/imi_init_param.bin",
};

static const char* vendor_file_path_download[] = {  //same as read
	"/amt/backup/imi_firmwwre.bin",    //VENDOR_FILE_FIRMWARE
	"/amt/backup/imi_ref_image.bin",   //VENDOR_FILE_REF_IMAGE
	"/amt/backup/imi_ar_params.bin",   //VENDOR_FILE_AR_PARAMS
	"/amt/backup/imi_camera_params.bin",   //VENDOR_FILE_CAMERA_PARAMS
	"/amt/backup/imi_dev_params.bin",     //VENDOR_FILE_DEVEICE_PARAMS
	"/amt/backup/imi_color.bin",   //VENDOR_FILE_COLOR_FRAME_MODE_PARAMS
	"/amt/backup/imi_depth.bin",   //VENDOR_FILE_DEPTH_FRAME_MODE_PARAMS
	"/amt/backup/imi_ir.bin",  //VENDOR_FILE_IR_FRAME_MODE_PARAMS
	"/amt/backup/update_package.zip",  //VENDOR_RECOVERY
	NULL, //VENDOR_PAY_AES_KEY
	NULL, //VENDOR_PAY_RSA_KEY
	NULL, //VENDOR_PAY_FW
	"/amt/imi_init_param.bin",
};


#define SERIAL_NO_LEN 32
static const char g_serial_no[SERIAL_NO_LEN] = "hjimi";
//static const char *usb_speed_path = "/sys/devices/platform/soc/32200000.dwusb3/5000000.dwc3/gadget/imi_info";

static const char *log_file_path[] = {
	"/data/log/log_file_kernel",
	"/data/log/log_file_kernel.1",
	"/data/log/log_file_kernel.2",
	"/data/log/log_file_system",
	"/data/log/log_file_system.1",
	"/data/log/log_file_system.2",
};

#define close(x)
#define remove(x)

static int vendor_misc_open_file(const char *p)
{
	int fd = -EBADF;
#if 0
	fd = open(p, O_RDWR | O_CREAT | O_EXCL | O_TRUNC , 0644);
	if (fd < 0 && errno == ENOENT) {
		vendor_misc_mkdirs(p);
		fd = open(p, O_RDWR | O_CREAT, 0644);
	}

	if (fd < 0 && errno == EEXIST)
		fd = open(p, O_RDWR | O_TRUNC);
#endif
	return fd;
}

static int vendor_misc_write_file(int fd, unsigned char *buf, unsigned int len)
{
#if 0
	unsigned int tmp, total_len;
	unsigned char *ptr = buf;

	total_len = len;

	while (total_len > 0) {
		tmp = write(fd, ptr, total_len);
		if (tmp > 0) {
			total_len -= tmp;
			ptr += tmp;
		} else {
			return -EIO;
		}
	}
#endif
	return -EIO;
}

static int vendor_misc_get_property(void *cmd)
{
	return 0;
}

static int vendor_misc_set_property(void *cmd)
{
	return 0;
}

static int vendor_misc_get_file_info(const char *path, struct vendor_file_info *download)
{
	int ret = -EINVAL;
#if 0
	int fd = -1;

	fd = imi_open(path, O_RDONLY);
	if (fd < 0) {
		log_e("unable to imi_open '%s'\n", path);
		return -ENOENT;
	}

	download->len = lseek(fd, 0, SEEK_END);
	if (download->len <= 0) {
		log_e("log file has no context\n");
		goto end;
	}

	lseek(fd, 0, SEEK_SET);
	download->data = (unsigned char *)pvPortMalloc(download->len * sizeof(char));
	if (download->data == NULL) {
		log_e("unable to allocate memory\n");
		download->len = 0;
		ret = -ENOMEM;
		goto end;
	}

	read(fd, download->data, download->len);

	download->init = 1;
	ret = 0;

end:
	if (fd != -1)
		close(fd);
#endif
	return ret;
}

static int vendor_misc_get_log(void *cmd)
{
	struct vendor_file_info *info = (struct vendor_file_info*)cmd;
	struct vendor_file_info *download = &download_file_info;
	const char *path = NULL;
	int ret = -EINVAL, len = 0;
#define MAX_TRANSFER_SIZE (50)

	if (info->type >= sizeof(log_file_path) / sizeof(log_file_path[0])) {
		log_e("file type err \n");
		goto fail;
	}

	path = log_file_path[info->type];

	if (!download->init) {
		ret = vendor_misc_get_file_info(path, download);
		if (ret)
			goto fail;
	}

	info->data = download->data;
	info->len = download->len;

	len = download->len - download->offset;
	len = len <= MAX_TRANSFER_SIZE ? len :  MAX_TRANSFER_SIZE;
	info->offset = download->offset;
	download->offset += len;

	if (download->offset == download->len)
		memset((void *)download, 0x00, sizeof(struct vendor_file_info));

	return len;

fail:
	info->data = download->data;
	memset((void *)&download, 0x00, sizeof(struct vendor_file_info));

	return ret;
}

static int vendor_misc_init_file_upload(void *cmd)
{
	struct vendor_file_info *ptr = (struct vendor_file_info *)cmd;
	struct vendor_file_info *upload = &upload_file_info;
	unsigned char index = 0;

	if (cmd == NULL)
		return -EINVAL;

	if (ptr->type >= VENDOR_FILE_MAX || ptr->len <= 0) {
		log_e("file info err\n");
		return IMI_PROTOCOL_FILE_TYPE_ERR;
	}

	index = sizeof(vendor_file_path_upload) / sizeof(vendor_file_path_upload[0]);
	if (ptr->type > index && ptr->type != VENDOR_FACTORY_TEST_PARA) {
		log_e("can not find file path\n");
		return IMI_PROTOCOL_FILE_TYPE_ERR;
	}

	memset((void *)upload, 0x00, sizeof(*upload));

	if (ptr->type == VENDOR_FACTORY_TEST_PARA) {
		upload->path = FACTORY_TEST_PARA_FILE;
	} else
		upload->path = vendor_file_path_upload[ptr->type];
	upload->type = ptr->type;
	upload->attr = ptr->attr;
	upload->len = ptr->len;
	upload->data = (unsigned char *)pvPortMalloc(ptr->len);
	if (upload->data == NULL) {
		log_e("can not pvPortMalloc file data\n");
		return -ENOMEM;
	}

	memset((void *)upload->data, 0x00, ptr->len);

	return 0;
}

static int vendor_misc_write_file_upload(void *cmd)
{
	struct vendor_file_info *info = (struct vendor_file_info*)cmd;
	struct vendor_file_info *upload = &upload_file_info;

	if (cmd == NULL)
		return -EINVAL;

	if (info->len + upload->offset > upload->len) {
		log_e("file len err\n");
		return IMI_PROTOCOL_FILE_OFFSET_ERR;
	}

	memcpy((void *)(upload->data + upload->offset), info->data, info->len);

	upload->offset += info->len;

	return 0;
}

static int vendor_misc_send_file_to_encry(struct vendor_file_info *info)
{
	struct imi_vendor_sensor_cmd cmd;
	int ret, val;

	cmd.cmd_type = VENDOR_CMD_SET_PROPERTY;

	if (info->type == VENDOR_PAY_AES_KEY) {
		val = IMI_PAY_PARAM_SEND_AES_KEY;
	} else if (info->type == VENDOR_PAY_FW) {
		val = IMI_PAY_PARAM_SEND_FW;
	} else {
		//nothing
	}

	cmd.sensor_param.type = val;
	cmd.sensor_param.data_ptr = info->data;
	cmd.sensor_param.data_len = info->len;

	ret = sub_system_process_events(VENDOR_STREAM_ENCRY, &cmd);
	if (ret) {
		log_e("send AES key fail\n");
		return ret;
	}

	return 0;
}

static int vendor_misc_finish_file_upload(void *cmd)
{
	struct vendor_file_info *info = (struct vendor_file_info*)cmd;
	struct vendor_file_info *upload = &upload_file_info;
	unsigned short crc = 0;
	int fd = 0;
	int ret = -EINVAL;

	if (cmd == NULL)
		goto fail;

	if (upload->offset != upload->len) {
		log_e("transfer err\n");
		goto fail;
	}

	crc = vendor_misc_calculate_crc16(upload->data, upload->len);

	if (info->type != upload->type || crc != info->crc) {
		log_e("crc or file type err\n");
		goto fail;
	}

	if (upload->type <= VENDOR_RECOVERY) {
		fd = vendor_misc_open_file(vendor_file_path_upload[upload->type]);
		if (fd < 0) {
			log_e("imi_open %s fail\n", vendor_file_path_upload[upload->type]);
			goto fail;
		}

		ret = vendor_misc_write_file(fd, upload->data, upload->len);
		if (ret) {
			log_e("write file err\n");
			close(fd);
			goto fail;
		}

		close(fd);
		if (upload->data)
			vPortFree(upload->data);
	} else if (upload->type == VENDOR_PAY_AES_KEY || upload->type == VENDOR_PAY_FW) {
		ret = vendor_misc_send_file_to_encry(upload);
		if (ret) {
			log_e("send pay key fail\n");
			goto fail;
		}

		if (upload->data)
			vPortFree(upload->data);

	} else if (upload->type == VENDOR_FACTORY_TEST_PARA) {
		fd = vendor_misc_open_file(FACTORY_TEST_PARA_FILE);
		if (fd < 0) {
			log_e("imi_open %s fail\n", FACTORY_TEST_PARA_FILE);
			goto fail;
		}

		ret = vendor_misc_write_file(fd, upload->data, upload->len);
		if (ret) {
			log_e("write file err\n");
			close(fd);
			goto fail;
		}

		close(fd);
		if (upload->data)
			vPortFree(upload->data);
	} else if (upload->type == VENDOR_FILE_DSP_ARM_PARAMS) {
		fd = vendor_misc_open_file(vendor_file_path_upload[upload->type]);
		if (fd < 0) {
			log_e("imi_open %s fail\n", vendor_file_path_upload[upload->type]);
			goto fail;
		}

		close(fd);

		if (upload->data)
			vPortFree(upload->data);
	} else {
		log_e("file type err\n");
		goto fail;
	}

	return 0;

fail:
	if (upload->data)
		vPortFree(upload->data);

	return ret;
}

static int vendor_misc_delete_file(void *cmd)
{
	unsigned short file_id = *((unsigned short *)cmd);
	const char *path = NULL;
	int ret = 0;

	if (cmd == NULL)
		return -EINVAL;

	if (file_id >= VENDOR_FILE_MAX) {
		log_e("file id fail\n");
		return IMI_PROTOCOL_FILE_TYPE_ERR;
	}

	path = vendor_file_path_upload[file_id];
	if (path) {
		//ret = remove(path);
		log_e("remove %s, ret = %d\n", path, ret);
	}

	path = vendor_file_path_download[file_id];
	if (path) {
		//ret = remove(path);
		log_e("remove %s, ret = %d\n", path, ret);
	}

	return 0;
}

static int vendor_misc_copy_file(const char *dst, const char *src)
{
#if 0
	int fd = -1;
	int len, ret = -EINVAL;
	char *buf = NULL;

	if (dst == NULL || src == NULL)
		return ret;

	fd = open(src, O_RDONLY);
	if (fd < 0) {
		log_e("unable to imi_open '%s'\n", src);
		ret = -ENOENT;
		return ret;
	}

	len = lseek(fd, 0, SEEK_END);
	if (len <= 0) {
		log_e("file has no context\n");
		goto fail;
	}

	buf = (char *)pvPortMalloc(len * sizeof(char));
	if (buf == NULL) {
		log_e("unable to allocate memory file context\n");
		ret = -ENOMEM;
		goto fail;
	}

	lseek(fd, 0, SEEK_SET);
	read(fd, buf, len);
	close(fd);

	fd = vendor_misc_open_file(dst);
	if (fd < 0) {
		log_e("imi_open %s fail\n", dst);
		ret = -ENOENT;
		goto fail;
	}

	write(fd, buf, len);
	ret = 0;
fail:
	if (fd != -1)
		close(fd);
	if (buf)
		vPortFree(buf);
	return ret;
#else
	return -EFAULT;
#endif
}

static int path_is_exist(const char *path)
{
#if 0
	struct stat st;
	if (stat(path, &st)){
		log_d("unable to imi_open '%s'\n", path);
		return 0;
	}
#endif
	return -ENOENT;
}

static int vendor_misc_download_file_init(struct vendor_file_info *download, int type)
{
	const char *path = NULL;
	int ret = -EINVAL;

	path = vendor_file_path_download[type];
	if (!path)  {
		log_e("can not find file path\n");
		return ret;
	}

	if (!path_is_exist(path))
		vendor_misc_copy_file(path, vendor_file_path_upload[type]);

	ret = vendor_misc_get_file_info(path, download);

	return ret;
}

static int vendor_misc_get_usb_speed(void)
{
	return 0;
}

static int vendor_misc_ram_file_init(struct vendor_file_info *download, int type)
{
	int len = sizeof(struct device_param);
	struct device_param *param = pvPortMalloc(len);
	int index, k, audio_type;

	if (param == NULL)
		return -ENOMEM;
	else
		memset((void *)param, 0x00, len);

	memcpy(param->serial, g_serial_no, SERIAL_NO_LEN);
	param->usb_speed = vendor_misc_get_usb_speed();

	for (index = VENDOR_STREAM_COLOR_USB; index <= VENDOR_STREAM_ENCRY_USB; index++) {
		k = index - VENDOR_STREAM_COLOR_USB;
		param->sensor_list[k].ep_num = 0x82;
		param->sensor_list[k].sensor_type = k;
	}

	audio_type = 0xD0;
	for (; index < VENDOR_STREAM_MAX; index++) {
		k = index - VENDOR_STREAM_COLOR_USB;
		param->sensor_list[k].ep_num = 0x82;
		param->sensor_list[k].sensor_type = audio_type;
		++audio_type;
	}

	param->sensor_list[index - VENDOR_STREAM_COLOR_USB].ep_num = 0xff;
	param->sensor_list[index - VENDOR_STREAM_COLOR_USB].sensor_type = 0xff;

	download->len = len;
	download->data = (unsigned char *)param;
	download->init = 1;

	return 0;
}

static int vendor_misc_dev_param_init(struct vendor_file_info *download, int type)
{
	int ret = -EINVAL;
	struct device_param *param;

	ret = vendor_misc_download_file_init(download, type);
	if (!ret) {
		param = (struct device_param *)download->data;
		param->usb_speed = vendor_misc_get_usb_speed();
	} else {
		ret = vendor_misc_ram_file_init(download, type);
	}

	return ret;
}

static int vendor_misc_get_rsa_key(struct vendor_file_info *download)
{
	struct imi_vendor_sensor_cmd cmd;
	int ret;

	cmd.cmd_type = VENDOR_CMD_GET_PROPERTY;

	cmd.sensor_param.type = IMI_PAY_PARAM_GET_RSA_KEY;
	download->data = (unsigned char*)pvPortMalloc(1024);
	if (download->data == NULL) {
		log_e("no mem\n");
		return -ENOMEM;
	}
	cmd.sensor_param.data_ptr = download->data;
//copy key to cmd.sensor_param.data_ptr and  fill cmd.sensor_param.data_len
	ret = sub_system_process_events(VENDOR_STREAM_ENCRY, &cmd);
	if (ret) {
		log_e("get RSA key fail\n");
		return -EFAULT;
	}

	download->len = cmd.sensor_param.data_len;
	download->init = 1;

	return 0;
}

static int vendor_misc_download_file(void *cmd)
{
	struct vendor_file_info *info = (struct vendor_file_info*)cmd;
	struct vendor_file_info *download = &download_file_info;
	int ret = -EINVAL, len = 0;
#define MAX_TRANSFER_SIZE (50)

	if (info->type >= VENDOR_FILE_MAX) {
		log_e("file type  err\n");
		goto fail;
	}

	if (info->offset > download->len) {
		log_e("file offset err\n");
		goto fail;
	}

	if (!download->init) {
		switch (info->type) {
		case VENDOR_FILE_DEVEICE_PARAMS:
			ret = vendor_misc_dev_param_init(download, info->type);
			if (ret) {
				log_e("vendor_misc_ram_file_init fail\n");
				return ret;
			}
			break;

		case VENDOR_PAY_RSA_KEY:
			ret = vendor_misc_get_rsa_key(download);
			if (ret) {
				log_e("vendor_misc_get_rsa_key fail\n");
				return ret;
			}
			break;

		default:
			ret = vendor_misc_download_file_init(download, info->type);
			if (ret) {
				log_e("vendor_misc_download_file_init fail\n");
				return ret;
			}
			break;
		}
	}

	info->data = download->data;
	info->len = download->len;

	len = info->len - info->offset;
	len = len <= MAX_TRANSFER_SIZE ? len :  MAX_TRANSFER_SIZE;

	if (info->offset + len == info->len)
		memset((void *)download, 0x00, sizeof(struct vendor_file_info));

	return len;

fail:
	info->data = download->data;
	memset((void *)&download, 0x00, sizeof(struct vendor_file_info));

	return ret;
}

struct vendor_misc_handle {
	unsigned int cmd_type;
	int (*callback)(void *cmd);
};

struct vendor_misc_handle vendor_misc_handle_map[] = {
	{VENDOR_CMD_GET_PROPERTY,               vendor_misc_get_property            },
	{VENDOR_CMD_SET_PROPERTY,               vendor_misc_set_property            },
	{VENDOR_CMD_GET_LOG,                    vendor_misc_get_log                 },
	{VENDOR_CMD_INIT_FILE_UPLOAD,           vendor_misc_init_file_upload        },
	{VENDOR_CMD_WRITE_FILE_UPLOAD,          vendor_misc_write_file_upload       },
	{VENDOR_CMD_FINISH_FILE_UPLOAD,         vendor_misc_finish_file_upload      },
	{VENDOR_CMD_DELETE_FILE,                vendor_misc_delete_file             },
	{VENDOR_CMD_DOWNLOAD_FILE,              vendor_misc_download_file           },
};

int vendor_misc_handle_cmd(unsigned int cmd_type, void *cmd)
{
	int index = 0;
	int len = sizeof(vendor_misc_handle_map) / sizeof(vendor_misc_handle_map[0]);

	for (index = 0; index < len; index++) {
		if (vendor_misc_handle_map[index].cmd_type == cmd_type) {
			return vendor_misc_handle_map[index].callback(cmd);
		}
	}
	log_e("can not find cmd\n");
	return -EINVAL;
}

static unsigned short crc16_table[256] = {
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

unsigned short vendor_misc_calculate_crc16(unsigned char *pData, unsigned int dataLen)
{
	unsigned short crc_reg = 0xFFFF;
	while (dataLen--)
	{
		crc_reg = (crc_reg >> 8) ^ crc16_table[(crc_reg ^ *pData++) & 0xff];
	}
	return (unsigned short)(~crc_reg) & 0xFFFF;
}

int vendor_misc_init(void)
{
	memset((void *)&upload_file_info, 0x00, sizeof(struct vendor_file_info));
	memset((void *)&download_file_info, 0x00, sizeof(struct vendor_file_info));

	return 0;
}

void vendor_misc_deinit(void)
{
}
