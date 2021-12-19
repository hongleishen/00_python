#ifndef __DPU_2280_H__
#define __DPU_2280_H__


#define IR_MIRROR_ENABLE_BIT           (1 << 8)

#define DPU_REGISTRATION_PARA_LEN	(13)


#define SFT_RANGE_X_R    256
#define SFT_RANGE_X      320
#define SFT_RANGE_Y      65
#define L1_SFT_RANGE_X	 (SFT_RANGE_X/2)
#define	L1_SFT_RANGE_Y	 1
#define L2_SFT_RANGE_X	 5
#define	L2_SFT_RANGE_Y	 1
#define	L1_PARA_LINE     8
#define REG_LE_SAD_THRES 34
#define REG_L1_SAD_THRES 0
#define REG_L2_SAD_THRES 0
#define REG_FRAME_ID     0
#define REG_SUB_PIXEL	 0
#define SADW             17

#define EPSINON 0.000001

#define get_ver_le_width() 15
#define get_ver_le_height(ir_height, sadw, le_calc_step_y) \
	(((ir_height) - (sadw)) / (le_calc_step_y) + (1))

#define get_hor_le_width(image_width, le_calc_step_x) \
	((image_width) / (le_calc_step_x))
#define get_hor_le_height(image_height, le_calc_step_y) \
	((image_height) / (le_calc_step_y))
#define get_hor_dy_ram_height(image_height, trans_block_rows) \
	((image_height) / (trans_block_rows) + (1))
#define get_ver_dy_ram_height(image_height, trans_block_rows) \
	(((image_height ) - (1)) / (trans_block_rows) + (1))

#define get_ref_height(ir_ref_height, shift_y_range) \
	((ir_ref_height) + (shift_y_range) -(1))

#define get_ref_width(ir_ref_width, shift_x_range) \
	((ir_ref_width) + (shift_x_range))

#define get_ref_read_base_addr(ref_row_start ,ref_width) \
	((ref_row_start) * (ref_width) * (2) / (32))

#define get_ref_frame_size(ref_height, ref_width) \
	((ref_height) * (ref_width) * (2) / (32) - (1))

#define get_sram_space_addr_horizontal(ref_height, ref_width) \
	(((ref_height) - (32)) * (ref_width) * (2) / (32) -(1))

#define get_sram_space_addr_vertical(ref_height ,ref_row_start, ref_width) \
	(((ref_height) - (ref_row_start)) * (ref_width) * (2) / (32) - (1))

#endif
