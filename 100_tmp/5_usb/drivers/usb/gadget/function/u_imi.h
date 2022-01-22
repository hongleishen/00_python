/*
 * Copyright (C) 2018 IMI, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */

#ifndef _U_IMI_H_
#define _U_IMI_H_
#include <errno.h>
#include <usb/linux/composite.h>
#include <usb/linux/gadget.h>
#include <linux_list.h>
#include <linux_compat.h>
#include <imi_dev.h>
#include "imi_type.h"

struct imi_video_sub_dev;

#define IMI_PACKET_MAGIC                cpu_to_le32(0x4D49CDEF)

#define VIDEO_REQ_NUM 4
#define IMI_VIDEO_MAX_ADDR_NUM 16
#if 0  //todo
#define IMI_BULK_PAYLOAD_LEN	(64 * 1024)
#else
#define IMI_BULK_PAYLOAD_LEN	(0x200)
#endif

struct imi_packet_header {
	unsigned int         magic;
	unsigned short       type;
	unsigned long long   packet_id;
	unsigned int         offset;
	unsigned int         buf_size;
	unsigned long long   time_stamp;
	unsigned short       reserved; //add short for align to 4 bytes (musb transfer)
} __packed;

enum imi_stream_type {
	IMI_STREAM_COLOR_USB = 0x00,
	IMI_STREAM_DEPTH_USB,
	IMI_STREAM_IR_USB,
	IMI_STREAM_USER_TRACK_USB,
	IMI_STREAM_FACE_DETECT_USB,
	IMI_STREAM_SLAM_USB,
	IMI_STREAM_ENCRY_USB,
	IMI_STREAM_AUDIO_LEFT_USB,
	IMI_STREAM_AUDIO_RIGHT_USB,
	IMI_STREAM_MAX,
};

//one frame
struct imi_video_buffer {
	//connected to queued or done
	struct list_head list;
	//index in imi_video_dev->vbuf
	unsigned int index;
	//len per frame
	unsigned int frame_len;
	//idle or done
	enum imi_buffer_state state;
	//buf addr in kernel
	void *mem;
	//how many bytes being transferred
	unsigned int transferred;
	//buf id from sensor buf
	unsigned int buf_id;
	//sub_dev belonged to
	struct imi_video_sub_dev *dev;
};

struct imi_video_queue {
	struct list_head queued_entry;
	struct list_head started_entry;
	struct list_head done_entry;
	spinlock_t queue_lock;
};

//usb related req
struct imi_video_req {
	struct usb_request *req[VIDEO_REQ_NUM];
	unsigned char *req_buffer[VIDEO_REQ_NUM];

	struct list_head req_free;
	struct list_head req_busy;
	spinlock_t req_lock;
};

struct imi_addr_map {
	unsigned long phy_addr;
	void *kern_addr;
	unsigned int buf_size;
};

struct imi_video_sub_dev {
	int stream_on;
	enum imi_stream_type type;
	int (*encode)(struct usb_request *req, struct imi_video_sub_dev *dev,
			struct imi_video_buffer *buf);

	struct imi_video_buffer *vbuf;
	int vbuf_num;
	enum imi_buf_type buf_type;
	unsigned int buf_offset;
};

struct imi_upload_dev {
	struct usb_ep *ep_out;
	int bulk_out_maxpacket;
	struct imi_wait_head read_wq;
	spinlock_t lock;
	int error;

	struct usb_request *rx_req;
	int rx_done;

	//for simple bulk_in
	struct usb_request *tx_req;
	FRTOS_completion_t tx_complete;
};

struct imi_video_dev {
	struct usb_ep *ep;
	int connect;

	struct imi_wait_head wait;

	unsigned int max_payload_size;
	struct imi_video_req  video_req;
	struct imi_video_queue video_queue;
	struct imi_video_sub_dev sub_dev[IMI_STREAM_MAX];
	struct imi_upload_dev udev;
};

struct imi_vendor_dev {
	struct usb_function function;
	struct usb_composite_dev *cdev;
	int online;
	struct imi_video_dev  vdev;
};

int imi_video_pump(struct imi_video_dev *video);
int imi_video_enable(struct imi_video_dev *video, int enable);

int imi_video_init(struct imi_video_dev *video);
int imi_upload_init(struct imi_video_dev *dev);
int imi_video_sub_dev_stream_on(enum imi_stream_type type);

int imi_cmd_dev_setup(struct usb_function *f, const struct usb_ctrlrequest *ctrl);
bool imi_cmd_dev_req_match(struct usb_function *f, const struct usb_ctrlrequest *ctrl, bool cfg0);
int imi_cmd_dev_init(struct usb_composite_dev *cdev);
void imi_cmd_dev_deinit(void);
#endif

