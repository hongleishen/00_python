#include <stdlib.h>

#define CR_M    (1 << 0)    /* MMU enable               */
#define CR_A    (1 << 1)    /* Alignment abort enable       */
#define CR_C    (1 << 2)    /* Dcache enable            */
#define CR_W    (1 << 3)    /* Write buffer enable          */
#define CR_P    (1 << 4)    /* 32-bit exception handler     */
#define CR_D    (1 << 5)    /* 32-bit data address range        */
#define CR_L    (1 << 6)    /* Implementation defined       */
#define CR_B    (1 << 7)    /* Big endian               */
#define CR_S    (1 << 8)    /* System MMU protection        */
#define CR_R    (1 << 9)    /* ROM MMU protection           */
#define CR_F    (1 << 10)   /* Implementation defined       */
#define CR_Z    (1 << 11)   /* Implementation defined       */
#define CR_I    (1 << 12)   /* Icache enable            */
#define CR_V    (1 << 13)   /* Vectors relocated to 0xffff0000  */
#define CR_RR   (1 << 14)   /* Round Robin cache replacement    */
#define CR_L4   (1 << 15)   /* LDR pc can set T bit         */
#define CR_DT   (1 << 16)
#define CR_IT   (1 << 18)
#define CR_ST   (1 << 19)
#define CR_FI   (1 << 21)   /* Fast interrupt (lower latency mode)  */
#define CR_U    (1 << 22)   /* Unaligned access operation       */
#define CR_XP   (1 << 23)   /* Extended page tables         */
#define CR_VE   (1 << 24)   /* Vectored interrupts          */
#define CR_EE   (1 << 25)   /* Exception (Big) Endian       */
#define CR_TRE  (1 << 28)   /* TEX remap enable         */
#define CR_AFE  (1 << 29)   /* Access flag enable           */
#define CR_TE   (1 << 30)   /* Thumb exception enable       */

#define TTB_SECT_AP     (3 << 10)
/* options available for data cache on each page */
enum dcache_option {
	DCACHE_OFF = 0x12,
	DCACHE_WRITETHROUGH = 0x1a,
	DCACHE_WRITEBACK = 0x1e,
	DCACHE_WRITEALLOC = 0x16,
};

enum {
	MMU_SECTION_SHIFT   = 20, /* 1MB */
	MMU_SECTION_SIZE    = 1 << MMU_SECTION_SHIFT,
};

static unsigned int tlb_addr[4096] __attribute__((aligned(0x4000)));
#define TLB_ADDRESS tlb_addr

static inline unsigned int get_cr(void)
{
	unsigned int val;

	asm volatile("mrc p15, 0, %0, c1, c0, 0 @ get CR" : "=r" (val)
								  :
								  : "cc");
	return val;
}

static inline void set_cr(unsigned int val)
{

	asm volatile("mcr p15, 0, %0, c1, c0, 0 @ set CR" :
								  : "r" (val)
								  : "cc");

	asm volatile ("" : : : "memory");
}
extern int imi_printf(const char *fmt, ...);

static void set_section_dcache(int section, enum dcache_option option)
{
	unsigned int *page_table = (unsigned int *)TLB_ADDRESS;
	unsigned int value = TTB_SECT_AP;

	/* Add the page offset */
	value |= ((unsigned int)section << MMU_SECTION_SHIFT);

	/* Add caching bits */
	value |= option;

	/* Set PTE */
	page_table[section] = value;
}

static void set_int_vector_mmap(void)
{
	unsigned int *page_table = (unsigned int *)TLB_ADDRESS;
	unsigned int value = TTB_SECT_AP;

	/* Add the page offset */
	value |= ((unsigned int)CONFIG_INT_VECTOR_ADDR);

	/* Add caching bits */
	value |= DCACHE_WRITETHROUGH;

	/* Set PTE */
	page_table[0xfff] = value;
}

static void dram_bank_mmu_setup(int bank)
{
	unsigned int    i;

	for (i = CONFIG_ENTRY_ADDR >> MMU_SECTION_SHIFT;
		 i < (CONFIG_DMA_START_ADDR >> MMU_SECTION_SHIFT);
		 i++) {
		set_section_dcache(i, DCACHE_WRITEBACK);
	}

	for (i = CONFIG_DMA_START_ADDR >> MMU_SECTION_SHIFT;
		 i < (CONFIG_INT_VECTOR_ADDR >> MMU_SECTION_SHIFT);
		 i++) {
		set_section_dcache(i, DCACHE_WRITETHROUGH);
	}
}

/* to activate the MMU we need to set up virtual memory: use 1M areas */
static inline void mmu_setup(void)
{
	unsigned int i;
	unsigned int reg;

	/* Set up an identity-mapping for all 4G, rw for everyone */
	for (i = 0; i < ((4096ULL * 1024 * 1024) >> MMU_SECTION_SHIFT); i++) {
		set_section_dcache(i, DCACHE_OFF);
	}

	dram_bank_mmu_setup(0);
	set_int_vector_mmap();

	/* Copy the page table address to cp15 */
	asm volatile("mcr p15, 0, %0, c2, c0, 0"
			 : : "r" (TLB_ADDRESS) : "memory");

	/* Set the access control to all-supervisor */
	asm volatile("mcr p15, 0, %0, c3, c0, 0"
			 : : "r" (~0));

	/* and enable the mmu */
	reg = get_cr(); /* get control reg. */
	set_cr(reg | CR_M | CR_V);
}

static int mmu_enabled(void)
{
	return get_cr() & CR_M;
}


/* cache_bit must be either CR_I or CR_C */
static void cache_enable(unsigned int cache_bit)
{
	unsigned int reg;

	/* The data cache is not active unless the mmu/mpu is enabled too */
	if ((cache_bit == CR_C) && !mmu_enabled())
		mmu_setup();

	reg = get_cr(); /* get control reg. */
	set_cr(reg | cache_bit);
}

static void flush_dcache_all(void)
{
	asm volatile(
		"0:"
		"mrc p15, 0, r15, c7, c14, 3\n"
		"bne 0b\n"
		"mcr p15, 0, %0, c7, c10, 4\n"
		 : : "r"(0) : "memory"
	);
}

/* cache_bit must be either CR_I or CR_C */
static void cache_disable(unsigned int cache_bit)
{
	unsigned int reg;

	reg = get_cr();

	if (cache_bit == CR_C) {
		/* if cache isn;t enabled no need to disable */
		if ((reg & CR_C) != CR_C)
			return;
		/* if disabling data cache, disable mmu too */
		cache_bit |= CR_M;
	}
	reg = get_cr();

	if (cache_bit == (CR_C | CR_M))
		flush_dcache_all();
	set_cr(reg & ~cache_bit);
}

void icache_enable(void)
{
	cache_enable(CR_I);
}

void icache_disable(void)
{
	cache_disable(CR_I);
}

int icache_status(void)
{
	return (get_cr() & CR_I) != 0;
}

void dcache_enable(void)
{
	cache_enable(CR_C);
}

void dcache_disable(void)
{
	cache_disable(CR_C);
}

int dcache_status(void)
{
	return (get_cr() & CR_C) != 0;
}
