#ifndef __VENDOR_CMD_H__
#define __VENDOR_CMD_H__

#define HOST_TO_DEVICE 0x4d49
#define DEVICE_TO_HOST 0x494d

#define VENDOR_CMD_HEADER_LEN 8

enum imi_stream_type {
	//for all sensor data stream
	VENDOR_STREAM_COLOR = 0x00,
	VENDOR_STREAM_DEPTH,
	VENDOR_STREAM_IR,
	VENDOR_STREAM_USER_TRACK,
	VENDOR_STREAM_FACE_DETECT,
	VENDOR_STREAM_SLAM,
	VENDOR_STREAM_FACE_AE, //add for faceae[0929]
	//for app use only
	VENDOR_STREAM_ENCRY, //depth + ir + jpeg
	VENDOR_STREAM_AUDIO_LEFT,
	VENDOR_STREAM_AUDIO_RIGHT,
	VENDOR_STREAM_BULK_OUT,
	VENDOR_STREAM_VPU,
	VENDOR_STREAM_DEVICE,
	VENDOR_STREAM_UNKNOWN,
	//for usb endpoint data stream
	VENDOR_STREAM_COLOR_USB = 0x10,
	VENDOR_STREAM_DEPTH_USB,
	VENDOR_STREAM_IR_USB,
	VENDOR_STREAM_USER_TRACK_USB,
	VENDOR_STREAM_FACE_DETECT_USB,
	VENDOR_STREAM_SLAM_USB,
	//for app use only
	VENDOR_STREAM_ENCRY_USB,
	VENDOR_STREAM_AUDIO_LEFT_USB,
	VENDOR_STREAM_AUDIO_RIGHT_USB,
	VENDOR_STREAM_MAX,
};

enum imi_protocol_ret_code
{
	IMI_PROTOCOL_OK                  = 0x00,
	IMI_PROTOCOL_PARSE_CMD_ERR,
	IMI_PROTOCOL_CMD_LENGH_ERR,
	IMI_PROTOCOL_CMD_OP_ERR,
	IMI_PROTOCOL_CMD_CNT_ERR,
	IMI_PROTOCOL_CMD_CRC_ERR,
	IMI_PROTOCOL_CMD_NOT_SUPPORT,
	IMI_PROTOCOL_CMD_OP_MISMATCH,

	IMI_PROTOCOL_IN_PROGRESS = 0x09,

	IMI_PROTOCOL_GET_VER_FAIL = 0x10,

	IMI_PROTOCOL_RESET_FAIL = 0x13,

	IMI_PROTOCOL_PROPERTY_TYPE_ERR = 0x16,
	IMI_PROTOCOL_PROPERTY_VALUE_ERR,

	IMI_PROTOCOL_STREAM_TYPE_ERR = 0x20,
	IMI_PROTOCOL_STREAM_MODE_ERR,
	IMI_PROTOCOL_OPEN_STREAM_FAIL,
	IMI_PROTOCOL_CLOSE_STREAM_FAIL,

	IMI_PROTOCOL_FILE_TYPE_ERR = 0x30,
	IMI_PROTOCOL_FILE_OFFSET_ERR,
	IMI_PROTOCOL_NO_DATA,

	IMI_PROTOCOL_LOG_TYPE_ERR = 0x40,

	IMI_PROTOCOL_REG_TYPE_ERR = 0x43,
	IMI_PROTOCOL_REG_ADDR_ERR,

	IMI_PROTOCOL_FAILURE             = 0xFFFF,
};

enum vendor_cmd {
	VENDOR_CMD_GET_VER = 0,
	VENDOR_CMD_RESET,
	VENDOR_CMD_KEEP_ALIVE,
	VENDOR_CMD_GET_PROPERTY,
	VENDOR_CMD_SET_PROPERTY,
	VENDOR_CMD_OPEN_STREAM,
	VENDOR_CMD_CLOSE_STREAM,
	VENDOR_CMD_SWTICH_SOURCE,
	VENDOR_CMD_GET_LOG,
	VENDOR_CMD_SET_CMOS_REG,
	VENDOR_CMD_GET_CMOS_REG, //0xa
	VENDOR_CMD_INIT_FILE_UPLOAD,
	VENDOR_CMD_WRITE_FILE_UPLOAD,
	VENDOR_CMD_FINISH_FILE_UPLOAD,
	VENDOR_CMD_DOWNLOAD_FILE,
	VENDOR_CMD_DELETE_FILE,
	VENDOR_CMD_SET_SERIAL_NUM,
	VENDOR_CMD_GET_SERIAL_NUM,
	VENDOR_CMD_SET_REGISTRATION_PARAM,
	VENDOR_CMD_GET_REGISTRATION_PARAM,
	VENDOR_CMD_SYSTEM_UPDATE = 0x30,
	VENDOR_CMD_CUSTOM_MAX,

	VENDOR_CMD_APP_INTERNAL = 1000,
	VENDOR_CMD_SET_FORMAT,
	VENDOR_CMD_USB_DISCONNECT,
};

enum vendor_device_param {
	VENDOR_DEVICE_PARAM_LD_OPERATE = 0,
	VENDOR_DEVICE_PARAM_LASER_MODE,

	VENDOR_DEVICE_PARAM_DEVICE = 0x1f4,

	VENDOR_DEVICE_PARAM_MAX = 0x257,
};

enum color_img_param {
	COLOR_IMG_PARAM_MIRROR = 0x259,
	COLOR_IMG_PARAM_SHARPNESS,
	COLOR_IMG_PARAM_BRIGHTNESS,
	COLOR_IMG_PARAM_CONTRAST,
	COLOR_IMG_PARAM_SATUARATION,
	COLOR_IMG_PARAM_GAIN,
	COLOR_IMG_PARAM_AUTO_WHITE_BALANCE_MODE,
	COLOR_IMG_PARAM_AUTO_EXPOSURE_MODE,
	COLOR_IMG_PARAM_ANTIFLICKER,
	COLOR_IMG_PARAM_INTRINSIC,

	COLOR_IMG_PARAM_FACE_INTO = 0x300,
	COLOR_IMG_PARAM_GET_IMG,

	COLOR_IMG_PARAM_ISP_INIT_PARA = 0x330,

	COLOR_IMG_PARAM_FRAME_MODE = 0x378,
	COLOR_IMG_PARAM_INCREASE_EXPOSURE_TIME,
	COLOR_IMG_PARAM_DECREASE_EXPOSURE_TIME,

	COLOR_IMG_PARAM_MAX = 0x383,
};

enum depth_img_param {
	DEPTH_IMG_PARAM_HOLE_FILTER = 0x385,
	DEPTH_IMG_PARAM_MIRROR,
	DEPTH_IMG_PARAM_DECIMATION,
	DEPTH_IMG_PARAM_DENOISE,
	DEPTH_IMG_PARAM_INTRINSIC,

	DEPTH_IMG_PARAM_FRAME_MODE = 0x4a4,

	DEPTH_IMG_PARAM_MAX = 0x4af,
};

enum ir_img_param {
	IR_IMG_PARAM_MIRROR = 0x4b1,
	IR_IMG_PARAM_EXPOSURE_TIME,
	IR_IMG_PARAM_DIGITAL_GAIN,
	IR_IMG_PARAM_ANALOG_GAIN,

	IR_IMG_PARAM_FRAME_MODE = 0x5a0,

	IR_IMG_PARAM_MAX = 0x5db,
};

enum user_track_param {
	USER_TRACK_PARAM_SMOOTH = 0x5dd,
	USER_TRACK_PARAM_GROUND_CLEANUP,
	USER_TRACK_PARAM_OUTPUT_FOREGROUND,
	USER_TRACK_PARAM_OUTPUT_SEG_MASK,

	USER_TRACK_PARAM_TRACK_MODE = 0x6fc,
	USER_TRACK_PARAM_TRACK_START,
	USER_TRACK_PARAM_TRACK_STOP,
	USER_TRACK_PARAM_TRACK_USER_FEATURE,

	USER_TRACK_PARAM_MAX = 0x707,
};

enum face_detect_param {
	FACE_DETECT_PARAM_FACE_DETECT = 0x828,
	FACE_DETECT_PARAM_MAX = 0x833,
};

enum slam_imu_param {
	SLAM_IMU_PARAM_MOVE_TRACK_SHOCK = 0x874,
	SLAM_IMU_PARAM_SLAM_CONTROL = 0x8B4,
	SLAM_IMU_PARAM_POSITON_TRACK = 0x954,
	SLAM_IMU_PARAM_MAX = 0x959,
};

enum imi_pay_param {
	IMI_PAY_PARAM_FRAME_OPEN = 0x961,
	IMI_PAY_PARAM_FRAME_CLOSE,
	IMI_PAY_PARAM_GET_RSA_KEY,
	IMI_PAY_PARAM_SEND_FW,
	IMI_PAY_PARAM_SEND_AES_KEY,
	IMI_PAY_PARAM_GET_CHIP_VER,
	IMI_PAY_PARAM_CHIP_INIT,
	IMI_PAY_PARAM_GET_SN,
	IMI_PAY_PARAM_MAX,
};

enum imi_echo_param {
	IMI_ECHO_DEBUG_SENSOR_OPEN = 0xA81,
	IMI_ECHO_DEBUG_SENSOR_CLOSE,
	IMI_ECHO_MAX,
};

enum heart_beat_param {
	HEART_BEAT_POLL = 0xa55a,
};

enum factory_test_param {
	IMI_RECOVERY_UPDATE_START = 0xb01,
	REF_IMG_SEND_START = 0xa56a,
	REF_IMG_SEND_END,
	DPU_LIGHT_CONTROL,
	FACTORY_TEST_PARAM_MAX,
};

enum img_resolution {
	IMI_RESOLUTION_320X240 = 0x00,
	IMI_RESOLUTION_320X244,
	IMI_RESOLUTION_320X248,

	IMI_RESOLUTION_640X480,
	IMI_RESOLUTION_640X488,
	IMI_RESOLUTION_640X512,

	IMI_RESOLUTION_800X600,
	IMI_RESOLUTION_1280X720,
	IMI_RESOLUTION_1280X1024,
	IMI_RESOLUTION_1600X1200,
	IMI_RESOLUTION_1920X1080,

	IMI_RESOLUTION_1280X976,
};

enum sensor_pixel_format {
	IMI_PIXEL_DEPTH_16BIT = 0x00,
	IMI_PIXEL_DEPTH_IR_16BIT,

	IMI_PIXEL_IR_10BIT = 100,
	IMI_PIXEL_IR_16BIT,
	IMI_PIXEL_IR_FLOOD_16BIT,

	IMI_PIXEL_COLOR_MJPEG = 200,
	IMI_PIXEL_COLOR_H264,

	IMI_PIXEL_COLOR_RGB888 = 230,
	IMI_PIXEL_COLOR_RGBA8888,

	IMI_PIXEL_COLOR_YUYV = 260,
	IMI_PIXEL_COLOR_NV12,
	IMI_PIXEL_COLOR_I420,

	IMI_PIXEL_INVALID = 400,
};

enum log_type {
	IMI_LOG_KERNEL = 0x00,
	IMI_LOG_APP,
};

enum imi_cmos_type {
	IMI_CMOS_TYPE_IMAGE = 0x00,
	IMI_CMOS_TYPE_DEPTH,
};

enum user_track_alg_mode {
	IMI_BODY_TRACK_MODE = 0x00,
	IMI_FACE_TRACK_MODE,
	IMI_HAND_TRACK_MODE,
};

enum pose_state {
	IMI_POSE_UNSURE = 0x00,
	IMI_POSE_STAND,
	IMI_POSE_SEATED,
};

enum face_alg_mode {
	IMI_FACE_DETECT_MODE = 0x00,
	IMI_FACE_IDENTIFIER_MODE,
	IMI_FACE_ANTI_SPOOFING_MODE,
	IMI_FACE_IDENTITIFER_AND_ANTI_SPOOFING_MODE,
	IMI_FACE_ECHO_FRAME_MODE,
};

enum face_select_mode {
	IMI_FACE_SELECT_ALL = 0x00,
	IMI_FACE_SELECT_LARGEST,
	IMI_FACE_SELECT_NEAREST,
};

enum imi_stream_cmd {
	IMI_STREAM_OFF = 0x00,
	IMI_STREAM_ON,
	IMI_STREAM_SUSPEND,
	IMI_STREAM_RESUME,
};

enum {
	DSP_CTRL_STREAM = 0x1,
	DSP_CTRL_PARAM,
	DSP_CTRL_LOG,
	DSP_CTRL_CMD,
};

enum {
	DSP_INCREASE_EXPOSURE_TIME = 0x0,
	DSP_DECREASE_EXPOSURE_TIME,
};

enum {
	DSP_PARAM_REQ_FILE = 0x835,
	DSP_PARAM_GET_RESOL,
	DSP_PARAM_GET_FILELEN,
	DSP_PARAM_SET_FILE,
};

struct imi_cmos_reg_info {
	//add mem at the last
	unsigned char type;
	unsigned int reg;
	unsigned int val;
}__attribute__((packed));

struct imi_vendor_cmd_header {
	//add mem at the last
	unsigned short flag;
	unsigned short data_len;
	unsigned short cmd;
	unsigned short cmd_cnt;
}__attribute__((packed));

struct imi_vendor_cmd_rsp_header {
	//add mem at the last
	struct imi_vendor_cmd_header rsp;
	unsigned short ret;
}__attribute__((packed));

#define VENDOR_CMD_RSP_HEADER_LEN sizeof(struct imi_vendor_cmd_rsp_header)

struct imi_vendor_device_ver {
	unsigned char fw_major;
	unsigned char fw_minor;
	unsigned short fw_ver;

	unsigned char ap_major;
	unsigned char ap_minor;
	unsigned short ap_ver;

	unsigned char hardware;
	unsigned char chip;
	unsigned char ap_hardware;
}__attribute__((packed));

struct imi_sub_system_version {
	unsigned char dpu_sw_major;
	unsigned char dpu_sw_minor;

	unsigned char vpu_sw_major;
	unsigned char vpu_sw_minor;

	unsigned char isp_sw_major;
	unsigned char isp_sw_minor;

	unsigned char dsp0_sw_major;
	unsigned char dsp0_sw_minor;

	unsigned char dsp1_sw_major;
	unsigned char dsp1_sw_minor;

	unsigned char dsp_load_mode;
}__attribute__((packed));


struct imi_sensor_reg_info {
	//add mem at the last
	unsigned int  addr;
	unsigned int  value;
}__attribute__((packed));

struct imi_sensor_param_info {
	//add mem at the last
	unsigned short type;
	union {
		unsigned char data[50];
		unsigned char *data_ptr;
	}__attribute__((packed));
	unsigned short data_len;
}__attribute__((packed));

struct imi_frame_format_info {
	//add mem at the last
	enum sensor_pixel_format pixel;
	unsigned short width;
	unsigned short high;
	unsigned char bits_per_pixel;
	unsigned char fps;
}__attribute__((packed));

struct user_track_mode {
	enum user_track_alg_mode alg_type;
	enum pose_state state;
	unsigned int width;
	unsigned int heigh;
	unsigned int fps;
}__attribute__((packed));

struct imi_one_frame {
	enum imi_stream_type type;
	unsigned int addr;
	unsigned int size;
}__attribute__((packed));

struct face_detect_mode {
	enum face_alg_mode alg_type;
	enum face_select_mode select_mdoe;
}__attribute__((packed));

struct imi_vendor_sensor_cmd {
	enum vendor_cmd cmd_type;
	union {
		struct imi_sensor_reg_info sensor_reg;
		struct imi_sensor_param_info sensor_param;
		struct imi_frame_format_info fmt;
	};
}__attribute__((packed));

struct imi_stream_ctl_cmd {
	unsigned short stream_type;
	union {
		int mode;
		struct imi_frame_format_info fmt;
		struct user_track_mode user_track;
		struct face_detect_mode face_detect;
	};
	enum imi_stream_cmd onoff;
	unsigned short stream_2_usb;
}__attribute__((packed));

#endif

