#ifndef __LOAD_PARA_H__
#define __LOAD_PARA_H__

#include "fpp.h"

#define REF_BASE_ADDR_DEPTH			(FPGA_DPU_BASE_ADDR + 0x41000)
#define REF_BASE_ADDR_SHIFT			(FPGA_DPU_BASE_ADDR + 0x42000)
#define REF_RD_CFG0					0x0 /* src_addr0 */
#define REF_RD_CFG1					0x4 /* src_addr1 */
#define REF_RD_CFG2					0x8 /* ref_frame_size */
#define REF_RD_CFG3					0xC /* max burst length */
#define REF_RD_CFG4					0x10 /* mode */
#define REF_RD_CFG5					0x14 /* dma_ena */
#define REF_RD_CH_0					0x18 /* read only, dma status*/
#define REF_RD_CH_1					0x1C /* read only, frame count*/
#define DEBUG_WR_CFG0				0x20 /* src_addr0 */
#define DEBUG_WR_CFG1				0x24 /* src_addr1 */
#define DEBUG_WR_CFG2				0x28 /* line stride*/
#define DEBUG_WR_CFG3				0x2C /* max burst length */
#define DEBUG_WR_CFG4				0x30 /* mode */
#define DEBUG_WR_CFG5				0x34 /* dma_en */
#define DEBUG_WR_CH_0				0x38 /* read only, dma status*/
#define DEBUG_WR_CH_1				0x3C /* read only, frame count*/
#define DEBUG_IMG_CFG0				0x40 /* img width / height */
#define DEBUG_IMG_CFG1				0x44 /* frame size */
#define DEBUG_FIFO_CTRL				0x4c /* fifo overflow */
#define REF_INT_ENA					0x50 /* dma done*/
#define REF_INR_RAW_STS				0x54
#define REF_INT_STS					0x58

#define REF_SRC_ADDR0				0xD0000000
#define REF_SRC_ADDR1				0xD1000000
#define DEBUG_SRC_ADDR0				0xD2000000
#define DEBUG_SRC_ADDR1				0xD3000000


void dpu_dump_data(struct stream_ctrl_cmd *stream_ctrl_cmd);
void ref_cfg_depth(struct stream_ctrl_cmd *stream_ctrl_cmd);
void ref_cfg_shitf(struct stream_ctrl_cmd *stream_ctrl_cmd);

#endif//__LOAD_PARA_H__
