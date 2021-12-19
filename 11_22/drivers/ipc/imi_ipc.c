// SPDX-License-Identifier: GPL-2.0+
/*
 * (C) Copyright 2018 Huajie IMI Technology Co., Ltd.
 *
 */

#include "imi_ipc.h"
#include "errno.h"
#include "elog.h"

#if defined(IN_SIMULATOR)
#include "console.h"

#define imi_printf		console_print
#define log_e 			console_print
#define log_d 			console_print
#define log_i 			console_print
#endif

static struct imi_ipc imi_ipc_ctx;
EventGroupHandle_t imi_evt_grps = 0;

static const short module_magic[IMI_IPC_MODULE_MAX] = {
	//suntongce
	0x1122,
	0x1122,
	0x1122,
	0x1122,
};

int imi_ring_buf_init(struct imi_ring_buf *rb, short magic)
{
	struct imi_rb_desc *desc;
	int i;

	desc = &rb->desc;
	memset((void*)desc, 0, sizeof(struct imi_rb_desc));

	desc->magic  = magic;
	desc->rd_idx = 0;
	desc->wr_idx = 0;

	for (i = 0; i < IMI_IPC_MSG_NUM; i++) {
		memset((void*)&rb->msg[i], 0, sizeof(struct imi_ipc_msg));
	}
	return 0;
}

void imi_ring_buf_deinit(struct imi_ring_buf *rb)
{
}

void imi_ring_buf_reset(enum imi_ipc_module module)
{
	struct imi_ring_buf *rb = &imi_ipc_ctx.rb[module];
	struct imi_rb_desc *desc = &rb->desc;
	desc->rd_idx = 0;
	desc->wr_idx = 0;
}

int imi_ring_buf_count(enum imi_ipc_module module)
{
	struct imi_ring_buf *rb = &imi_ipc_ctx.rb[module];
	struct imi_rb_desc *desc = &rb->desc;
	int cnt;

	if (desc->rd_idx > desc->wr_idx)
		cnt = IMI_IPC_MSG_NUM - desc->rd_idx + desc->wr_idx;
	else
		cnt = desc->wr_idx - desc->rd_idx;

	return cnt;
}

int imi_ring_buf_is_empty(enum imi_ipc_module module)
{
	struct imi_ring_buf *rb = &imi_ipc_ctx.rb[module];
	struct imi_rb_desc *desc = &rb->desc;

	return desc->rd_idx == desc->wr_idx;
}

//overwrite mode do not need check buffer full
int imi_ring_buf_is_full(enum imi_ipc_module module)
{
	struct imi_ring_buf *rb = &imi_ipc_ctx.rb[module];
	struct imi_rb_desc *desc = &rb->desc;

	return (((desc->wr_idx + 1) % IMI_IPC_MSG_NUM) == desc->rd_idx);
}

int imi_ring_buf_write(enum imi_ipc_module module, struct imi_ipc_msg *data,
			int overwrite)
{
	struct imi_ring_buf *rb = &imi_ipc_ctx.rb[module];
	struct imi_rb_desc *desc = &rb->desc;
	struct imi_ipc_msg *mem;
	//we presume stream buffer is far more than cmd buffer
	if (unlikely(!overwrite) && imi_ring_buf_is_full(module))
		return -ENOMEM;
	if (likely(overwrite)) {
		mem = &rb->msg[desc->wr_idx % IMI_IPC_MSG_NUM];
		memcpy(mem, data, sizeof(struct imi_ipc_msg));
		mb();
		desc->wr_idx++;
	} else {
		mem = &rb->msg[desc->wr_idx];
		memcpy(mem, data, sizeof(struct imi_ipc_msg));
		mb();
		desc->wr_idx = (desc->wr_idx + 1) % IMI_IPC_MSG_NUM;
	}
#if defined(IN_SIMULATOR)
	just_delay(10); 	//simulator ensure date in
#endif

	return 0;
}

int imi_ring_buf_read(enum imi_ipc_module module, struct imi_ipc_msg *data,
			int overwrite)
{
	struct imi_ring_buf *rb = &imi_ipc_ctx.rb[module];
	struct imi_rb_desc *desc = &rb->desc;
	struct imi_ipc_msg *mem;
	if (imi_ring_buf_is_empty(module)) {
		log_i("%s buffer empty\n", __func__);
		return -ENOBUFS;
	}
	if (likely(overwrite)) {
		// write overrun
		if (unlikely(desc->wr_idx >= (desc->rd_idx + IMI_IPC_MSG_NUM))) {
			desc->rd_idx = desc->wr_idx - IMI_IPC_MSG_NUM + 1;
		}
		mem = &rb->msg[desc->rd_idx % IMI_IPC_MSG_NUM];
		memcpy(data, mem, sizeof(struct imi_ipc_msg));
		mb();
		desc->rd_idx++;
	} else {
		mem = &rb->msg[desc->rd_idx];
		memcpy(data, mem, sizeof(struct imi_ipc_msg));
		mb();
		desc->rd_idx = (desc->rd_idx + 1) % IMI_IPC_MSG_NUM;
	}

	return 0;
}

void imi_ipc_init(void)
{
	int i;
	imi_evt_grps = xEventGroupCreate();

	for (i = 0; i < IMI_IPC_MODULE_MAX; i++) {
		imi_ring_buf_init(&imi_ipc_ctx.rb[i], module_magic[i]);
	}
}

void imi_ipc_dump_msg(struct imi_ipc_msg *msg)
{
//suntongce need complete
}
