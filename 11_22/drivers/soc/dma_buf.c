/*
	Simple dma buf driver, only support alloc!
*/

#include <stdlib.h>
#include "imi_type.h"
#include "printf.h"
#include "linux_compat.h"

#define DMA_BUF_SIZE 0x100000
#define DMA_BUF_ALLOC_ADDR (CONFIG_INT_VECTOR_ADDR - DMA_BUF_SIZE)
#define DMA_ALIGN_SIZE 32

static unsigned int next_addr = DMA_BUF_ALLOC_ADDR;

unsigned int dma_buf_alloc(unsigned int size)
{
	unsigned int addr;
	unsigned int val;

	val = ((size / DMA_ALIGN_SIZE) + 1) * DMA_ALIGN_SIZE; //align to DMA_ALIGN_SIZE
	if (next_addr + val >= CONFIG_INT_VECTOR_ADDR) {
		pr_err("alloc dma buf err\n");
		return 0;
	}

	addr = next_addr;
	next_addr += val;

	return addr;
}