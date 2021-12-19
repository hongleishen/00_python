/* SPDX-License-Identifier: GPL-2.0 */
/*
 * The contents of this file are private to DMA engine drivers, and is not
 * part of the API to be used by DMA engine users.
 */
#ifndef DMAENGINE_H
#define DMAENGINE_H
#include "imi_type.h"
#include <string.h>
#include "linux_compat.h"
#include "linux_list.h"
#include "errno.h"

/**
 * typedef dma_cookie_t - an opaque DMA cookie
 *
 * if dma_cookie_t is >0 it's a DMA request cookie, <0 it's an error code
 */
typedef signed int dma_cookie_t;
#define DMA_MIN_COOKIE  1

static inline int dma_submit_error(dma_cookie_t cookie)
{
	return cookie < 0 ? cookie : 0;
}

/**
 * enum dma_status - DMA transaction status
 * @DMA_COMPLETE: transaction completed
 * @DMA_IN_PROGRESS: transaction not yet processed
 * @DMA_PAUSED: transaction is paused
 * @DMA_ERROR: transaction failed
 */
enum dma_status {
	DMA_COMPLETE,
	DMA_IN_PROGRESS,
	DMA_PAUSED,
	DMA_ERROR,
};

/**
 * enum dma_transaction_type - DMA transaction types/indexes
 *
 * Note: The DMA_ASYNC_TX capability is not to be set by drivers.  It is
 * automatically set as dma devices are registered.
 */
enum dma_transaction_type {
	DMA_MEMCPY,
	DMA_MEMSET,
	DMA_PRIVATE,
	DMA_ASYNC_TX,
	DMA_SLAVE,
	DMA_CYCLIC,
/* last transaction type for creation of the capabilities mask */
	DMA_TX_TYPE_END,
};

/**
 * enum dma_transfer_direction - dma transfer mode and direction indicator
 * @DMA_MEM_TO_MEM: Async/Memcpy mode
 * @DMA_MEM_TO_DEV: Slave mode & From Memory to Device
 * @DMA_DEV_TO_MEM: Slave mode & From Device to Memory
 * @DMA_DEV_TO_DEV: Slave mode & From Device to Device
 */
enum dma_transfer_direction {
	DMA_MEM_TO_MEM,
	DMA_MEM_TO_DEV,
	DMA_DEV_TO_MEM,
	DMA_DEV_TO_DEV,
	DMA_TRANS_NONE,
};

/**
 * enum dma_ctrl_flags - DMA flags to augment operation preparation,
 *  control completion, and communicate status.
 * @DMA_PREP_INTERRUPT - trigger an interrupt (callback) upon completion of
 *  this transaction
 * @DMA_CTRL_ACK - if clear, the descriptor cannot be reused until the client
 *  acknowledges receipt, i.e. has has a chance to establish any dependency
 *  chains
 * @DMA_PREP_PQ_DISABLE_P - prevent generation of P while generating Q
 * @DMA_PREP_PQ_DISABLE_Q - prevent generation of Q while generating P
 * @DMA_PREP_CONTINUE - indicate to a driver that it is reusing buffers as
 *  sources that were the result of a previous operation, in the case of a PQ
 *  operation it continues the calculation with new sources
 * @DMA_PREP_FENCE - tell the driver that subsequent operations depend
 *  on the result of this operation
 * @DMA_CTRL_REUSE: client can reuse the descriptor and submit again till
 *  cleared or freed
 * @DMA_PREP_CMD: tell the driver that the data passed to DMA API is command
 *  data and the descriptor should be in different format from normal
 *  data descriptors.
 */
enum dma_ctrl_flags {
	DMA_PREP_INTERRUPT = (1 << 0),
	DMA_CTRL_ACK = (1 << 1),
	DMA_PREP_CONTINUE = (1 << 2),
	DMA_PREP_FENCE = (1 << 3),
	DMA_CTRL_REUSE = (1 << 4),
	DMA_PREP_CMD = (1 << 5),
};

/**
 * struct dma_chan - devices supply DMA channels, clients use them
 * @device: ptr to the dma device who supplies this channel, always !%NULL
 * @cookie: last cookie value returned to client
 * @completed_cookie: last completed cookie for this channel
 * @chan_id: channel ID for sysfs
 * @dev: class device for sysfs
 * @device_node: used to add this to the device chan list
 * @local: per-cpu pointer to a struct dma_chan_percpu
 * @client_count: how many clients are using this channel
 * @table_count: number of appearances in the mem-to-mem allocation table
 * @router: pointer to the DMA router structure
 * @route_data: channel specific data for the router
 * @private: private data for certain client-channel associations
 */
struct dma_chan {
	struct dma_device *device;
	dma_cookie_t cookie;
	dma_cookie_t completed_cookie;

	/* sysfs */
	int chan_id;
	const char *chan_name;
	struct dma_chan_dev *dev;

	struct list_head device_node;
	int client_count;

	void *private;
};

/**
 * struct dma_chan_dev - relate sysfs device node to backing channel device
 * @chan: driver channel device
 * @device: sysfs device
 * @dev_id: parent dma_device dev_id
 * @idr_ref: reference count to gate release of dma_device dev_id
 */
struct dma_chan_dev {
	struct dma_chan *chan;
	struct device device;
	int dev_id;
};

/**
 * enum dma_slave_buswidth - defines bus width of the DMA slave
 * device, source or target buses
 */
enum dma_slave_buswidth {
	DMA_SLAVE_BUSWIDTH_UNDEFINED = 0,
	DMA_SLAVE_BUSWIDTH_1_BYTE = 1,
	DMA_SLAVE_BUSWIDTH_2_BYTES = 2,
	DMA_SLAVE_BUSWIDTH_3_BYTES = 3,
	DMA_SLAVE_BUSWIDTH_4_BYTES = 4,
	DMA_SLAVE_BUSWIDTH_8_BYTES = 8,
	DMA_SLAVE_BUSWIDTH_16_BYTES = 16,
	DMA_SLAVE_BUSWIDTH_32_BYTES = 32,
	DMA_SLAVE_BUSWIDTH_64_BYTES = 64,
};

/**
 * struct dma_slave_config - dma slave channel runtime config
 * @direction: whether the data shall go in or out on this slave
 * channel, right now. DMA_MEM_TO_DEV and DMA_DEV_TO_MEM are
 * legal values. DEPRECATED, drivers should use the direction argument
 * to the device_prep_slave_sg and device_prep_dma_cyclic functions or
 * the dir field in the dma_interleaved_template structure.
 * @src_addr: this is the physical address where DMA slave data
 * should be read (RX), if the source is memory this argument is
 * ignored.
 * @dst_addr: this is the physical address where DMA slave data
 * should be written (TX), if the source is memory this argument
 * is ignored.
 * @src_addr_width: this is the width in bytes of the source (RX)
 * register where DMA data shall be read. If the source
 * is memory this may be ignored depending on architecture.
 * Legal values: 1, 2, 4, 8.
 * @dst_addr_width: same as src_addr_width but for destination
 * target (TX) mutatis mutandis.
 * @src_maxburst: the maximum number of words (note: words, as in
 * units of the src_addr_width member, not bytes) that can be sent
 * in one burst to the device. Typically something like half the
 * FIFO depth on I/O peripherals so you don't overflow it. This
 * may or may not be applicable on memory sources.
 * @dst_maxburst: same as src_maxburst but for destination target
 * mutatis mutandis.
 * @src_port_window_size: The length of the register area in words the data need
 * to be accessed on the device side. It is only used for devices which is using
 * an area instead of a single register to receive the data. Typically the DMA
 * loops in this area in order to transfer the data.
 * @dst_port_window_size: same as src_port_window_size but for the destination
 * port.
 * @device_fc: Flow Controller Settings. Only valid for slave channels. Fill
 * with 'true' if peripheral should be flow controller. Direction will be
 * selected at Runtime.
 * @slave_id: Slave requester id. Only valid for slave channels. The dma
 * slave peripheral will have unique id as dma requester which need to be
 * pass as slave config.
 *
 * This struct is passed in as configuration data to a DMA engine
 * in order to set up a certain channel for DMA transport at runtime.
 * The DMA device/engine has to provide support for an additional
 * callback in the dma_device structure, device_config and this struct
 * will then be passed in as an argument to the function.
 *
 * The rationale for adding configuration information to this struct is as
 * follows: if it is likely that more than one DMA slave controllers in
 * the world will support the configuration option, then make it generic.
 * If not: if it is fixed so that it be sent in static from the platform
 * data, then prefer to do that.
 */
struct dma_slave_config {
	enum dma_transfer_direction direction;
	unsigned int src_addr;
	unsigned int dst_addr;
	enum dma_slave_buswidth src_addr_width;
	enum dma_slave_buswidth dst_addr_width;
	u32 src_maxburst;
	u32 dst_maxburst;
	u32 src_port_window_size;
	u32 dst_port_window_size;
	bool device_fc;
	unsigned int slave_id;
};

/**
 * enum dma_residue_granularity - Granularity of the reported transfer residue
 * @DMA_RESIDUE_GRANULARITY_DESCRIPTOR: Residue reporting is not support. The
 *  DMA channel is only able to tell whether a descriptor has been completed or
 *  not, which means residue reporting is not supported by this channel. The
 *  residue field of the dma_tx_state field will always be 0.
 * @DMA_RESIDUE_GRANULARITY_SEGMENT: Residue is updated after each successfully
 *  completed segment of the transfer (For cyclic transfers this is after each
 *  period). This is typically implemented by having the hardware generate an
 *  interrupt after each transferred segment and then the drivers updates the
 *  outstanding residue by the size of the segment. Another possibility is if
 *  the hardware supports scatter-gather and the segment descriptor has a field
 *  which gets set after the segment has been completed. The driver then counts
 *  the number of segments without the flag set to compute the residue.
 * @DMA_RESIDUE_GRANULARITY_BURST: Residue is updated after each transferred
 *  burst. This is typically only supported if the hardware has a progress
 *  register of some sort (E.g. a register with the current read/write address
 *  or a register with the amount of bursts/beats/bytes that have been
 *  transferred or still need to be transferred).
 */
enum dma_residue_granularity {
	DMA_RESIDUE_GRANULARITY_DESCRIPTOR = 0,
	DMA_RESIDUE_GRANULARITY_SEGMENT = 1,
	DMA_RESIDUE_GRANULARITY_BURST = 2,
};

/* struct dma_slave_caps - expose capabilities of a slave channel only
 *
 * @src_addr_widths: bit mask of src addr widths the channel supports
 * @dst_addr_widths: bit mask of dstn addr widths the channel supports
 * @directions: bit mask of slave direction the channel supported
 *  since the enum dma_transfer_direction is not defined as bits for each
 *  type of direction, the dma controller should fill (1 << <TYPE>) and same
 *  should be checked by controller as well
 * @max_burst: max burst capability per-transfer
 * @cmd_pause: true, if pause and thereby resume is supported
 * @cmd_terminate: true, if terminate cmd is supported
 * @residue_granularity: granularity of the reported transfer residue
 * @descriptor_reuse: if a descriptor can be reused by client and
 * resubmitted multiple times
 */
struct dma_slave_caps {
	u32 src_addr_widths;
	u32 dst_addr_widths;
	u32 directions;
	u32 max_burst;
	bool cmd_pause;
	bool cmd_terminate;
	enum dma_residue_granularity residue_granularity;
	bool descriptor_reuse;
};

static inline const char *dma_chan_name(struct dma_chan *chan)
{
	return chan->chan_name;
}

typedef void (*dma_async_tx_callback)(void *param);

enum dmaengine_tx_result {
	DMA_TRANS_NOERROR = 0,      /* SUCCESS */
	DMA_TRANS_READ_FAILED,      /* Source DMA read failed */
	DMA_TRANS_WRITE_FAILED,     /* Destination DMA write failed */
	DMA_TRANS_ABORTED,      /* Op never submitted / aborted */
};

struct dmaengine_result {
	enum dmaengine_tx_result result;
	u32 residue;
};

typedef void (*dma_async_tx_callback_result)(void *param,
				const struct dmaengine_result *result);

/**
 * struct dma_async_tx_descriptor - async transaction descriptor
 * ---dma generic offload fields---
 * @cookie: tracking cookie for this transaction, set to -EBUSY if
 *  this tx is sitting on a dependency list
 * @flags: flags to augment operation preparation, control completion, and
 *  communicate status
 * @phys: physical address of the descriptor
 * @chan: target channel for this operation
 * @tx_submit: accept the descriptor, assign ordered cookie and mark the
 * descriptor pending. To be pushed on .issue_pending() call
 * @callback: routine to call after this operation is complete
 * @callback_param: general parameter to pass to the callback routine
 * ---async_tx api specific fields---
 * @next: at completion submit this descriptor
 * @parent: pointer to the next level up in the dependency chain
 * @lock: protect the parent and next pointers
 */
struct dma_async_tx_descriptor {
	dma_cookie_t cookie;
	enum dma_ctrl_flags flags; /* not a 'long' to pack with cookie */
	dma_addr_t phys;
	struct dma_chan *chan;
	dma_cookie_t (*tx_submit)(struct dma_async_tx_descriptor *tx);
	int (*desc_free)(struct dma_async_tx_descriptor *tx);
	dma_async_tx_callback callback;
	dma_async_tx_callback_result callback_result;
	void *callback_param;
};

/**
 * struct dma_tx_state - filled in to report the status of
 * a transfer.
 * @last: last completed DMA cookie
 * @used: last issued DMA cookie (i.e. the one in progress)
 * @residue: the remaining number of bytes left to transmit
 *  on the selected transfer for states DMA_IN_PROGRESS and
 *  DMA_PAUSED if this is implemented in the driver, else 0
 */
struct dma_tx_state {
	dma_cookie_t last;
	dma_cookie_t used;
	u32 residue;
};

/**
 * enum dmaengine_alignment - defines alignment of the DMA async tx
 * buffers
 */
enum dmaengine_alignment {
	DMAENGINE_ALIGN_1_BYTE = 0,
	DMAENGINE_ALIGN_2_BYTES = 1,
	DMAENGINE_ALIGN_4_BYTES = 2,
	DMAENGINE_ALIGN_8_BYTES = 3,
	DMAENGINE_ALIGN_16_BYTES = 4,
	DMAENGINE_ALIGN_32_BYTES = 5,
	DMAENGINE_ALIGN_64_BYTES = 6,
};

struct dma_tr_param {
	unsigned int addr;
	unsigned int len;
};

/**
 * struct dma_device - info on the entity supplying DMA services
 * @chancnt: how many DMA channels are supported
 * @privatecnt: how many DMA channels are requested by dma_request_channel
 * @channels: the list of struct dma_chan
 * @global_node: list_head for global dma_device_list
 * @filter: information for device/slave to filter function/param mapping
 * @cap_mask: one or more dma_capability flags
 * @max_xor: maximum number of xor sources, 0 if no capability
 * @max_pq: maximum number of PQ sources and PQ-continue capability
 * @copy_align: alignment shift for memcpy operations
 * @xor_align: alignment shift for xor operations
 * @pq_align: alignment shift for pq operations
 * @fill_align: alignment shift for memset operations
 * @dev_id: unique device ID
 * @dev: struct device reference for dma mapping api
 * @src_addr_widths: bit mask of src addr widths the device supports
 * @dst_addr_widths: bit mask of dst addr widths the device supports
 * @directions: bit mask of slave direction the device supports since
 *  the enum dma_transfer_direction is not defined as bits for
 *  each type of direction, the dma controller should fill (1 <<
 *  <TYPE>) and same should be checked by controller as well
 * @max_burst: max burst capability per-transfer
 * @residue_granularity: granularity of the transfer residue reported
 *  by tx_status
 * @device_alloc_chan_resources: allocate resources and return the
 *  number of allocated descriptors
 * @device_free_chan_resources: release DMA channel's resources
 * @device_prep_dma_memcpy: prepares a memcpy operation
 * @device_prep_dma_xor: prepares a xor operation
 * @device_prep_dma_xor_val: prepares a xor validation operation
 * @device_prep_dma_pq: prepares a pq operation
 * @device_prep_dma_pq_val: prepares a pqzero_sum operation
 * @device_prep_dma_memset: prepares a memset operation
 * @device_prep_dma_memset_sg: prepares a memset operation over a scatter list
 * @device_prep_dma_interrupt: prepares an end of chain interrupt operation
 * @device_prep_slave_sg: prepares a slave dma operation
 * @device_prep_dma_cyclic: prepare a cyclic dma operation suitable for audio.
 *  The function takes a buffer of size buf_len. The callback function will
 *  be called after period_len bytes have been transferred.
 * @device_prep_interleaved_dma: Transfer expression in a generic way.
 * @device_prep_dma_imm_data: DMA's 8 byte immediate data to the dst address
 * @device_config: Pushes a new configuration to a channel, return 0 or an error
 *  code
 * @device_pause: Pauses any transfer happening on a channel. Returns
 *  0 or an error code
 * @device_resume: Resumes any transfer on a channel previously
 *  paused. Returns 0 or an error code
 * @device_terminate_all: Aborts all transfers on a channel. Returns 0
 *  or an error code
 * @device_synchronize: Synchronizes the termination of a transfers to the
 *  current context.
 * @device_tx_status: poll for transaction completion, the optional
 *  txstate parameter can be supplied with a pointer to get a
 *  struct with auxiliary transfer status information, otherwise the call
 *  will just return a simple status code
 * @device_issue_pending: push pending transactions to hardware
 * @descriptor_reuse: a submitted transfer can be resubmitted after completion
 */
struct dma_device {
	unsigned int chancnt;
	unsigned int privatecnt;
	struct list_head channels;
	enum dmaengine_alignment copy_align;
	enum dmaengine_alignment fill_align;

	int dev_id;
	struct device *dev;

	u32 src_addr_widths;
	u32 dst_addr_widths;
	u32 directions;
	u32 max_burst;
	bool descriptor_reuse;
	enum dma_residue_granularity residue_granularity;

	int (*device_alloc_chan_resources)(struct dma_chan *chan);
	void (*device_free_chan_resources)(struct dma_chan *chan);

	struct dma_async_tx_descriptor *(*device_prep_dma_memcpy)(
		struct dma_chan *chan, dma_addr_t dst, dma_addr_t src,
		size_t len, unsigned long flags);
	struct dma_async_tx_descriptor *(*device_prep_slave_sg)(
		struct dma_chan *chan, struct dma_tr_param *sgl,
		unsigned int sg_len, enum dma_transfer_direction direction,
		unsigned long flags, void *context);
	struct dma_async_tx_descriptor *(*device_prep_dma_cyclic)(
		struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction direction,
		unsigned long flags);

	int (*device_config)(struct dma_chan *chan,
				 struct dma_slave_config *config);
	int (*device_pause)(struct dma_chan *chan);
	int (*device_resume)(struct dma_chan *chan);
	int (*device_terminate_all)(struct dma_chan *chan);
	void (*device_synchronize)(struct dma_chan *chan);

	enum dma_status (*device_tx_status)(struct dma_chan *chan,
						dma_cookie_t cookie,
						struct dma_tx_state *txstate);
	void (*device_issue_pending)(struct dma_chan *chan);
};

static inline int dmaengine_slave_config(struct dma_chan *chan,
					  struct dma_slave_config *config)
{
	if (chan->device->device_config)
		return chan->device->device_config(chan, config);

	return -ENOSYS;
}

static inline bool is_slave_direction(enum dma_transfer_direction direction)
{
	return (direction == DMA_MEM_TO_DEV) || (direction == DMA_DEV_TO_MEM);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_slave_single(
	struct dma_chan *chan, dma_addr_t buf, size_t len,
	enum dma_transfer_direction dir, unsigned long flags)
{
	struct dma_tr_param sg[2];

	sg[0].addr = buf;
	sg[0].len = len;

	sg[1].addr = 0;
	sg[1].len = 0;

	if (!chan || !chan->device || !chan->device->device_prep_slave_sg)
		return NULL;

	return chan->device->device_prep_slave_sg(chan, &sg[0], 1,
						  dir, flags, NULL);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_slave_sg(
	struct dma_chan *chan, struct dma_tr_param *sgl,    unsigned int sg_len,
	enum dma_transfer_direction dir, unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_slave_sg)
		return NULL;

	return chan->device->device_prep_slave_sg(chan, sgl, sg_len,
						  dir, flags, NULL);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_dma_cyclic(
		struct dma_chan *chan, dma_addr_t buf_addr, size_t buf_len,
		size_t period_len, enum dma_transfer_direction dir,
		unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_dma_cyclic)
		return NULL;

	return chan->device->device_prep_dma_cyclic(chan, buf_addr, buf_len,
						period_len, dir, flags);
}

static inline struct dma_async_tx_descriptor *dmaengine_prep_dma_memcpy(
		struct dma_chan *chan, dma_addr_t dest, dma_addr_t src,
		size_t len, unsigned long flags)
{
	if (!chan || !chan->device || !chan->device->device_prep_dma_memcpy)
		return NULL;

	return chan->device->device_prep_dma_memcpy(chan, dest, src,
							len, flags);
}

/**
 * dmaengine_terminate_all() - Terminate all active DMA transfers
 * @chan: The channel for which to terminate the transfers
 *
 * This function is DEPRECATED use either dmaengine_terminate_sync() or
 * dmaengine_terminate_async() instead.
 */
static inline int dmaengine_terminate_all(struct dma_chan *chan)
{
	if (chan->device->device_terminate_all)
		return chan->device->device_terminate_all(chan);

	return -ENOSYS;
}

/**
 * dmaengine_terminate_async() - Terminate all active DMA transfers
 * @chan: The channel for which to terminate the transfers
 *
 * Calling this function will terminate all active and pending descriptors
 * that have previously been submitted to the channel. It is not guaranteed
 * though that the transfer for the active descriptor has stopped when the
 * function returns. Furthermore it is possible the complete callback of a
 * submitted transfer is still running when this function returns.
 *
 * dmaengine_synchronize() needs to be called before it is safe to free
 * any memory that is accessed by previously submitted descriptors or before
 * freeing any resources accessed from within the completion callback of any
 * perviously submitted descriptors.
 *
 * This function can be called from atomic context as well as from within a
 * complete callback of a descriptor submitted on the same channel.
 *
 * If none of the two conditions above apply consider using
 * dmaengine_terminate_sync() instead.
 */
static inline int dmaengine_terminate_async(struct dma_chan *chan)
{
	if (chan->device->device_terminate_all)
		return chan->device->device_terminate_all(chan);

	return -EINVAL;
}

/**
 * dmaengine_synchronize() - Synchronize DMA channel termination
 * @chan: The channel to synchronize
 *
 * Synchronizes to the DMA channel termination to the current context. When this
 * function returns it is guaranteed that all transfers for previously issued
 * descriptors have stopped and and it is safe to free the memory assoicated
 * with them. Furthermore it is guaranteed that all complete callback functions
 * for a previously submitted descriptor have finished running and it is safe to
 * free resources accessed from within the complete callbacks.
 *
 * The behavior of this function is undefined if dma_async_issue_pending() has
 * been called between dmaengine_terminate_async() and this function.
 *
 * This function must only be called from non-atomic context and must not be
 * called from within a complete callback of a descriptor submitted on the same
 * channel.
 */
static inline void dmaengine_synchronize(struct dma_chan *chan)
{

	if (chan->device->device_synchronize)
		chan->device->device_synchronize(chan);
}

/**
 * dmaengine_terminate_sync() - Terminate all active DMA transfers
 * @chan: The channel for which to terminate the transfers
 *
 * Calling this function will terminate all active and pending transfers
 * that have previously been submitted to the channel. It is similar to
 * dmaengine_terminate_async() but guarantees that the DMA transfer has actually
 * stopped and that all complete callbacks have finished running when the
 * function returns.
 *
 * This function must only be called from non-atomic context and must not be
 * called from within a complete callback of a descriptor submitted on the same
 * channel.
 */
static inline int dmaengine_terminate_sync(struct dma_chan *chan)
{
	int ret;

	ret = dmaengine_terminate_async(chan);
	if (ret)
		return ret;

	dmaengine_synchronize(chan);

	return 0;
}

static inline int dmaengine_pause(struct dma_chan *chan)
{
	if (chan->device->device_pause)
		return chan->device->device_pause(chan);

	return -ENOSYS;
}

static inline int dmaengine_resume(struct dma_chan *chan)
{
	if (chan->device->device_resume)
		return chan->device->device_resume(chan);

	return -ENOSYS;
}

static inline enum dma_status dmaengine_tx_status(struct dma_chan *chan,
	dma_cookie_t cookie, struct dma_tx_state *state)
{
	return chan->device->device_tx_status(chan, cookie, state);
}

static inline dma_cookie_t dmaengine_submit(struct dma_async_tx_descriptor *desc)
{
	return desc->tx_submit(desc);
}

static inline bool dmaengine_check_align(enum dmaengine_alignment align,
					 size_t off1, size_t off2, size_t len)
{
	size_t mask;

	if (!align)
		return true;
	mask = (1 << align) - 1;
	if (mask & (off1 | off2 | len))
		return false;
	return true;
}

static inline bool is_dma_copy_aligned(struct dma_device *dev, size_t off1,
					   size_t off2, size_t len)
{
	return dmaengine_check_align(dev->copy_align, off1, off2, len);
}

static inline bool is_dma_fill_aligned(struct dma_device *dev, size_t off1,
					   size_t off2, size_t len)
{
	return dmaengine_check_align(dev->fill_align, off1, off2, len);
}

static inline bool dmaf_continue(enum dma_ctrl_flags flags)
{
	return (flags & DMA_PREP_CONTINUE) == DMA_PREP_CONTINUE;
}

void dma_async_tx_descriptor_init(struct dma_async_tx_descriptor *tx,
				  struct dma_chan *chan);

static inline void async_tx_ack(struct dma_async_tx_descriptor *tx)
{
	tx->flags |= DMA_CTRL_ACK;
}

static inline void async_tx_clear_ack(struct dma_async_tx_descriptor *tx)
{
	tx->flags &= ~DMA_CTRL_ACK;
}

static inline bool async_tx_test_ack(struct dma_async_tx_descriptor *tx)
{
	return (tx->flags & DMA_CTRL_ACK) == DMA_CTRL_ACK;
}

/**
 * dma_async_issue_pending - flush pending transactions to HW
 * @chan: target DMA channel
 *
 * This allows drivers to push copies to HW in batches,
 * reducing MMIO writes where possible.
 */
static inline void dma_async_issue_pending(struct dma_chan *chan)
{
	chan->device->device_issue_pending(chan);
}

/**
 * dma_async_is_tx_complete - poll for transaction completion
 * @chan: DMA channel
 * @cookie: transaction identifier to check status of
 * @last: returns last completed cookie, can be NULL
 * @used: returns last issued cookie, can be NULL
 *
 * If @last and @used are passed in, upon return they reflect the driver
 * internal state and can be used with dma_async_is_complete() to check
 * the status of multiple cookies without re-checking hardware state.
 */
static inline enum dma_status dma_async_is_tx_complete(struct dma_chan *chan,
	dma_cookie_t cookie, dma_cookie_t *last, dma_cookie_t *used)
{
	struct dma_tx_state state;
	enum dma_status status;

	status = chan->device->device_tx_status(chan, cookie, &state);
	if (last)
		*last = state.last;
	if (used)
		*used = state.used;
	return status;
}

/**
 * dma_async_is_complete - test a cookie against chan state
 * @cookie: transaction identifier to test status of
 * @last_complete: last know completed transaction
 * @last_used: last cookie value handed out
 *
 * dma_async_is_complete() is used in dma_async_is_tx_complete()
 * the test logic is separated for lightweight testing of multiple cookies
 */
static inline enum dma_status dma_async_is_complete(dma_cookie_t cookie,
			dma_cookie_t last_complete, dma_cookie_t last_used)
{
	if (last_complete <= last_used) {
		if ((cookie <= last_complete) || (cookie > last_used))
			return DMA_COMPLETE;
	} else {
		if ((cookie <= last_complete) && (cookie > last_used))
			return DMA_COMPLETE;
	}
	return DMA_IN_PROGRESS;
}

static inline void
dma_set_tx_state(struct dma_tx_state *st, dma_cookie_t last, dma_cookie_t used, u32 residue)
{
	if (st) {
		st->last = last;
		st->used = used;
		st->residue = residue;
	}
}

enum dma_status dma_sync_wait(struct dma_chan *chan, dma_cookie_t cookie);
enum dma_status dma_wait_for_async_tx(struct dma_async_tx_descriptor *tx);
void dma_issue_pending_all(void);

struct dma_chan *dma_request_chan(struct device *dev, const char *name);
struct dma_chan *dma_get_slave_channel(struct dma_chan *chan);
void dma_release_channel(struct dma_chan *chan);
int dma_get_slave_caps(struct dma_chan *chan, struct dma_slave_caps *caps);

/* --- DMA device --- */
int dma_async_device_register(struct dma_device *device);

/**
 * dma_cookie_init - initialize the cookies for a DMA channel
 * @chan: dma channel to initialize
 */
static inline void dma_cookie_init(struct dma_chan *chan)
{
	chan->cookie = DMA_MIN_COOKIE;
	chan->completed_cookie = DMA_MIN_COOKIE;
}

/**
 * dma_cookie_assign - assign a DMA engine cookie to the descriptor
 * @tx: descriptor needing cookie
 *
 * Assign a unique non-zero per-channel cookie to the descriptor.
 * Note: caller is expected to hold a lock to prevent concurrency.
 */
static inline dma_cookie_t dma_cookie_assign(struct dma_async_tx_descriptor *tx)
{
	struct dma_chan *chan = tx->chan;
	dma_cookie_t cookie;

	cookie = chan->cookie + 1;
	if (cookie < DMA_MIN_COOKIE)
		cookie = DMA_MIN_COOKIE;
	tx->cookie = chan->cookie = cookie;

	return cookie;
}

/**
 * dma_cookie_complete - complete a descriptor
 * @tx: descriptor to complete
 *
 * Mark this descriptor complete by updating the channels completed
 * cookie marker.  Zero the descriptors cookie to prevent accidental
 * repeated completions.
 *
 * Note: caller is expected to hold a lock to prevent concurrency.
 */
static inline void dma_cookie_complete(struct dma_async_tx_descriptor *tx)
{
	BUG_ON(tx->cookie < DMA_MIN_COOKIE);
	tx->chan->completed_cookie = tx->cookie;
	tx->cookie = 0;
}

/**
 * dma_cookie_status - report cookie status
 * @chan: dma channel
 * @cookie: cookie we are interested in
 * @state: dma_tx_state structure to return last/used cookies
 *
 * Report the status of the cookie, filling in the state structure if
 * non-NULL.  No locking is required.
 */
static inline enum dma_status dma_cookie_status(struct dma_chan *chan,
	dma_cookie_t cookie, struct dma_tx_state *state)
{
	dma_cookie_t used, complete;

	used = chan->cookie;
	complete = chan->completed_cookie;

	if (state) {
		state->last = complete;
		state->used = used;
		state->residue = 0;
	}
	return dma_async_is_complete(cookie, complete, used);
}

static inline void dma_set_residue(struct dma_tx_state *state, u32 residue)
{
	if (state)
		state->residue = residue;
}

struct dmaengine_desc_callback {
	dma_async_tx_callback callback;
	dma_async_tx_callback_result callback_result;
	void *callback_param;
};

/**
 * dmaengine_desc_get_callback - get the passed in callback function
 * @tx: tx descriptor
 * @cb: temp struct to hold the callback info
 *
 * Fill the passed in cb struct with what's available in the passed in
 * tx descriptor struct
 * No locking is required.
 */
static inline void
dmaengine_desc_get_callback(struct dma_async_tx_descriptor *tx,
				struct dmaengine_desc_callback *cb)
{
	cb->callback = tx->callback;
	cb->callback_result = tx->callback_result;
	cb->callback_param = tx->callback_param;
}

/**
 * dmaengine_desc_callback_invoke - call the callback function in cb struct
 * @cb: temp struct that is holding the callback info
 * @result: transaction result
 *
 * Call the callback function provided in the cb struct with the parameter
 * in the cb struct.
 * Locking is dependent on the driver.
 */
static inline void
dmaengine_desc_callback_invoke(struct dmaengine_desc_callback *cb,
				   const struct dmaengine_result *result)
{
	struct dmaengine_result dummy_result = {
		.result = DMA_TRANS_NOERROR,
		.residue = 0
	};

	if (cb->callback_result) {
		if (!result)
			result = &dummy_result;
		cb->callback_result(cb->callback_param, result);
	} else if (cb->callback) {
		cb->callback(cb->callback_param);
	}
}

/**
 * dmaengine_desc_get_callback_invoke - get the callback in tx descriptor and
 *                  then immediately call the callback.
 * @tx: dma async tx descriptor
 * @result: transaction result
 *
 * Call dmaengine_desc_get_callback() and dmaengine_desc_callback_invoke()
 * in a single function since no work is necessary in between for the driver.
 * Locking is dependent on the driver.
 */
static inline void
dmaengine_desc_get_callback_invoke(struct dma_async_tx_descriptor *tx,
				   const struct dmaengine_result *result)
{
	struct dmaengine_desc_callback cb;

	dmaengine_desc_get_callback(tx, &cb);
	dmaengine_desc_callback_invoke(&cb, result);
}

/**
 * dmaengine_desc_callback_valid - verify the callback is valid in cb
 * @cb: callback info struct
 *
 * Return a bool that verifies whether callback in cb is valid or not.
 * No locking is required.
 */
static inline bool
dmaengine_desc_callback_valid(struct dmaengine_desc_callback *cb)
{
	return (cb->callback) ? true : false;
}

#if 0
//one eg
#include "dmaengine.h"

void dma_test_callback(void *param)
{
	imi_printf("dma tr call back\n");
	dma_release_channel((struct dma_chan *)param);
}

void dmatest(void *pvParameters)
{
	struct dma_chan *chan;
	struct dma_async_tx_descriptor *desc;
	int ret;
	imi_printf("dma test start\n");

	chan = dma_request_chan(NULL, "chan_0");

	if (!chan) {
		imi_printf("can not request channel\n");
		goto end;
	}

	chan = dma_get_slave_channel(chan);
	if (chan == NULL) {
		imi_printf("can not get channel\n");
		goto end;
	}

	desc = dmaengine_prep_dma_memcpy(chan, 0xa0000000,
		0xa1000000, 0x200,
		DMA_PREP_INTERRUPT | DMA_CTRL_ACK);
	if (!desc) {
		pr_err("Failed to prep dma memoroy\n");
		goto end;
	}

	desc->callback = dma_test_callback;
	desc->callback_param = chan;
	ret = dma_submit_error(dmaengine_submit(desc));
	if (ret) {
		pr_err("Failed to do dma submit\n");
		goto end;
	}

	dma_async_issue_pending(chan);

end:
	imi_printf("end\n");
}
#endif

#endif
