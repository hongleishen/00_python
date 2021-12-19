#include "imi_io.h"
#include "dev.h"
#include "clock.h"
#include "initcalls.h"
#include "delay.h"
#include "mailbox.h"
#include "hellboy_map.h"
#include "linux_compat.h"
#include "elog.h"

void imi1680_a2b_enable(struct mail_desc *desc)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->a2b_enable) {
		log_e("my_mailbox_driver->a2b_enable is NULL\n");
		return;
	}
	my_mailbox_driver->a2b_enable(desc);
}

void imi1680_a2b_clear(struct mail_desc *desc)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->a2b_clear) {
		log_e("my_mailbox_driver->a2b_clear is NULL\n");
		return;
	}
	my_mailbox_driver->a2b_clear(desc);
}

u32 imi1680_a2b_status(struct mail_desc *desc)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->a2b_status) {
		log_e("my_mailbox_driver->a2b_status is NULL\n");
		return -1;
	}
	return my_mailbox_driver->a2b_status(desc);
}

void imi1680_a2b_trigger(struct mail_desc *desc, u32 channel)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->a2b_trigger) {
		log_e("my_mailbox_driver->a2b_trigger is NULL\n");
		return;
	}
	my_mailbox_driver->a2b_trigger(desc, channel);
}

void imi1680_b2a_enable(struct mail_desc *desc)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->b2a_enable) {
		log_e("my_mailbox_driver->b2a_enable is NULL\n");
		return;
	}
	my_mailbox_driver->b2a_enable(desc);
}

void imi1680_b2a_clear(struct mail_desc *desc)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->b2a_clear) {
		log_e("my_mailbox_driver->b2a_clear is NULL\n");
		return;
	}
	my_mailbox_driver->b2a_clear(desc);
}

u32 imi1680_b2a_status(struct mail_desc *desc)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->b2a_status) {
		log_e("my_mailbox_driver->b2a_status is NULL\n");
		return -1;
	}
	return my_mailbox_driver->b2a_status(desc);
}

void imi1680_b2a_trigger(struct mail_desc *desc, u32 channel)
{
	struct mailbox_driver *my_mailbox_driver = (struct mailbox_driver *)(dev_list[MAILBOX].ops);
	if (!my_mailbox_driver->b2a_trigger) {
		log_e("my_mailbox_driver->b2a_trigger is NULL\n");
		return;
	}
	my_mailbox_driver->b2a_trigger(desc, channel);
}


