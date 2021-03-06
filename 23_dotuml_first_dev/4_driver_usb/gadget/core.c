/**
 * udc.c - Core UDC Framework
 *
 * Copyright (C) 2010 Texas Instruments
 * Author: Felipe Balbi <balbi@ti.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2  of
 * the License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <linux_compat.h>
#include <errno.h>
#include <list.h>
#include <usb/linux/ch9.h>
#include <usb/linux/gadget.h>

/**
 * struct usb_udc - describes one usb device controller
 * @driver - the gadget driver pointer. For use by the class code
 * @dev - the child device to the actual controller
 * @gadget - the gadget. For use by the class code
 * @list - for use by the udc class driver
 * @vbus - for udcs who care about vbus status, this value is real vbus status;
 * for udcs who do not care about vbus status, this value is always true
 *
 * This represents the internal data structure which is used by the UDC-class
 * to hold information about udc driver and gadget together.
 */
struct usb_udc {
	struct usb_gadget_driver    *driver;
	struct usb_gadget       *gadget;
	struct list_head        list;
	bool                vbus;
};

static LIST_HEAD(udc_list);
static int udc_bind_to_driver(struct usb_udc *udc,
		struct usb_gadget_driver *driver);

/* ------------------------------------------------------------------------- */

/**
 * usb_ep_set_maxpacket_limit - set maximum packet size limit for endpoint
 * @ep:the endpoint being configured
 * @maxpacket_limit:value of maximum packet size limit
 *
 * This function should be used only in UDC drivers to initialize endpoint
 * (usually in probe function).
 */
void usb_ep_set_maxpacket_limit(struct usb_ep *ep,
						  unsigned maxpacket_limit)
{
	ep->maxpacket_limit = maxpacket_limit;
	ep->maxpacket = maxpacket_limit;

}

/**
 * usb_ep_enable - configure endpoint, making it usable
 * @ep:the endpoint being configured.  may not be the endpoint named "ep0".
 *  drivers discover endpoints through the ep_list of a usb_gadget.
 *
 * When configurations are set, or when interface settings change, the driver
 * will enable or disable the relevant endpoints.  while it is enabled, an
 * endpoint may be used for i/o until the driver receives a disconnect() from
 * the host or until the endpoint is disabled.
 *
 * the ep0 implementation (which calls this routine) must ensure that the
 * hardware capabilities of each endpoint match the descriptor provided
 * for it.  for example, an endpoint named "ep2in-bulk" would be usable
 * for interrupt transfers as well as bulk, but it likely couldn't be used
 * for iso transfers or for endpoint 14.  some endpoints are fully
 * configurable, with more generic names like "ep-a".  (remember that for
 * USB, "in" means "towards the USB master".)
 *
 * returns zero, or a negative error code.
 */
int usb_ep_enable(struct usb_ep *ep)
{
	int ret = 0;

	if (ep->enabled)
		goto out;

	ret = ep->ops->enable(ep, ep->desc);
	if (ret)
		goto out;

	ep->enabled = true;

out:

	return ret;
}

/**
 * usb_ep_disable - endpoint is no longer usable
 * @ep:the endpoint being unconfigured.  may not be the endpoint named "ep0".
 *
 * no other task may be using this endpoint when this is called.
 * any pending and uncompleted requests will complete with status
 * indicating disconnect (-ESHUTDOWN) before this call returns.
 * gadget drivers must call usb_ep_enable() again before queueing
 * requests to the endpoint.
 *
 * returns zero, or a negative error code.
 */
int usb_ep_disable(struct usb_ep *ep)
{
	int ret = 0;

	if (!ep->enabled)
		goto out;

	ret = ep->ops->disable(ep);
	if (ret)
		goto out;

	ep->enabled = false;

out:

	return ret;
}

/**
 * usb_ep_alloc_request - allocate a request object to use with this endpoint
 * @ep:the endpoint to be used with with the request
 * @gfp_flags:GFP_* flags to use
 *
 * Request objects must be allocated with this call, since they normally
 * need controller-specific setup and may even need endpoint-specific
 * resources such as allocation of DMA descriptors.
 * Requests may be submitted with usb_ep_queue(), and receive a single
 * completion callback.  Free requests with usb_ep_free_request(), when
 * they are no longer needed.
 *
 * Returns the request, or null if one could not be allocated.
 */
struct usb_request *usb_ep_alloc_request(struct usb_ep *ep,
							   gfp_t gfp_flags)
{
	struct usb_request *req = NULL;

	req = ep->ops->alloc_request(ep, gfp_flags);


	return req;
}

/**
 * usb_ep_free_request - frees a request object
 * @ep:the endpoint associated with the request
 * @req:the request being freed
 *
 * Reverses the effect of usb_ep_alloc_request().
 * Caller guarantees the request is not queued, and that it will
 * no longer be requeued (or otherwise used).
 */
void usb_ep_free_request(struct usb_ep *ep,
					   struct usb_request *req)
{
	ep->ops->free_request(ep, req);
}

/**
 * usb_ep_queue - queues (submits) an I/O request to an endpoint.
 * @ep:the endpoint associated with the request
 * @req:the request being submitted
 * @gfp_flags: GFP_* flags to use in case the lower level driver couldn't
 *  pre-allocate all necessary memory with the request.
 *
 * This tells the device controller to perform the specified request through
 * that endpoint (reading or writing a buffer).  When the request completes,
 * including being canceled by usb_ep_dequeue(), the request's completion
 * routine is called to return the request to the driver.  Any endpoint
 * (except control endpoints like ep0) may have more than one transfer
 * request queued; they complete in FIFO order.  Once a gadget driver
 * submits a request, that request may not be examined or modified until it
 * is given back to that driver through the completion callback.
 *
 * Each request is turned into one or more packets.  The controller driver
 * never merges adjacent requests into the same packet.  OUT transfers
 * will sometimes use data that's already buffered in the hardware.
 * Drivers can rely on the fact that the first byte of the request's buffer
 * always corresponds to the first byte of some USB packet, for both
 * IN and OUT transfers.
 *
 * Bulk endpoints can queue any amount of data; the transfer is packetized
 * automatically.  The last packet will be short if the request doesn't fill it
 * out completely.  Zero length packets (ZLPs) should be avoided in portable
 * protocols since not all usb hardware can successfully handle zero length
 * packets.  (ZLPs may be explicitly written, and may be implicitly written if
 * the request 'zero' flag is set.)  Bulk endpoints may also be used
 * for interrupt transfers; but the reverse is not true, and some endpoints
 * won't support every interrupt transfer.  (Such as 768 byte packets.)
 *
 * Interrupt-only endpoints are less functional than bulk endpoints, for
 * example by not supporting queueing or not handling buffers that are
 * larger than the endpoint's maxpacket size.  They may also treat data
 * toggle differently.
 *
 * Control endpoints ... after getting a setup() callback, the driver queues
 * one response (even if it would be zero length).  That enables the
 * status ack, after transferring data as specified in the response.  Setup
 * functions may return negative error codes to generate protocol stalls.
 * (Note that some USB device controllers disallow protocol stall responses
 * in some cases.)  When control responses are deferred (the response is
 * written after the setup callback returns), then usb_ep_set_halt() may be
 * used on ep0 to trigger protocol stalls.  Depending on the controller,
 * it may not be possible to trigger a status-stage protocol stall when the
 * data stage is over, that is, from within the response's completion
 * routine.
 *
 * For periodic endpoints, like interrupt or isochronous ones, the usb host
 * arranges to poll once per interval, and the gadget driver usually will
 * have queued some data to transfer at that time.
 *
 * Note that @req's ->complete() callback must never be called from
 * within usb_ep_queue() as that can create deadlock situations.
 *
 * Returns zero, or a negative error code.  Endpoints that are not enabled
 * report errors; errors will also be
 * reported when the usb peripheral is disconnected.
 */
int usb_ep_queue(struct usb_ep *ep,
				   struct usb_request *req, gfp_t gfp_flags)
{
	int ret = 0;

	if (WARN_ON_ONCE(!ep->enabled && ep->address)) {
		ret = -ESHUTDOWN;
		goto out;
	}
	ret = ep->ops->queue(ep, req, gfp_flags);

out:

	return ret;
}

/**
 * usb_ep_dequeue - dequeues (cancels, unlinks) an I/O request from an endpoint
 * @ep:the endpoint associated with the request
 * @req:the request being canceled
 *
 * If the request is still active on the endpoint, it is dequeued and its
 * completion routine is called (with status -ECONNRESET); else a negative
 * error code is returned. This is guaranteed to happen before the call to
 * usb_ep_dequeue() returns.
 *
 * Note that some hardware can't clear out write fifos (to unlink the request
 * at the head of the queue) except as part of disconnecting from usb. Such
 * restrictions prevent drivers from supporting configuration changes,
 * even to configuration zero (a "chapter 9" requirement).
 */
int usb_ep_dequeue(struct usb_ep *ep, struct usb_request *req)
{
	int ret;

	ret = ep->ops->dequeue(ep, req);

	return ret;
}

/**
 * usb_ep_set_halt - sets the endpoint halt feature.
 * @ep: the non-isochronous endpoint being stalled
 *
 * Use this to stall an endpoint, perhaps as an error report.
 * Except for control endpoints,
 * the endpoint stays halted (will not stream any data) until the host
 * clears this feature; drivers may need to empty the endpoint's request
 * queue first, to make sure no inappropriate transfers happen.
 *
 * Note that while an endpoint CLEAR_FEATURE will be invisible to the
 * gadget driver, a SET_INTERFACE will not be.  To reset endpoints for the
 * current altsetting, see usb_ep_clear_halt().  When switching altsettings,
 * it's simplest to use usb_ep_enable() or usb_ep_disable() for the endpoints.
 *
 * Returns zero, or a negative error code.  On success, this call sets
 * underlying hardware state that blocks data transfers.
 * Attempts to halt IN endpoints will fail (returning -EAGAIN) if any
 * transfer requests are still queued, or if the controller hardware
 * (usually a FIFO) still holds bytes that the host hasn't collected.
 */
int usb_ep_set_halt(struct usb_ep *ep)
{
	int ret;

	ret = ep->ops->set_halt(ep, 1);

	return ret;
}

/**
 * usb_ep_clear_halt - clears endpoint halt, and resets toggle
 * @ep:the bulk or interrupt endpoint being reset
 *
 * Use this when responding to the standard usb "set interface" request,
 * for endpoints that aren't reconfigured, after clearing any other state
 * in the endpoint's i/o queue.
 *
 * Returns zero, or a negative error code.  On success, this call clears
 * the underlying hardware state reflecting endpoint halt and data toggle.
 * Note that some hardware can't support this request (like pxa2xx_udc),
 * and accordingly can't correctly implement interface altsettings.
 */
int usb_ep_clear_halt(struct usb_ep *ep)
{
	int ret;

	ret = ep->ops->set_halt(ep, 0);

	return ret;
}

/**
 * usb_ep_set_wedge - sets the halt feature and ignores clear requests
 * @ep: the endpoint being wedged
 *
 * Use this to stall an endpoint and ignore CLEAR_FEATURE(HALT_ENDPOINT)
 * requests. If the gadget driver clears the halt status, it will
 * automatically unwedge the endpoint.
 *
 * Returns zero on success, else negative errno.
 */
int usb_ep_set_wedge(struct usb_ep *ep)
{
	int ret;

	if (ep->ops->set_wedge)
		ret = ep->ops->set_wedge(ep);
	else
		ret = ep->ops->set_halt(ep, 1);


	return ret;
}

/**
 * usb_ep_fifo_status - returns number of bytes in fifo, or error
 * @ep: the endpoint whose fifo status is being checked.
 *
 * FIFO endpoints may have "unclaimed data" in them in certain cases,
 * such as after aborted transfers.  Hosts may not have collected all
 * the IN data written by the gadget driver (and reported by a request
 * completion).  The gadget driver may not have collected all the data
 * written OUT to it by the host.  Drivers that need precise handling for
 * fault reporting or recovery may need to use this call.
 *
 * This returns the number of such bytes in the fifo, or a negative
 * errno if the endpoint doesn't use a FIFO or doesn't support such
 * precise handling.
 */
int usb_ep_fifo_status(struct usb_ep *ep)
{
	int ret;

	if (ep->ops->fifo_status)
		ret = ep->ops->fifo_status(ep);
	else
		ret = -EOPNOTSUPP;


	return ret;
}

/**
 * usb_ep_fifo_flush - flushes contents of a fifo
 * @ep: the endpoint whose fifo is being flushed.
 *
 * This call may be used to flush the "unclaimed data" that may exist in
 * an endpoint fifo after abnormal transaction terminations.  The call
 * must never be used except when endpoint is not being used for any
 * protocol translation.
 */
void usb_ep_fifo_flush(struct usb_ep *ep)
{
	if (ep->ops->fifo_flush)
		ep->ops->fifo_flush(ep);

}

/* ------------------------------------------------------------------------- */

/**
 * usb_gadget_frame_number - returns the current frame number
 * @gadget: controller that reports the frame number
 *
 * Returns the usb frame number, normally eleven bits from a SOF packet,
 * or negative errno if this device doesn't support this capability.
 */
int usb_gadget_frame_number(struct usb_gadget *gadget)
{
	int ret;

	ret = gadget->ops->get_frame(gadget);


	return ret;
}

/**
 * usb_gadget_wakeup - tries to wake up the host connected to this gadget
 * @gadget: controller used to wake up the host
 *
 * Returns zero on success, else negative error code if the hardware
 * doesn't support such attempts, or its support has not been enabled
 * by the usb host.  Drivers must return device descriptors that report
 * their ability to support this, or hosts won't enable it.
 *
 * This may also try to use SRP to wake the host and start enumeration,
 * even if OTG isn't otherwise in use.  OTG devices may also start
 * remote wakeup even when hosts don't explicitly enable it.
 */
int usb_gadget_wakeup(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->ops->wakeup) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = gadget->ops->wakeup(gadget);

out:

	return ret;
}

/**
 * usb_gadget_set_selfpowered - sets the device selfpowered feature.
 * @gadget:the device being declared as self-powered
 *
 * this affects the device status reported by the hardware driver
 * to reflect that it now has a local power supply.
 *
 * returns zero on success, else negative errno.
 */
int usb_gadget_set_selfpowered(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->ops->set_selfpowered) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = gadget->ops->set_selfpowered(gadget, 1);

out:

	return ret;
}

/**
 * usb_gadget_clear_selfpowered - clear the device selfpowered feature.
 * @gadget:the device being declared as bus-powered
 *
 * this affects the device status reported by the hardware driver.
 * some hardware may not support bus-powered operation, in which
 * case this feature's value can never change.
 *
 * returns zero on success, else negative errno.
 */
int usb_gadget_clear_selfpowered(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->ops->set_selfpowered) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = gadget->ops->set_selfpowered(gadget, 0);

out:

	return ret;
}

/**
 * usb_gadget_vbus_connect - Notify controller that VBUS is powered
 * @gadget:The device which now has VBUS power.
 * Context: can sleep
 *
 * This call is used by a driver for an external transceiver (or GPIO)
 * that detects a VBUS power session starting.  Common responses include
 * resuming the controller, activating the D+ (or D-) pullup to let the
 * host detect that a USB device is attached, and starting to draw power
 * (8mA or possibly more, especially after SET_CONFIGURATION).
 *
 * Returns zero on success, else negative errno.
 */
int usb_gadget_vbus_connect(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->ops->vbus_session) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = gadget->ops->vbus_session(gadget, 1);

out:

	return ret;
}

/**
 * usb_gadget_vbus_draw - constrain controller's VBUS power usage
 * @gadget:The device whose VBUS usage is being described
 * @mA:How much current to draw, in milliAmperes.  This should be twice
 *  the value listed in the configuration descriptor bMaxPower field.
 *
 * This call is used by gadget drivers during SET_CONFIGURATION calls,
 * reporting how much power the device may consume.  For example, this
 * could affect how quickly batteries are recharged.
 *
 * Returns zero on success, else negative errno.
 */
int usb_gadget_vbus_draw(struct usb_gadget *gadget, unsigned mA)
{
	int ret = 0;

	if (!gadget->ops->vbus_draw) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = gadget->ops->vbus_draw(gadget, mA);
	if (!ret)
		gadget->mA = mA;

out:

	return ret;
}

/**
 * usb_gadget_vbus_disconnect - notify controller about VBUS session end
 * @gadget:the device whose VBUS supply is being described
 * Context: can sleep
 *
 * This call is used by a driver for an external transceiver (or GPIO)
 * that detects a VBUS power session ending.  Common responses include
 * reversing everything done in usb_gadget_vbus_connect().
 *
 * Returns zero on success, else negative errno.
 */
int usb_gadget_vbus_disconnect(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->ops->vbus_session) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	ret = gadget->ops->vbus_session(gadget, 0);

out:

	return ret;
}

/**
 * usb_gadget_connect - software-controlled connect to USB host
 * @gadget:the peripheral being connected
 *
 * Enables the D+ (or potentially D-) pullup.  The host will start
 * enumerating this gadget when the pullup is active and a VBUS session
 * is active (the link is powered).  This pullup is always enabled unless
 * usb_gadget_disconnect() has been used to disable it.
 *
 * Returns zero on success, else negative errno.
 */
int usb_gadget_connect(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->ops->pullup) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	if (gadget->deactivated) {
		/*
		 * If gadget is deactivated we only save new state.
		 * Gadget will be connected automatically after activation.
		 */
		gadget->connected = true;
		goto out;
	}

	ret = gadget->ops->pullup(gadget, 1);
	if (!ret)
		gadget->connected = 1;

out:

	return ret;
}

/**
 * usb_gadget_disconnect - software-controlled disconnect from USB host
 * @gadget:the peripheral being disconnected
 *
 * Disables the D+ (or potentially D-) pullup, which the host may see
 * as a disconnect (when a VBUS session is active).  Not all systems
 * support software pullup controls.
 *
 * Returns zero on success, else negative errno.
 */
int usb_gadget_disconnect(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->ops->pullup) {
		ret = -EOPNOTSUPP;
		goto out;
	}

	if (gadget->deactivated) {
		/*
		 * If gadget is deactivated we only save new state.
		 * Gadget will stay disconnected after activation.
		 */
		gadget->connected = false;
		goto out;
	}

	ret = gadget->ops->pullup(gadget, 0);
	if (!ret)
		gadget->connected = 0;

out:

	return ret;
}

/**
 * usb_gadget_deactivate - deactivate function which is not ready to work
 * @gadget: the peripheral being deactivated
 *
 * This routine may be used during the gadget driver bind() call to prevent
 * the peripheral from ever being visible to the USB host, unless later
 * usb_gadget_activate() is called.  For example, user mode components may
 * need to be activated before the system can talk to hosts.
 *
 * Returns zero on success, else negative errno.
 */
int usb_gadget_deactivate(struct usb_gadget *gadget)
{
	int ret = 0;

	if (gadget->deactivated)
		goto out;

	if (gadget->connected) {
		ret = usb_gadget_disconnect(gadget);
		if (ret)
			goto out;

		/*
		 * If gadget was being connected before deactivation, we want
		 * to reconnect it in usb_gadget_activate().
		 */
		gadget->connected = true;
	}
	gadget->deactivated = true;

out:

	return ret;
}

/**
 * usb_gadget_activate - activate function which is not ready to work
 * @gadget: the peripheral being activated
 *
 * This routine activates gadget which was previously deactivated with
 * usb_gadget_deactivate() call. It calls usb_gadget_connect() if needed.
 *
 * Returns zero on success, else negative errno.
 */
int usb_gadget_activate(struct usb_gadget *gadget)
{
	int ret = 0;

	if (!gadget->deactivated)
		goto out;

	gadget->deactivated = false;

	/*
	 * If gadget has been connected before deactivation, or became connected
	 * while it was being deactivated, we call usb_gadget_connect().
	 */
	if (gadget->connected)
		ret = usb_gadget_connect(gadget);

out:

	return ret;
}

int usb_gadget_map_request(struct usb_gadget *gadget,
		struct usb_request *req, int is_in)
{
	if (req->length == 0)
		return 0;

	req->dma = (dma_addr_t)req->buf;
	return 0;
}

void usb_gadget_unmap_request(struct usb_gadget *gadget,
		struct usb_request *req, int is_in)
{
	if (req->length == 0)
		return;
}

/* ------------------------------------------------------------------------- */

/**
 * usb_gadget_giveback_request - give the request back to the gadget layer
 * Context: in_interrupt()
 *
 * This is called by device controller drivers in order to return the
 * completed request back to the gadget layer.
 */
void usb_gadget_giveback_request(struct usb_ep *ep,
		struct usb_request *req)
{
	req->complete(ep, req);
}

/* ------------------------------------------------------------------------- */

/**
 * gadget_find_ep_by_name - returns ep whose name is the same as sting passed
 *  in second parameter or NULL if searched endpoint not found
 * @g: controller to check for quirk
 * @name: name of searched endpoint
 */
struct usb_ep *gadget_find_ep_by_name(struct usb_gadget *g, const char *name)
{
	struct usb_ep *ep;

	gadget_for_each_ep(ep, g) {
		if (!strcmp(ep->name, name))
			return ep;
	}

	return NULL;
}

int usb_gadget_ep_match_desc(struct usb_gadget *gadget,
		struct usb_ep *ep, struct usb_endpoint_descriptor *desc,
		struct usb_ss_ep_comp_descriptor *ep_comp)
{
	u8      type;
	u16     max;
	int     num_req_streams = 0;

	/* endpoint already claimed? */
	if (ep->claimed)
		return 0;

	type = usb_endpoint_type(desc);
	max = 0x7ff & usb_endpoint_maxp(desc);

	if (usb_endpoint_dir_in(desc) && !ep->caps.dir_in)
		return 0;

	if (usb_endpoint_dir_out(desc) && !ep->caps.dir_out)
		return 0;

	if (max > ep->maxpacket_limit)
		return 0;

	/* "high bandwidth" works only at high speed */
	if (!gadget_is_dualspeed(gadget) && usb_endpoint_maxp_mult(desc) > 1)
		return 0;

	switch (type) {
	case USB_ENDPOINT_XFER_CONTROL:
		/* only support ep0 for portable CONTROL traffic */
		return 0;
	case USB_ENDPOINT_XFER_ISOC:
		if (!ep->caps.type_iso)
			return 0;
		/* ISO:  limit 1023 bytes full speed, 1024 high/super speed */
		if (!gadget_is_dualspeed(gadget) && max > 1023)
			return 0;
		break;

	case USB_ENDPOINT_XFER_BULK:
		if (!ep->caps.type_bulk)
			return 0;

		if (ep_comp && gadget_is_superspeed(gadget)) {
			/* Get the number of required streams from the
			 * EP companion descriptor and see if the EP
			 * matches it
			 */
			num_req_streams = ep_comp->bmAttributes & 0x1f;
			if (num_req_streams > ep->max_streams)
				return 0;
		}
		break;

	case USB_ENDPOINT_XFER_INT:
		/* Bulk endpoints handle interrupt transfers,
		 * except the toggle-quirky iso-synch kind
		 */
		if (!ep->caps.type_int && !ep->caps.type_bulk)
			return 0;
		/* INT:  limit 64 bytes full speed, 1024 high/super speed */
		if (!gadget_is_dualspeed(gadget) && max > 64)
			return 0;
		break;
	}

	return 1;
}
void usb_gadget_set_state(struct usb_gadget *gadget,
		enum usb_device_state state)
{
	gadget->state = state;
}

/* ------------------------------------------------------------------------- */

static void usb_udc_connect_control(struct usb_udc *udc)
{
	if (udc->vbus)
		usb_gadget_connect(udc->gadget);
	else
		usb_gadget_disconnect(udc->gadget);
}

/**
 * usb_udc_vbus_handler - updates the udc core vbus status, and try to
 * connect or disconnect gadget
 * @gadget: The gadget which vbus change occurs
 * @status: The vbus status
 *
 * The udc driver calls it when it wants to connect or disconnect gadget
 * according to vbus status.
 */
void usb_udc_vbus_handler(struct usb_gadget *gadget, bool status)
{
	struct usb_udc *udc = gadget->udc;

	if (udc) {
		udc->vbus = status;
		usb_udc_connect_control(udc);
	}
}

/**
 * usb_gadget_udc_reset - notifies the udc core that bus reset occurs
 * @gadget: The gadget which bus reset occurs
 * @driver: The gadget driver we want to notify
 *
 * If the udc driver has bus reset handler, it needs to call this when the bus
 * reset occurs, it notifies the gadget driver that the bus reset occurs as
 * well as updates gadget state.
 */
void usb_gadget_udc_reset(struct usb_gadget *gadget,
		struct usb_gadget_driver *driver)
{
	driver->reset(gadget);
	usb_gadget_set_state(gadget, USB_STATE_DEFAULT);
}

/**
 * usb_gadget_udc_start - tells usb device controller to start up
 * @udc: The UDC to be started
 *
 * This call is issued by the UDC Class driver when it's about
 * to register a gadget driver to the device controller, before
 * calling gadget driver's bind() method.
 *
 * It allows the controller to be powered off until strictly
 * necessary to have it powered on.
 *
 * Returns zero on success, else negative errno.
 */
static inline int usb_gadget_udc_start(struct usb_udc *udc)
{

	return udc->gadget->ops->udc_start(udc->gadget, udc->driver);
}

/**
 * usb_gadget_udc_stop - tells usb device controller we don't need it anymore
 * @gadget: The device we want to stop activity
 * @driver: The driver to unbind from @gadget
 *
 * This call is issued by the UDC Class driver after calling
 * gadget driver's unbind() method.
 *
 * The details are implementation specific, but it can go as
 * far as powering off UDC completely and disable its data
 * line pullups.
 */
static inline void usb_gadget_udc_stop(struct usb_udc *udc)
{
	udc->gadget->ops->udc_stop(udc->gadget);
}

/**
 * usb_gadget_udc_set_speed - tells usb device controller speed supported by
 *    current driver
 * @udc: The device we want to set maximum speed
 * @speed: The maximum speed to allowed to run
 *
 * This call is issued by the UDC Class driver before calling
 * usb_gadget_udc_start() in order to make sure that we don't try to
 * connect on speeds the gadget driver doesn't support.
 */
static inline void usb_gadget_udc_set_speed(struct usb_udc *udc,
						enum usb_device_speed speed)
{
	if (udc->gadget->ops->udc_set_speed) {
		enum usb_device_speed s;

		s = min(speed, udc->gadget->max_speed);
		udc->gadget->ops->udc_set_speed(udc->gadget, s);
	}
}

static struct usb_udc       _imi_udc;
int usb_add_gadget_udc_release(struct usb_gadget *gadget)
{
	struct usb_udc      *udc = 0;

	udc = &_imi_udc;

	udc->gadget = gadget;
	gadget->udc = udc;

	list_add_tail(&udc->list, &udc_list);

	usb_gadget_set_state(gadget, USB_STATE_NOTATTACHED);
	udc->vbus = true;

	return 0;
}

int usb_add_gadget_udc(struct usb_gadget *gadget)
{
	return usb_add_gadget_udc_release(gadget);
}

/* ------------------------------------------------------------------------- */

static int udc_bind_to_driver(struct usb_udc *udc, struct usb_gadget_driver *driver)
{
	int ret;

	udc->driver = driver;
	usb_gadget_udc_set_speed(udc, driver->max_speed);

	ret = driver->bind(udc->gadget, driver);
	if (ret)
		goto err1;

	ret = usb_gadget_udc_start(udc);
	if (ret) {
		driver->unbind(udc->gadget);
		goto err1;
	}

	udc->vbus = true;
	usb_udc_connect_control(udc);

	return 0;

err1:
	udc->driver = NULL;
	return ret;
}

int usb_gadget_probe_driver(struct usb_gadget_driver *driver)
{
	struct usb_udc      *udc = NULL;
	int         ret = -ENODEV;

	if (!driver || !driver->bind || !driver->setup)
		return -EINVAL;

	list_for_each_entry(udc, &udc_list, list) {

		/* For now we take the first one */
		if (!udc->driver)
			goto found;
	}

	return ret;
found:
	ret = udc_bind_to_driver(udc, driver);

	return ret;
}

int usb_gadget_register_driver(struct usb_gadget_driver *driver)
{
	return usb_gadget_probe_driver(driver);
}
