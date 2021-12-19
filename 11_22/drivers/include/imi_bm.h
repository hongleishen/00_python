#ifndef __IMI_BM__
#define __IMI_BM__
#include "imi_config.h"
#include "linux_compat.h"
#include "errno.h"
#include "linux_list.h"

//#define  USING_MALLOC_PHY_BUF (1)
//#define  USING_PREDEFINE_PHY_BUF	(1)
#define  USING_ARRAY_PHY_BUF	(1)


#define IMI_BUFFER_POISION_HEAD	(0x55AA00FF)
#define IMI_BUFFER_POISION_TAIL	(0xFF00AA55)
#define IMI_BUFFER_GAP_POISION	(0x5A)

#define IMI_BUFPOOL_TOTAL		(0x6000000)

//suntongce
// buffer memory pool:
//#ifdef IR RAW10
// 1280*960*4/6 * n			ir compact raw 10 data
// 1280*960*2 * n			ir raw16 data
//#ifdef DPU_RESOLUTION_480P
// 1280 * 960 * 2		depth raw buffer
// 600KB for vpu JPEG
// 300KB for PNG out
// RGB data ??
// #ifdef registration out resolution_480*640
// 480*640*6 * n
// #ifdef scaler output resolution_480*640/480*768/480*848
// 480*848*2 * n
//buffer pool manager
#define BPM_BUF_GAP						(0x2000)
#define BPM_SIZE_IR_RAW10				(IMI_BM_IR_RES_WIDTH_MAX * IMI_BM_IR_RES_HEIGHT_MAX * 4 / 3)
#define BPM_SIZE_IR_RAW10_NUM			(6)
#define BPM_SIZE_IR_RAW16				(IMI_BM_IR_RES_WIDTH_MAX * IMI_BM_IR_RES_HEIGHT_MAX * 2)
#define BPM_SIZE_IR_RAW16_NUM			(6)
#define BPM_SIZE_RGB_RAW16				(IMI_BM_RGB_RES_WIDTH_MAX * IMI_BM_RGB_RES_HEIGHT_MAX * 2)
#define BPM_SIZE_RGB_RAW16_NUM			(6)
#define BPM_SIZE_RGB_RAW8				(IMI_BM_RGB_RES_WIDTH_MAX * IMI_BM_RGB_RES_HEIGHT_MAX)
#define BPM_SIZE_RGB_RAW8_NUM			(6)
#define BPM_SIZE_DPU_RAW16				(IMI_BM_DEPTH_RES_WIDTH_MAX * IMI_BM_DEPTH_RES_HEIGHT_MAX * 2)
#define BPM_SIZE_DPU_RAW16_NUM			(6)


#define BMP_BUF_ALIGN_SZ(sz, gap)		((sz + gap -1) & (gap-1) + gap)
enum IMI_BUF_BPP {
	IMI_BUF_8BITPP = 0,
	IMI_BUF_10BITPP,		//10bits packed
	IMI_BUF_12BITPP,		//10bits packed
	IMI_BUF_16BITPP,
	IMI_BUF_BPP_MAX,
};

enum imi_bm_user {
	IMI_BUFFER_USER_IR = 0, 	//0x1
	IMI_BUFFER_USER_DEPTH,		//0x2
	IMI_BUFFER_USER_CORLOR, 	//0x4
	IMI_BUFFER_USER_VPU,		//0x8
	IMI_BUFFER_USER_PNG,		//0x10
	IMI_BUFFER_USER_G2D,		//0x20
	IMI_BUFFER_USER_HASH,		//0x40
	IMI_BUFFER_USER_USB_IN, 	//0x80 usb protocal IN direction, IE:dev->host
	IMI_BUFFER_USER_USB_OUT,	//0x100 usb protocal OUT direction, IE:host->dev
	IMI_BUFFER_USER_DSP,		//0x200
	IMI_BUFFER_USER_MAX = 32,
};

enum imi_bm_buf_type {
	IMI_BM_BUF_TYPE_DEPTH = 1,
	IMI_BM_BUF_TYPE_IR,
	IMI_BM_BUF_TYPE_IR_DOT,
	IMI_BM_BUF_TYPE_IR_BG,
	IMI_BM_BUF_TYPE_RGB,
	IMI_BM_BUF_TYPE_ALGO = 100,
	IMI_BM_BUF_TYPE_USB_IN = 200,
	IMI_BM_BUF_TYPE_USB_OUT,
	IMI_BM_BUF_TYPE_VPU_JPG = 300,
	IMI_BM_BUF_TYPE_PNG = 400,
	IMI_BM_BUF_TYPE_SCALER = 500,
	IMI_BM_BUF_TYPE_REGISTRA = 600,
	IMI_BM_BUF_TYPE_MAX = 1000,
};

#define imi_bm_buf_type_valid(type) ((type) && ((type) < IMI_BM_BUF_TYPE_MAX))
#define imi_bm_set_buf_type(node, type) 	(node->buf_type = type)
struct imi_buf_pool {
	//	struct imi_buf_pool *prev;
	//	struct imi_buf_pool *next;
	struct list_head list;
	int pool_size;
	int pool_min_size;
	struct list_head free_list;
	int freecnt;
	struct list_head busy_list;
	FRTOS_mutex lock;
};

struct imi_buf_node {
	//bm using this list , user DO NOT USE
	struct list_head list;
	//user can use usrlist to make buffer queue
	struct list_head usrlist;
	struct imi_buf_pool *ppool;
	int buf_max_size;
	int buf_addr;
	int buf_len;
	int buf_offset;
	unsigned int buf_inuse;
	enum imi_bm_buf_type buf_type;
	unsigned long long time_stamp;
};

struct imi_bufpool_cfg {
	int bm_size;
	int bm_minsz;
	int bm_cnt;
	int bm_gap;
};

int bm_buf_get(int size, struct imi_buf_node **node);
int bm_buf_addusers(struct imi_buf_node *node, enum imi_bm_user user);
int bm_buf_put(struct imi_buf_node *node, enum imi_bm_user user);
void bm_bufpool_info_dump(void);
int bm_bufpool_init(struct imi_bufpool_cfg *cfg, struct list_head *head);
struct list_head *bm_bufman_init(void);

#endif /* __IMI_BM__*/
