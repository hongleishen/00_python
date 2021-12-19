#include <stdlib.h>
#include <linux_compat.h>
#include <linux_list.h>
#include <errno.h>
#include <imi_dev.h>
#include "FreeRTOS.h"

#define HJIMI_MAX_DEV_NUM 255
static int imi_dev_index = 0;

static struct imi_device_handle *hjimi_dev_handle[HJIMI_MAX_DEV_NUM] = {
	NULL
};

int imi_register_dev(struct imi_device_handle *handle)
{
	if (imi_dev_index >= HJIMI_MAX_DEV_NUM) {
		imi_printf("register %s fail \n", handle->name);
		return -EINVAL;
	}

	imi_printf("register dev: %s\n", handle->name);

	hjimi_dev_handle[imi_dev_index] = handle;
	handle->dev->index = imi_dev_index;

	imi_dev_index++;

	return 0;
}

void imi_deregister_dev(struct imi_device_handle *handle)
{
	if (handle->dev->index >= HJIMI_MAX_DEV_NUM)
		return;

	hjimi_dev_handle[handle->dev->index] = NULL;
}

struct imi_device_handle *imi_open(const char *name)
{
	struct imi_device_handle *handle = NULL;
	int len = imi_dev_index;
	int ret, index = 0;

	for (index = 0; index < len; index++) {
		handle = hjimi_dev_handle[index];
		if (!handle)
			continue;

		//imi_printf("find: %s\n", handle->name);

		if (!strcmp(name, handle->name)) {
			if (handle->ops && handle->ops->open)
				ret = handle->ops->open(handle);

			if (ret) {
				imi_printf("open %s fail\n", handle->name);
				return NULL;
			} else
				return handle;
		}
	}

	return NULL;
}

void imi_close(struct imi_device_handle *handle)
{
	if (handle && handle->ops && handle->ops->close)
		handle->ops->close(handle);
}

int imi_ioctl(struct imi_device_handle *handle, unsigned int cmd, void *cmd_buf)
{
	if (handle && handle->ops && handle->ops->ioctl)
		return handle->ops->ioctl(handle, cmd, cmd_buf);

	return -EINVAL;
}

int imi_read(struct imi_device_handle *handle, void *buf, unsigned int len)
{
	if (handle && handle->ops && handle->ops->read)
		return handle->ops->read(handle, buf, len);

	return -EINVAL;
}

int imi_write(struct imi_device_handle *handle, void *buf, unsigned int len)
{
	if (handle && handle->ops && handle->ops->write)
		return handle->ops->write(handle, buf, len);

	return -EINVAL;
}

static int _imi_poll(struct imi_device_handle *handle, TickType_t xTicksToWait)
{
	unsigned int mask = -EINVAL;

	struct imi_wait_node *node = pvPortMalloc(sizeof(*node));

	if (node == NULL)
		return -ENOMEM;

	if (handle && handle->ops && handle->ops->poll)
		mask = handle->ops->poll(handle, node); //add wait_node to your wait_head

	if ((mask & POLLOUT) || (mask & POLLIN)) {
		vPortEnterCritical();
		list_del(&node->wait_node);
		vPortExitCritical();
	} else {
		node->wait_sem = FRTOS_sem_create(0);
		if (node->wait_sem == NULL) {
			mask = -ENOMEM;
			goto end;
		}

		FRTOS_sem_wait(node->wait_sem, xTicksToWait); //task suspend here

		vPortEnterCritical(); //resume
		list_del(&node->wait_node);
		vPortExitCritical();

		FRTOS_sem_free(node->wait_sem);
	}

end:
	vPortFree(node);

	return mask;
}

int imi_poll(struct imi_device_handle *handle, int *rd, int *wr, int *exp, TickType_t xTicksToWait)
{
	unsigned int mask;

	mask = _imi_poll(handle, xTicksToWait);

	if ((mask & POLLOUT) && wr)
		*wr = 1;
	else if ((mask & POLLIN) && rd)
		*rd = 1;
	else if ((mask & POLLPRI) && exp)
		*exp = 1;
	else {
		//nothing to do
	}

	return mask;
}

void imi_add_to_wait(struct imi_wait_head *head, struct imi_wait_node *node)
{
	vPortEnterCritical();
	list_add(&node->wait_node, &head->wait_head);
	vPortExitCritical();
}

int imi_prepare_to_wait(struct imi_wait_head *head, struct imi_wait_node *node)
{
	node->wait_sem = FRTOS_sem_create(0);
	if (node->wait_sem == NULL)
		return -ENOMEM;

	INIT_LIST_HEAD(&node->wait_node);
	imi_add_to_wait(head, node);

	return 0;
}

void imi_wait_evt(struct imi_wait_node *node,  TickType_t xTicksToWait)
{
	FRTOS_sem_wait(node->wait_sem, xTicksToWait); //task suspend here
}

void imi_finish_wait(struct imi_wait_head *head, struct imi_wait_node *node)
{
	if (node->wait_sem)
		FRTOS_sem_free(node->wait_sem);

	vPortEnterCritical(); //resume
	list_del(&node->wait_node);
	vPortExitCritical();
}

void imi_wakeup_all(struct imi_wait_head *head, enum imi_wakeup_mode mode)
{
	struct imi_wait_node *node;
	void  (*sem_signal)(FRTOS_sem_t sem);

	if (mode == IMI_WAKEUP_NORMAL)
		sem_signal = FRTOS_sem_signal;
	else
		sem_signal = FRTOS_sem_signal_from_isr;

	vPortEnterCritical(); //todo
	list_for_each_entry(node, &head->wait_head, wait_node) {
		sem_signal(node->wait_sem);
	}
	vPortExitCritical();
}

void imi_wakeup_one(struct imi_wait_head *head, enum imi_wakeup_mode mode)
{
	struct imi_wait_node *node;
	void  (*sem_signal)(FRTOS_sem_t sem);

	if (mode == IMI_WAKEUP_NORMAL)
		sem_signal = FRTOS_sem_signal;
	else
		sem_signal = FRTOS_sem_signal_from_isr;

	vPortEnterCritical();
	if (list_empty(&head->wait_head)) {
		vPortExitCritical();
		return;
	}

	node = list_first_entry(&head->wait_head, struct imi_wait_node, wait_node);
	vPortExitCritical();

	sem_signal(node->wait_sem);
}

