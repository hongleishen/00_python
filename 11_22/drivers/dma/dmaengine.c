#include "imi_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "dev.h"
#include "initcalls.h"
#include "clock.h"
#include "dmaengine.h"

#include "errno.h"
#include "linux_compat.h"
#include "linux_list.h"
static DEFINE_MUTEX(dma_list_mutex);

struct dma_device *imi_dma_device = NULL;
static char *imi1680_dma_chan[] = {
	"chan_0","chan_1","chan_2","chan_3","chan_4","chan_5","chan_6","chan_7"
};

/**
 * dma_chan_get - try to grab a dma channel's parent driver module
 * @chan - channel to grab
 *
 * Must be called under dma_list_mutex
 */
static int dma_chan_get(struct dma_chan *chan)
{
	int ret;

	/* allocate upon first client reference */
	if (chan->device->device_alloc_chan_resources) {
		ret = chan->device->device_alloc_chan_resources(chan);
		if (ret < 0)
			return ret;
	}

	chan->client_count++;
	return 0;
}

/**
 * dma_chan_put - drop a reference to a dma channel's parent driver module
 * @chan - channel to release
 *
 * Must be called under dma_list_mutex
 */
static void dma_chan_put(struct dma_chan *chan)
{
	/* This channel is not in use, bail out */
	if (!chan->client_count)
		return;

	chan->client_count--;

	/* This channel is not in use anymore, free it */
	if (!chan->client_count && chan->device->device_free_chan_resources) {
		/* Make sure all operations have completed */
		dmaengine_synchronize(chan);
		chan->device->device_free_chan_resources(chan);
	}
}

enum dma_status dma_sync_wait(struct dma_chan *chan, dma_cookie_t cookie)
{
	enum dma_status status;
	unsigned long dma_sync_wait_timeout = xTaskGetTickCount() + msecs_to_jiffies(5000);

	dma_async_issue_pending(chan);
	do {
		status = dma_async_is_tx_complete(chan, cookie, NULL, NULL);
		if ((xTaskGetTickCount() - dma_sync_wait_timeout) > 0) {
			dev_err(chan->device->dev, "%s: timeout!\n", __func__);
			return DMA_ERROR;
		}
		if (status != DMA_IN_PROGRESS)
			break;
	} while (1);

	return status;
}

/**
 * dma_issue_pending_all - flush all pending operations across all channels
 */
void dma_issue_pending_all(void)
{
	struct dma_device *device = imi_dma_device;
	struct dma_chan *chan;

	list_for_each_entry(chan, &device->channels, device_node) {
			if (chan->client_count)
				device->device_issue_pending(chan);
	}
}

int dma_get_slave_caps(struct dma_chan *chan, struct dma_slave_caps *caps)
{
	struct dma_device *device;

	if (!chan || !caps)
		return -EINVAL;

	device = chan->device;

	/*
	 * Check whether it reports it uses the generic slave
	 * capabilities, if not, that means it doesn't support any
	 * kind of slave capabilities reporting.
	 */
	if (!device->directions)
		return -ENXIO;

	caps->src_addr_widths = device->src_addr_widths;
	caps->dst_addr_widths = device->dst_addr_widths;
	caps->directions = device->directions;
	caps->max_burst = device->max_burst;
	caps->residue_granularity = device->residue_granularity;
	caps->descriptor_reuse = device->descriptor_reuse;

	/*
	 * Some devices implement only pause (e.g. to get residuum) but no
	 * resume. However cmd_pause is advertised as pause AND resume.
	 */
	caps->cmd_pause = !!(device->device_pause && device->device_resume);
	caps->cmd_terminate = !!device->device_terminate_all;

	return 0;
}

/**
 * dma_get_slave_channel - try to get specific channel exclusively
 * @chan: target channel
 */
struct dma_chan *dma_get_slave_channel(struct dma_chan *chan)
{
	int err = -EBUSY;

	/* lock against __dma_request_channel */
	mutex_lock(&dma_list_mutex);

	if (chan->client_count == 0) {
		struct dma_device *device = chan->device;
		device->privatecnt++;
		err = dma_chan_get(chan);
		if (err) {
			dev_dbg(chan->device->dev,
				"%s: failed to get %s: (%d)\n",
				__func__, dma_chan_name(chan), err);
			chan = NULL;
			device->privatecnt--;
		}
	} else
		chan = NULL;

	mutex_unlock(&dma_list_mutex);


	return chan;
}

/**
 * dma_request_chan - try to allocate an exclusive slave channel
 * @dev:    pointer to client device structure
 * @name:   slave channel name
 *
 * Returns pointer to appropriate DMA channel on success or an error pointer.
 */
struct dma_chan *dma_request_chan(struct device *dev, const char *name)
{
	struct dma_chan *chan = NULL, *tmp = NULL;
	struct dma_device *dma_dev = imi_dma_device;

	/* Try to find the channel via the DMA filter map(s) */
	mutex_lock(&dma_list_mutex);
	list_for_each_entry(tmp, &dma_dev->channels, device_node) {
		if (!strcmp(tmp->chan_name, name)) {
			chan = tmp;
			break;
		}
	}

	mutex_unlock(&dma_list_mutex);

	return chan ? chan : ERR_PTR(-EPROBE_DEFER);
}

void dma_release_channel(struct dma_chan *chan)
{
	mutex_lock(&dma_list_mutex);
	WARN_ONCE(chan->client_count != 1,
		  "chan reference count %d != 1\n", chan->client_count);
	dma_chan_put(chan);
	--chan->device->privatecnt;
	mutex_unlock(&dma_list_mutex);
}

/**
 * dma_async_device_register - registers DMA devices found
 * @device: &dma_device
 */
int dma_async_device_register(struct dma_device *device)
{
	int chancnt = 0, rc;
	struct dma_chan* chan;

	imi_dma_device = device;

	/* represent channels in sysfs. Probably want devs too */
	list_for_each_entry(chan, &device->channels, device_node) {
		rc = -ENOMEM;
		chan->dev = pvPortMalloc(sizeof(*chan->dev));
		if (chan->dev == NULL) {
			goto err_out;
		}

		chan->chan_name = imi1680_dma_chan[chancnt];
		chan->chan_id = chancnt++;
		chan->dev->chan = chan;
		chan->dev->dev_id = device->dev_id;
		chan->client_count = 0;
		chan->private = (void *)chan->chan_id;
	}

	if (!chancnt) {
		dev_err(device->dev, "%s: device has no channels!\n", __func__);
		rc = -ENODEV;
		goto err_out;
	}

	device->chancnt = chancnt;

	mutex_lock(&dma_list_mutex);

	device->privatecnt++;   /* Always private */
	mutex_unlock(&dma_list_mutex);

	return 0;

err_out:
	list_for_each_entry(chan, &device->channels, device_node) {
		mutex_lock(&dma_list_mutex);
		chan->dev->chan = NULL;
		mutex_unlock(&dma_list_mutex);
	}
	return rc;
}

void dma_async_tx_descriptor_init(struct dma_async_tx_descriptor *tx,
	struct dma_chan *chan)
{
	tx->chan = chan;
}

/* dma_wait_for_async_tx - spin wait for a transaction to complete
 * @tx: in-flight transaction to wait on
 */
enum dma_status
dma_wait_for_async_tx(struct dma_async_tx_descriptor *tx)
{
	unsigned long dma_sync_wait_timeout = xTaskGetTickCount() + msecs_to_jiffies(5000);

	if (!tx)
		return DMA_COMPLETE;

	while (tx->cookie == -EBUSY) {
		if ((xTaskGetTickCount() - dma_sync_wait_timeout ) > 0) {
			dev_err(tx->chan->device->dev,
				"%s timeout waiting for descriptor submission\n",
				__func__);
			return DMA_ERROR;
		}
	}
	return dma_sync_wait(tx->chan, tx->cookie);
}

void dma_engine_probe(void)
{
	clock_enable("dma");
	mutex_init(&dma_list_mutex);
}
SUBSYS_INIT(dma_engine_probe);

