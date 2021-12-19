#include "debug.h"

void dump_dpu_setting(struct imi_dpu *idpu)
{
	int i;
	int base;
	base = (int)idpu->hw.dpu_rbase.depth;
	DPU_LOGV("dump dpu:\r\n");
	for (i = 0 ; i< 0x120; i+=0x20) {
		DPU_LOGV("%x = %08x %08x %08x %08x %08x %08x %08x %08x\r\n", base+i,
		read32(base+i), read32(base+i+4), read32(base+i+8), read32(base+i+0xc),
		read32(base+i+0x10), read32(base+i+0x14), read32(base+i+0x18), read32(base+i+0x1c));
	}
	base = (int)idpu->hw.dpu_rbase.fpp;
	DPU_LOGV("dump fpp:\r\n");
	for (i = 0 ; i< 0x40; i+=0x20) {
		DPU_LOGV("%x = %08x %08x %08x %08x %08x %08x %08x %08x\r\n", base+i,
		read32(base+i), read32(base+i+4), read32(base+i+8), read32(base+i+0xc),
		read32(base+i+0x10), read32(base+i+0x14), read32(base+i+0x18), read32(base+i+0x1c));
	}
	base = (int)idpu->hw.dpu_rbase.uvc_dep;
	DPU_LOGV("dump uvc_depth:\r\n");
	for (i = 0 ; i< 0x80; i+=0x20) {
		DPU_LOGV("%x = %08x %08x %08x %08x %08x %08x %08x %08x\r\n", base+i,
		read32(base+i), read32(base+i+4), read32(base+i+8), read32(base+i+0xc),
		read32(base+i+0x10), read32(base+i+0x14), read32(base+i+0x18), read32(base+i+0x1c));
	}
	base = (int)idpu->hw.dpu_rbase.uvc_ir;
	DPU_LOGV("dump uvc_ir:\r\n");
	for (i = 0 ; i< 0x80; i+=0x20) {
		DPU_LOGV("%x = %08x %08x %08x %08x %08x %08x %08x %08x\r\n", base+i,
		read32(base+i), read32(base+i+4), read32(base+i+8), read32(base+i+0xc),
		read32(base+i+0x10), read32(base+i+0x14), read32(base+i+0x18), read32(base+i+0x1c));
	}
}

