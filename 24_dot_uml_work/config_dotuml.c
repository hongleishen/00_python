// 原始指令
// python .\5.py .\6_usb_app\ simple .\rm_class.c

// 现在统一在config_dotuml文件中


// **** 1. 是否simple*** , simple: 没有关联的 class讲祛除
simple

// **** 2. rm_class **** ， rm_class是开关， 之后才是要移除的class
// rm_class

// usb_ep_ops
// usb_ep_caps
// usb_ss_ep_comp_descriptor

// usb_gadget
// usb_gadget_ops
// usb_gadget_driver
// usb_udc

// imi_video_buffer

// usb_endpoint_descriptor
// imi_upload_dev


// ** under_stand ****
rm_class
usb_udc
usb_gadget_driver
usb_gadget_strings
usb_string
musb_context_registers
musb_csr_regs

imi_video_buffer
musb_platform_ops
usb_gadget_ops

usb_ep_ops
usb_ep_caps


// **** 3. rm file ****
//u_imi.h
//imi_video.h