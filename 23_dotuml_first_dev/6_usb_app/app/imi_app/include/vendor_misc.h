#ifndef __VENDOR_MISC_H__
#define  __VENDOR_MISC_H__

#define FACTORY_TEST_PARA_FILE "/tmp/factory_test.bin"
enum vendor_file_type {
	VENDOR_FILE_FIRMWARE = 0x00,
	VENDOR_FILE_REF_IMAGE,
	VENDOR_FILE_AR_PARAMS,
	VENDOR_FILE_CAMERA_PARAMS,
	VENDOR_FILE_DEVEICE_PARAMS,
	VENDOR_FILE_COLOR_FRAME_MODE_PARAMS,
	VENDOR_FILE_DEPTH_FRAME_MODE_PARAMS,
	VENDOR_FILE_IR_FRAME_MODE_PARAMS,
	VENDOR_RECOVERY,
	VENDOR_PAY_AES_KEY,
	VENDOR_PAY_RSA_KEY,
	VENDOR_PAY_FW,
	VENDOR_FILE_DSP_ARM_PARAMS,
	VENDOR_FACTORY_TEST_PARA = 0x20,
	VENDOR_FILE_MAX,
};

struct vendor_file_info {
	//add member at last
	unsigned short type;
	unsigned short attr;
	unsigned int len;
	unsigned short crc;
	unsigned int offset;
	unsigned char *data;
	const char *path;
	unsigned char init;
}__attribute__((packed));

int vendor_misc_init(void);
void vendor_misc_deinit(void);
int vendor_misc_handle_cmd(unsigned int cmd_type, void *cmd);
unsigned short vendor_misc_calculate_crc16(unsigned char *pData, unsigned int dataLen);

#endif
