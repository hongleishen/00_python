#if defined(CHIP_IMI_1680)
#include "imi_type.h"
#include "imi_io.h"
#include "csi_rx.h"

typedef struct imi_campw_s {
	unsigned int mclkp;
	unsigned int strbp;
	unsigned int trigp;
	unsigned int shutdnp;
	unsigned int rstp;
}IMI_CAMPW_S, *IMI_CAMPW_T;

typedef enum {
	SNR_HDR_DISABLE = 0,
	SNR_HDR_2_FRAME,
	SNR_HDR_3_FRAME
} SNR_HDR_EN;

typedef struct imi_cam_param {
	unsigned short     id;
	unsigned char      again;
	unsigned char      fps;
	unsigned short     height;
	unsigned short     width;
	unsigned short     sysclk;		//Mhz
	unsigned short     exposure;	//us
	unsigned int       vts;
	unsigned int       hts;
	unsigned short     trigen;
	SNR_HDR_EN         hdren;
	struct i2c_client  *slave;
	struct dw_i2c_dev  *dev;
	IMI_MIPI_S         mipi_cfg;
	IMI_CAMPW_S        pw;
}IMI_CAM_PARAMS, *PIMI_CAM_PARAMS;

// for camera features set
typedef enum {
	SENSOR_FEATURE_BEGIN = 0,

	SENSOR_FEATURE_SET_AGAIN,
	SENSOR_FEATURE_SET_DGAIN,

	SENSOR_FEATURE_GET_RESOLUTION,
	SENSOR_FEATURE_GET_AF_STATUS,
	SENSOR_FEATURE_GET_AE_STATUS,
	SENSOR_FEATURE_GET_AWB_STATUS,

	SENSOR_FEATURE_SET_REGISTER,
	SENSOR_FEATURE_GET_REGISTER,

	SENSOR_FEATURE_MAX
} IMI_SENSOR_FEATURE;


typedef struct imi_cam_funcs {
	int (*sensor_open)(IMI_CAM_PARAMS *ircam);	//power up, reset, check chip_id
	int (*sensor_regs_init)(IMI_CAM_PARAMS *ircam);// sensor regs init
	int (*sensor_get_info)(IMI_SENSOR_FEATURE feat, char *pparam);
	int (*sensor_set_feature)(IMI_SENSOR_FEATURE feat, char *pparam);
	int (*sensor_start_stream)(IMI_CAM_PARAMS *ircam);
	int (*sensor_close)(IMI_CAM_PARAMS *ircam);
}IMI_CAM_FUNCS, *PIMI_CAM_FUNCS;

#ifndef IMI_SENSOR_LIST
typedef struct imi_sensor_list {
	unsigned char name[16];
	unsigned short id;
	int (* sensor_init)(PIMI_CAM_FUNCS *funcs);
} IMI_SENSOR_LIST;
#endif
#endif/*CHIP_IMI_1680*/
