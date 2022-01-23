// 原始指令
// python .\5.py .\6_usb_app\ simple .\rm_class.c

// 现在统一在config_dotuml文件中


// **** 1. 是否simple*** , simple: 没有关联的 class讲祛除
simple

// **** 2. rm_class **** ， rm_class是开关， 之后才是要移除的class
rm_class

usb_ep_ops
usb_ep_caps
usb_ss_ep_comp_descriptor

usb_gadget
usb_gadget_ops
usb_gadget_driver
usb_udc

imi_video_buffer

usb_endpoint_descriptor

// **** 3. rm file ****
//u_imi.h