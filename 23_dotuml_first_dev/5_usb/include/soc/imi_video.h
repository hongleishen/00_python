#ifndef __IMI_VIDEO_H__
#define __IMI_VIDEO_H__

enum sensor_stream_type {
	ISP_SENSOR_STREAM_YUV = 0,
	VPU_SENSOR_STREAM_JPG,
	IR_SENSOR_STREAM,
	DEPTH_SENSOR_STREAM,
};

enum imi_buf_type {
	//use phy addr and memcpy
	IMI_BUF_TYPE_RAW = 0,
	IMI_BUF_TYPE_DMA_SINGLE,
};

enum imi_buffer_state {
	IMI_BUF_STATE_IDLE  = 0,
	IMI_BUF_STATE_QUEUED,
	IMI_BUF_STATE_DONE,
	IMI_BUF_STATE_ERROR,
};

enum vendor_cmd_stage {
	VENDOR_CMD_STAGE_NULL = 0,
	VENDOR_CMD_STAGE_SETUP,
	VENDOR_CMD_STAGE_DATA,
};

struct imi_video_req_buf {
	unsigned char dev_index;
	unsigned int num;
	//reserved mem for imi_packet_header
	unsigned int buf_offset;
	enum imi_buf_type type;
};

struct imi_video_buf_info {
	unsigned char dev_index;
	//index in usb video buf
	unsigned int index;
	unsigned long phy_addr;
	unsigned long long tm_stamp;
	unsigned int frame_len;
};

struct imi_video_sub_dev_ctl {
	unsigned char dev_index;
	unsigned char cmd; //0: deinit, 1:init
};

struct imi_video_echo_addr {
	unsigned long addr;
	unsigned int len;
};

struct imi_video_echo_frame_head {
	unsigned int       magic; //0xefcd494d
	unsigned int       rest_len;
	unsigned int       width; //color: RGB
	unsigned int       height;
}__attribute__((packed));

enum imi_video_ioctl_cmd {
	VIDIOC_IMI_REQ_BUF = 0x0,
	VIDIOC_IMI_QBUF,
	VIDIOC_IMI_DQBUF,
	VIDIOC_IMI_STREAMON,
	VIDIOC_IMI_STREAMOFF,
	VIDIOC_IMI_SUB_DEV_CTL,
	VIDIOC_IMI_SET_ECHO_ADDR,
	VIDIOC_IMI_GET_ECHO_FRAME_HEAD,
	VIDIOC_IMI_GET_ECHO_FRAME,
	VIDIOC_GET_DATA_STAGE,
	VIDIOC_READ_CMD_EXIT,
	VIDIOC_READ_CMD_INIT,
};

#endif /* __IMI_VIDEO_H__ */
