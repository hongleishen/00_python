#ifndef __IMI_DEV__
#define __IMI_DEV__
#include <linux_list.h>
#include <linux_compat.h>
#include <portable.h>

struct imi_device;
struct imi_device_handle;

enum imi_wakeup_mode {
	IMI_WAKEUP_NORMAL = 0x0,
	IMI_WAKEUP_FROM_ISR,
};

struct imi_wait_node {
	struct list_head wait_node;
	FRTOS_sem_t wait_sem;
};

struct imi_wait_head {
	struct list_head wait_head;
};

struct imi_device {
		unsigned int io_mem; //mem base
		void *device_data;
		void *priv;
		int index;
};

struct imi_device_ops {
	int (*open)(struct imi_device_handle *handle);
	int (*close)(struct imi_device_handle *handle);
	int (*read)(struct imi_device_handle *handle, void *buf, unsigned int len);
	int (*write)(struct imi_device_handle *handle, void *buf, unsigned int len);
	unsigned int (*poll)(struct imi_device_handle *handle, struct imi_wait_node *node);
	int (*ioctl)(struct imi_device_handle *handle, unsigned int cmd, void *cmd_buf);
};

struct imi_device_handle {
	const char *name;
	struct imi_device_ops *ops;  //for driver
	struct imi_device *dev; //which device
};

int imi_register_dev(struct imi_device_handle *handle);
void imi_deregister_dev(struct imi_device_handle *handle);

struct imi_device_handle *imi_open(const char *name);
void imi_close(struct imi_device_handle *handle);
int imi_ioctl(struct imi_device_handle *handle, unsigned int cmd, void *cmd_buf);
int imi_read(struct imi_device_handle *handle, void *buf, unsigned int len);
int imi_write(struct imi_device_handle *handle, void *buf, unsigned int len);
int imi_poll(struct imi_device_handle *handle, int *rd, int *wr, int *exp, TickType_t xTicksToWait);
void imi_add_to_wait(struct imi_wait_head *head, struct imi_wait_node *node);
void imi_wakeup_all(struct imi_wait_head *head, enum imi_wakeup_mode mode);
void imi_wakeup_one(struct imi_wait_head *head, enum imi_wakeup_mode mode);
int imi_prepare_to_wait(struct imi_wait_head *head, struct imi_wait_node *node);
void imi_wait_evt(struct imi_wait_node *node,  TickType_t xTicksToWait);
void imi_finish_wait(struct imi_wait_head *head, struct imi_wait_node *node);

//=======================================wait func==============================//
#define _wait_event_timeout(wq_head, condition, ret, timeout)		\
({	__label__ __out;									\
	struct imi_wait_node __wq_entry;					\
	long __ret = ret;	/* explicit shadow */				\
										\
	__ret = imi_prepare_to_wait(&(wq_head), &__wq_entry);	\
	if (__ret)  goto __out; \
	for (;;) {								\
		imi_wait_evt(&__wq_entry, timeout);					\
		if (condition)							\
			break;							\
	}									\
	imi_finish_wait(&(wq_head), &__wq_entry);					\
__out:	__ret;									\
})

#define wait_event_timeout(wq_head, condition, timeout)				\
({										\
	int __ret = 0;								\
	if (!(condition))							\
		__ret = _wait_event_timeout(wq_head, (condition), __ret, timeout);		\
	__ret;									\
})

#define wait_event(wq_head, condition)  wait_event_timeout(wq_head, condition, 0)

#endif
