#ifndef __RVL_H__
#define __RVL_H__
#include "imi_type.h"
#include "vic_int.h"

#define RVL_BASE_ADDR           0x25100000

#define I_ADDRESS_IN            0x0000
#define I_ADDRESS_OUT           0x0004
#define I_WIDTH                 0x0008
#define I_HEIGHT                0x000C
#define I_FRAME_PIX             0x0010
#define AFWM_ID                 0x0014
#define CRC_DATA_1              0x0018
#define CRC_DATA_0              0x001C
#define INTR_ENABLE             0x0020
#define INTR_STAGE              0x0024
#define DATA_BIT                30
#define MODULE_EN_BIT           31

typedef int (* _rvl_out_data)(u32 src, u32 out, u32 width, u32 height);
typedef int (* _dpu_rvl_out_data)(u32 out, u32 width, u32 height);

struct rvl_driver{
    _rvl_out_data rvl_out_data;
    _dpu_rvl_out_data dpu_rvl_out_data;
};

irqreturn_t rvl_handler(int irq, void *arg);
int imi_rvl(u32 src, u32 out, u32 width, u32 height);
int imi_dpu_rvl(u32 out, u32 width, u32 height);

#endif
