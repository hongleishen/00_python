#ifndef __LINUX_COMPAT_H__
#define __LINUX_COMPAT_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <portmacro.h>
#include <string.h>
#include "printf.h"

typedef xSemaphoreHandle FRTOS_mutex;
#if configUSE_RECURSIVE_MUTEXES != 1
	#error "configUSE_RECURSIVE_MUTEXES in FreeRTOSConfig.h need to 1"
#endif

void FRTOS_mutex_create( FRTOS_mutex * mutex_ptr );
void FRTOS_mutex_free( FRTOS_mutex * mutex_ptr );
void FRTOS_mutex_lock( FRTOS_mutex mutex );
void FRTOS_mutex_unlock( FRTOS_mutex mutex );

typedef SemaphoreHandle_t FRTOS_sem_t;

FRTOS_sem_t  FRTOS_sem_create(unsigned char count);
void  FRTOS_sem_free(FRTOS_sem_t sem);
unsigned int  FRTOS_sem_wait(FRTOS_sem_t sem, unsigned int timeout);
void  FRTOS_sem_signal(FRTOS_sem_t sem);
void FRTOS_sem_signal_from_isr(FRTOS_sem_t sem);

typedef SemaphoreHandle_t FRTOS_completion_t;

FRTOS_completion_t FRTOS_completion_init(void);
void FRTOS_completion_reinit(FRTOS_completion_t x);
void FRTOS_complete(FRTOS_completion_t x);
//timeout : how many ticks, 0 is wait forever
unsigned int FRTOS_completion_wait(FRTOS_completion_t x, unsigned int timeout);

int ffs(int x);

void vPortEnterCritical( void );
void vPortExitCritical( void );

/*
 * Save the current interrupt enable state & disable IRQs
 */
unsigned long local_irq_save(void);
void local_irq_restore(unsigned long flags);

#define spin_lock_irqsave(a, flags)         do { \
	flags = local_irq_save(); \
	} while (0)

#define spin_unlock_irqrestore(a, flags)    do { \
	local_irq_restore(flags); \
	} while (0)

#define spin_lock(a)    vPortEnterCritical();
#define spin_unlock(a)  vPortExitCritical();
#define spin_lock_init(a)               *a = 0
#define EXPORT_SYMBOL_GPL(...)

#define platform_data device_data

#ifndef wmb
#define wmb()           asm volatile (""   : : : "memory")
#endif

TickType_t msecs_to_jiffies(unsigned int ms);
void msleep(unsigned int ms);

//===============================================================================
#include <stddef.h>

#ifndef unicode_t
typedef u32 unicode_t;
#endif

#ifndef dma_addr_t
typedef unsigned int dma_addr_t;
#endif
typedef unsigned short umode_t;

#ifdef __CHECKER__
#define __bitwise__ __attribute__((bitwise))
#define __force    __attribute__((force))
#define __acquires(x)   __attribute__((context(x,0,1)))
#define __releases(x)   __attribute__((context(x,1,0)))
#else
#define __bitwise__
#define __force
#define __releases(x)
#define __acquires(x)
#endif
#ifdef __CHECK_ENDIAN__
#define __bitwise __bitwise__
#else
#define __bitwise
#endif

typedef __u16 __bitwise __le16;
typedef __u16 __bitwise __be16;
typedef __u32 __bitwise __le32;
typedef __u32 __bitwise __be32;
#if defined(__GNUC__)
typedef __u64 __bitwise __le64;
typedef __u64 __bitwise __be64;
#endif

typedef unsigned int    gfp_t;
typedef int spinlock_t;

#define array_size(x)   (sizeof(x) / sizeof((x)[0]))

#define GFP_ATOMIC ((gfp_t) 0)
#define GFP_KERNEL ((gfp_t) 0)
#define GFP_NOFS ((gfp_t) 0)
#define GFP_USER ((gfp_t) 0)
#define __GFP_NOWARN ((gfp_t) 0)
#define __GFP_ZERO  ((__force gfp_t)0x8000u)    /* Return zeroed page on success */

#define CONFIG_SYS_CACHELINE_SIZE   32
#define BITS_PER_LONG 32

#define CONFIG_USB_GADGET_VBUS_DRAW 50


#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({          \
		const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
		(type *)( (char *)__mptr - offsetof(type, member) );})

#define __iomem

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) < (b)) ? (b) : (a))

#define min_t(t, a, b) (((a) < (b)) ? (a) : (b))
#define max_t(t, a, b) (((a) < (b)) ? (b) : (a))

#define DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))
#ifndef BIT
#define BIT(nr)         (1UL << (nr))
#endif
#define BIT_ULL(nr)     (1ULL << (nr))
#define BIT_MASK(nr)        (1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)        ((nr) / BITS_PER_LONG)
#define BIT_ULL_MASK(nr)    (1ULL << ((nr) % BITS_PER_LONG_LONG))
#define BIT_ULL_WORD(nr)    ((nr) / BITS_PER_LONG_LONG)
#define BITS_PER_BYTE       8
#define BITS_TO_LONGS(nr)   DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))

#define DECLARE_BITMAP(name, bits) \
	unsigned long name[BITS_TO_LONGS(bits)]

#define small_const_nbits(nbits) \
	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG)

#define __constant_cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define __cpu_to_le64(x) ((__force __le64)(__u64)(x))
#define __cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define __le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define __cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define __le16_to_cpu(x) ((__force __u16)(__le16)(x))

#define cpu_to_le64 __cpu_to_le64
#define cpu_to_le32 __cpu_to_le32
#define le32_to_cpu __le32_to_cpu
#define cpu_to_le16 __cpu_to_le16
#define le16_to_cpu __le16_to_cpu

#define upper_32_bits(n) ((u32)(((n) >> 16) >> 16))
#define lower_32_bits(n) ((u32)(n))

#define likely(x)  __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

#define IS_ALIGNED(x, a)        (((x) & ((typeof(x))(a) - 1)) == 0)

#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)

#ifndef __aligned
#define __aligned(x)        __attribute__((aligned(x)))
#endif
#define __weak      __attribute__((weak))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

extern void * memset(void * s,int c,size_t count);
static inline void bitmap_zero(unsigned long *dst, int nbits)
{
	if (small_const_nbits(nbits))
		*dst = 0UL;
	else {
		int len = BITS_TO_LONGS(nbits) * sizeof(unsigned long);
		memset((void*)dst, 0, len);
	}
}

//dma
enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};

//nls
enum utf16_endian {
	UTF16_HOST_ENDIAN,
	UTF16_LITTLE_ENDIAN,
	UTF16_BIG_ENDIAN
};

static inline int test_bit(int nr, const void * addr)
{
	return ((unsigned char *) addr)[nr >> 3] & (1U << (nr & 7));
}

static inline void __put_unaligned_le16(u16 val, u8 *p)
{
	*p++ = val;
	*p++ = val >> 8;
}
static inline void __put_unaligned_le32(u32 val, u8 *p)
{
	__put_unaligned_le16(val >> 16, p + 2);
	__put_unaligned_le16(val, p);
}
static inline void put_unaligned_le16(u16 val, void *p)
{
	__put_unaligned_le16(val, p);
}
static inline void put_unaligned_le32(u32 val, void *p)
{
	__put_unaligned_le32(val, p);
}

static inline void le16_add_cpu(__le16 *var, u16 val)
{
	*var = cpu_to_le16(le16_to_cpu(*var) + val);
}

static inline void le32_add_cpu(__le32 *var, u32 val)
{
	*var = cpu_to_le32(le32_to_cpu(*var) + val);
}

static inline void generic_set_bit(int nr, volatile unsigned long *addr)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

	*p  |= mask;
}

extern int imi_printf(const char *fmt, ...);

#define set_gadget_data(g, data)    g->private_data = (void *)data
#define get_gadget_data(g)          (g->private_data)

#define BUG() do { \
	imi_printf("BUG at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
} while (0)

#define BUG_ON(condition) do { if ((condition)) BUG(); } while (0)

#define WARN_ON(condition) ({                       \
	int __ret_warn_on = !!(condition);              \
	if ((__ret_warn_on))                    \
		imi_printf("WARNING at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
	(__ret_warn_on);                    \
})
#define WARN(condition, format...) ({                   \
	int __ret_warn_on = !!(condition);              \
	if (__ret_warn_on)                    \
		imi_printf(format);                  \
		__ret_warn_on;                    \
})

#define WARN_ON_ONCE(condition) ({              \
	static bool __warned;                   \
	int __ret_warn_once = !!(condition);            \
								\
	if ((__ret_warn_once && !__warned)) {       \
		__warned = true;                \
		WARN_ON(1);                 \
	}                           \
	(__ret_warn_once);              \
})

#define WARN_ONCE(condition, format...) ({          \
	static bool __warned;     \
	int __ret_warn_once = !!(condition);            \
								\
	if ((__ret_warn_once && !__warned)) {       \
		__warned = true;                \
		WARN(1, format);                \
	}                           \
	(__ret_warn_once);              \
})

#define err(fmt, ...) imi_printf(fmt, ##__VA_ARGS__)

#define     KLOG_LVL_ERR     0
#define     KLOG_LVL_WARN    1
#define     KLOG_LVL_DEBUG   2
#define     KLOG_LVL_INFO    3

#define IMI_KLOG_LVL   KLOG_LVL_ERR

#if IMI_KLOG_LVL >= KLOG_LVL_ERR
#define dev_err(dev, fmt, ...) do {(void)(dev); print_cur_time(); imi_printf(fmt, ##__VA_ARGS__);} while(0)
#else
#define dev_err(dev, fmt...) do {(void)(dev); } while (0)
#endif

#if IMI_KLOG_LVL >= KLOG_LVL_WARN
#define dev_warn(dev, fmt, ...) do {(void)(dev); print_cur_time(); imi_printf(fmt, ##__VA_ARGS__);} while(0)
#else
#define dev_warn(dev, fmt...) do {(void)(dev); } while (0)
#endif

#if IMI_KLOG_LVL >= KLOG_LVL_DEBUG
#define dev_dbg(dev, fmt, ...) do {(void)(dev); print_cur_time(); imi_printf(fmt, ##__VA_ARGS__);} while(0)
#else
#define dev_dbg(dev, fmt...) do {(void)(dev); } while(0)
#endif

#if IMI_KLOG_LVL >= KLOG_LVL_DEBUG
#define dev_vdbg(dev, fmt, ...) do {(void)(dev); print_cur_time(); imi_printf(fmt, ##__VA_ARGS__);} while(0)
#else
#define dev_vdbg(dev, fmt...) do { (void)(dev);} while (0)
#endif

#if IMI_KLOG_LVL >= KLOG_LVL_DEBUG
#define debug(fmt, ...) do {imi_printf(fmt, ##__VA_ARGS__);} while(0)
#else
#define debug(fmt...) do { } while (0)
#endif

#if IMI_KLOG_LVL >= KLOG_LVL_DEBUG
#define pr_debug(fmt, ...) do {imi_printf(fmt, ##__VA_ARGS__);} while(0)
#else
#define pr_debug(fmt, ...) do {}while(0)
#endif

#if IMI_KLOG_LVL >= KLOG_LVL_INFO
#define dev_info(dev, fmt, ...) do {(void)(dev); print_cur_time(); imi_printf(fmt, ##__VA_ARGS__);} while(0)
#else
#define dev_info(dev, fmt...) do {(void)(dev); } while (0)
#endif

#define compiletime_assert(...)

static inline void *ERR_PTR(long error)
{
	return (void *) error;
}
static inline long IS_ERR(const void *ptr)
{
	return ((unsigned long)(ptr) >= (unsigned long)-4095);
}
static inline long PTR_ERR(const void *ptr)
{
	return (long) ptr;
}

static inline void * ERR_CAST(__force const void *ptr)
{
	/* cast away the const */
	return (void *) ptr;
}

extern int utf8s_to_utf16s(const u8 *s, int len,
		enum utf16_endian endian, wchar_t *pwcs, int maxlen);


#define     dma_mapping_error(x, y) 0
#define     is_vmalloc_addr(x)      0
#define     object_is_on_stack(x)   0

static inline void dma_free_coherent(void *addr)
{
	//
}

static inline unsigned long dma_map_single(volatile void *vaddr, size_t len,
		enum dma_data_direction dir)
{
	return (unsigned long)vaddr;
}

static inline void dma_unmap_single(volatile void *vaddr, size_t len,
		unsigned long paddr)
{
}

struct device {
	const char *name;
	struct device       *parent;
	void    (*release)(struct device *dev);
	/* This is used from drivers/usb/musb-new subsystem only */
	void        *driver_data;   /* data private to the driver */
	void            *device_data;   /* data private to the device */
};


//================================================poll==========================
/* These are specified by iBCS2 */
#define POLLIN      0x0001
#define POLLPRI     0x0002
#define POLLOUT     0x0004
#define POLLERR     0x0008
#define POLLHUP     0x0010
#define POLLNVAL    0x0020

/* The rest seem to be more-or-less nonstandard. Check them! */
#define POLLRDNORM  0x0040
#define POLLRDBAND  0x0080
#ifndef POLLWRNORM
#define POLLWRNORM  0x0100
#endif
#ifndef POLLWRBAND
#define POLLWRBAND  0x0200
#endif
#ifndef POLLMSG
#define POLLMSG     0x0400
#endif
#ifndef POLLREMOVE
#define POLLREMOVE  0x1000
#endif
#ifndef POLLRDHUP
#define POLLRDHUP       0x2000
#endif

#define POLLFREE    0x4000  /* currently only for epoll */

#define POLL_BUSY_LOOP  0x8000
//==================================================================

#define swab16(x) ((__u16)(             \
		(((__u16)(x) & (__u16)0x00ffU) << 8) |          \
		(((__u16)(x) & (__u16)0xff00U) >> 8)))

#define swab32(x) ((__u32)(             \
		(((__u32)(x) & (__u32)0x000000ffUL) << 24) |        \
		(((__u32)(x) & (__u32)0x0000ff00UL) <<  8) |        \
		(((__u32)(x) & (__u32)0x00ff0000UL) >>  8) |        \
		(((__u32)(x) & (__u32)0xff000000UL) >> 24)))

static inline void *devm_kzalloc(struct device *dev, size_t size, gfp_t gfp)
{
	void *mem = 0;
	mem = pvPortMalloc(size);
	if (mem) {
		memset(mem, 0, size);
	}
	return mem;
}

unsigned int dma_buf_alloc(unsigned int size);

struct mutex {
	FRTOS_mutex fmutex;
};

// un-imple marcros:  /by suntongce
//mutex
#define DEFINE_MUTEX(a)         struct mutex a

void mutex_lock(struct mutex *lock);
void mutex_unlock(struct mutex *lock);
void mutex_init(struct mutex *lock);

//#define mutex_lock(lock)    vPortEnterCritical()
//#define mutex_unlock(lock)  vPortExitCritical()

//clock
#define clk_disable_unprepare(a)    do{}while(0)
#define clk_parent_select(a, b)     do{}while(0)
#define clk_set_rate(a, b)      do {(a)->clk_val = b;}while(0)
#define clk_prepare_enable(a)   do{}while(0)
//suntongce
#define clk_get_rate(a) do{}while(0)
#define clk_get_parent(a) do{a}while(0)
#define rt_mutex_trylock(a) 0
#define mb()

//print
#define pr_info     imi_printf
#define pr_err      imi_printf
#define printk      imi_printf

#define atomic_set(a, b)

#define EXPORT_SYMBOL(a)
#endif
