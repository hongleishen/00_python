#include "include/imi_bm.h"
#include "linux_list.h"
#include "elog.h"
#if defined(IN_SIMULATOR)
#include "console.h"

#define imi_printf		console_print
#define log_e 			console_print
#define log_d 			console_print
#define log_i 			console_print
#define log_v 			console_print
#endif

//int lock;

#if defined(USING_PREDEFINE_PHY_BUF)
#define IMI_BUFPOLL_BASE		(0x90000000+IMI_BUFPOOL_TOTAL)
char *imi_bufpool_base = (char*)IMI_BUFPOLL_BASE;
static int imi_bufpool_idx = 0;
#elif defined(USING_ARRAY_PHY_BUF)
char IMI_BUFPOLL_BASE[IMI_BUFPOOL_TOTAL];
char *imi_bufpool_base = &IMI_BUFPOLL_BASE[0];
static int imi_bufpool_idx = 0;
#endif

LIST_HEAD(ibpl);

//buffer acquired from freelist via get method & add user reference
//buffer get use reference via get method & get buffer context
//buffer put back, when no user hold reference, just give buffer back
//to manager for next buf_set
int bm_buf_get(int size, struct imi_buf_node **node)
{
	struct imi_buf_pool *ibp = 0;
	struct imi_buf_node *ibn = 0;
	int list_find = 0;
	int ret = -ENOMEM;

	spin_lock(lock);
	list_for_each_entry(ibp, &ibpl, list) {
		if ((ibp->pool_size >= size) && (ibp->pool_min_size <= size)) {
			if (ibp->freecnt) {
				list_find = 1;
				log_v("ibp %p, sz %d , pool %d %d\n", ibp,
						size, ibp->pool_size, ibp->pool_min_size);
				break;
			}
		}
	}
	if (list_find) {
		if (!list_empty(&ibp->free_list)) {
			ibn = list_first_entry(&ibp->free_list, struct imi_buf_node, list);
			if (ibn->buf_inuse) {
				configASSERT(0);
				log_e("err buf_inuse %x\n", ibn->buf_inuse);
				ret = -EFAULT;
				goto out;
			}
			ibn->buf_max_size = ibp->pool_size;
			list_move_tail(&ibn->list, &ibp->busy_list);
			if (ibp->freecnt) {
				ibp->freecnt--;
			} else {
				configASSERT(0);
				goto out;
			}
			*node = ibn;
			log_v("ibn %p, buf %x, sz %d\n", ibn, ibn->buf_addr,
					ibn->buf_max_size);
			spin_unlock(lock);
			return 0;
		} else {
			configASSERT(0);
			goto out;
		}
	}
out:
	spin_unlock(lock);
	log_e("failed\n");
	bm_bufpool_info_dump();

	return ret;
}

int bm_buf_addusers(struct imi_buf_node *node, enum imi_bm_user user)
{
	if (!node) {
		configASSERT(0);
		return -EINVAL;
	}
	log_v("node %p, cur %x user %x\n", node, node->buf_inuse,
			1<<user);
	spin_lock(lock);
	if (node->buf_inuse & (1 << user)) {
		log_e("node %x already have user %x\n", node, 1 << user);
	}
	node->buf_inuse |= 1<<user;
	spin_unlock(lock);
	return 0;
}
int bm_buf_put(struct imi_buf_node *node, enum imi_bm_user user)
{
	log_v("node %p, user %x, inuse %x\n", node, 1 << user, node->buf_inuse);
	if (!node) {
		configASSERT(0);
		return -EINVAL;
	}
	if (!(node->buf_inuse & (1 << user))) {
		log_e("user %d not get this buffer %x\n", 1 << user, node);
		return -EINVAL;
	}
	spin_lock(lock);
	node->buf_inuse &= (~(1 << user));
	if (!node->buf_inuse) {
		list_move_tail(&node->list, &(node->ppool->free_list));
		node->ppool->freecnt++;
		log_v("freecnt %d\n", node->ppool->freecnt);
	}
	spin_unlock(lock);

	return 0;
}

static const char bm_dump_usr_info[IMI_BUFFER_USER_MAX][8] = {
	"ir",
	"depth",
	"color",
	"vpu",
	"png",
	"g2d",
	"hash",
	"usbin",
	"usbout",
};
void bm_bufpool_info_dump(void)
{
	struct imi_buf_pool *ibp;
	struct imi_buf_node *ibn;
	int i;

	log_v("bm_buffer user enum:\n");

	for (i = 0; i < IMI_BUFFER_USER_MAX; i++) {
		if (bm_dump_usr_info[i][0] !='\0') {
			log_v("user %x: %s\n", 1<<i, &bm_dump_usr_info[i][0]);
		}
	}
	list_for_each_entry(ibp, &ibpl, list) {
		log_v("bufpool %x, sz %x, minsz %x, freecnt %d\n", ibp,
				ibp->pool_size, ibp->pool_min_size, ibp->freecnt);
		if (ibp->freecnt) {
			list_for_each_entry(ibn, &ibp->free_list, list) {
				log_v("node %x on freelist:\n", ibn);
				log_v("pool %x, max_sz %x add %x, len %x off %x, usr %x, ts %x\n",
						ibn->ppool, ibn->buf_max_size, ibn->buf_addr,
						ibn->buf_len, ibn->buf_offset, ibn->buf_inuse,
						ibn->time_stamp);
			}
		}
		list_for_each_entry(ibn, &ibp->busy_list, list) {
			log_v("node %x on busylist:\n", ibn);
			log_v("pool %x, max_sz %x add %x, len %x off %x, user %x, ts %x\n",
					ibn->ppool, ibn->buf_max_size, ibn->buf_addr,
					ibn->buf_len, ibn->buf_offset, ibn->buf_inuse,
					ibn->time_stamp);
		}
	}
}
int bm_bufpool_init(struct imi_bufpool_cfg *cfg, struct list_head *head)
{
	int i;
	struct imi_buf_pool *ibp;
	struct imi_buf_node *ibn;
	char *buf;
	int ret = 0;

	spin_lock(lock);

	ibp = pvPortMalloc(sizeof(struct imi_buf_pool));
	memset((void*)ibp, 0, sizeof(struct imi_buf_pool));
	//ibp->prev = ibp->next = 0;
	list_add_tail(&ibp->list, head);

	ibp->pool_size = cfg->bm_size;
	ibp->pool_min_size = cfg->bm_minsz;
	ibp->freecnt = 0;
	INIT_LIST_HEAD(&ibp->free_list);
	INIT_LIST_HEAD(&ibp->busy_list);

	for (i = 0; i < cfg->bm_cnt; i++) {
		ibn = pvPortMalloc(sizeof(struct imi_buf_node));
		memset((void*)ibn, 0, sizeof(struct imi_buf_node));
		INIT_LIST_HEAD(&ibn->list);
		INIT_LIST_HEAD(&ibn->usrlist);
		log_v("ibn %p , list: head %p , next %p, prev %p, usrlist: head %p, \
				next %p, prev %p\n", ibn, &ibn->list, ibn->list.next,
				ibn->list.prev, &ibn->usrlist, ibn->usrlist.next,
				ibn->usrlist.prev);
#if defined(USING_MALLOC_PHY_BUF)
		buf = pvPortMalloc(cfg->bm_size);
		if (buf < 0) {
			log_e("malloc buf failed %d, num %d, size %d\n", buf,
					i, cfg->bm_size);
			ret = -EFAULT;
			goto out;
		}
		memset((void*)buf, 0, cfg->bm_size);
#elif defined(USING_PREDEFINE_PHY_BUF) || defined(USING_ARRAY_PHY_BUF)
		if (imi_bufpool_idx + cfg->bm_size > IMI_BUFPOOL_TOTAL) {
			log_e("failed ,size %d , no.%d, current total %d\n", cfg->bm_size,
					i, imi_bufpool_idx);
			ret = -ENOMEM;
			goto out;
		}
		buf = imi_bufpool_base + imi_bufpool_idx;
		imi_bufpool_idx += (cfg->bm_size + cfg->bm_gap);
		memset((void*)buf, 0, cfg->bm_size);
		memset((void*)buf + cfg->bm_size, IMI_BUFFER_GAP_POISION, cfg->bm_gap);
#endif
		ibn->buf_max_size = cfg->bm_size;
		ibn->buf_addr = (int)buf;
		log_v("pool %x node %x, buf %x, sz %d\n", ibp, ibn, buf,
				cfg->bm_size);
		ibn->ppool = ibp;
		list_add_tail(&ibn->list, &ibp->free_list);
		ibp->freecnt++;
	}
out:
	spin_unlock(lock);
	return ret;
}

struct list_head *bm_bufman_init(void)
{
	struct imi_bufpool_cfg ibpc;
	int ret = 0;
#if defined(USING_PREDEFINE_PHY_BUF) || defined (USING_ARRAY_PHY_BUF)
	imi_bufpool_idx = 0;
#endif
	ibpc.bm_size = BPM_SIZE_IR_RAW16;
	ibpc.bm_minsz = ibpc.bm_size;
	ibpc.bm_cnt = BPM_SIZE_IR_RAW16_NUM;
	ibpc.bm_gap = BPM_BUF_GAP;
	ret = bm_bufpool_init(&ibpc, &ibpl);
	if (ret) {
		log_e("buffer pool: %d * %d init failed\n", ibpc.bm_size, ibpc.bm_cnt);
		return NULL;
	}

	ibpc.bm_size = BPM_SIZE_IR_RAW10;
	ibpc.bm_minsz = ibpc.bm_size;
	ibpc.bm_cnt = BPM_SIZE_IR_RAW10_NUM;
	ibpc.bm_gap = BPM_BUF_GAP;
	ret = bm_bufpool_init(&ibpc, &ibpl);
	if (ret) {
		log_e("buffer pool: %d * %d init failed\n", ibpc.bm_size, ibpc.bm_cnt);
		return NULL;
	}

	ibpc.bm_size = BPM_SIZE_RGB_RAW16;
	ibpc.bm_minsz = ibpc.bm_size;
	ibpc.bm_cnt = BPM_SIZE_RGB_RAW16_NUM;
	ibpc.bm_gap = BPM_BUF_GAP;
	ret = bm_bufpool_init(&ibpc, &ibpl);
	if (ret) {
		log_e("buffer pool: %d * %d init failed\n", ibpc.bm_size, ibpc.bm_cnt);
		return NULL;
	}

	ibpc.bm_size = BPM_SIZE_RGB_RAW8;
	ibpc.bm_minsz = ibpc.bm_size;
	ibpc.bm_cnt = BPM_SIZE_RGB_RAW8_NUM;
	ibpc.bm_gap = BPM_BUF_GAP;
	ret = bm_bufpool_init(&ibpc, &ibpl);
	if (ret) {
		log_e("buffer pool: %d * %d init failed\n", ibpc.bm_size, ibpc.bm_cnt);
		return NULL;
	}

	ibpc.bm_size = BPM_SIZE_DPU_RAW16;
	ibpc.bm_minsz = ibpc.bm_size;
	ibpc.bm_cnt = BPM_SIZE_DPU_RAW16_NUM;
	ibpc.bm_gap = BPM_BUF_GAP;
	ret = bm_bufpool_init(&ibpc, &ibpl);
	if (ret) {
		log_e("buffer pool: %d * %d init failed\n", ibpc.bm_size, ibpc.bm_cnt);
		return NULL;
	}

	return &ibpl;
}

