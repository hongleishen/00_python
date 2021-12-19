#ifndef __IMI_MAILBOX_H__
#define __IMI_MAILBOX_H__
#include "imi_type.h"
#include "vic_int.h"

void imi1680_a2b_enable(struct mail_desc *desc);
void imi1680_a2b_clear(struct mail_desc *desc);
u32 imi1680_a2b_status(struct mail_desc *desc);
void imi1680_a2b_trigger(struct mail_desc *desc, u32 channel);
void imi1680_b2a_enable(struct mail_desc *desc);
void imi1680_b2a_clear(struct mail_desc *desc);
u32 imi1680_b2a_status(struct mail_desc *desc);
void imi1680_b2a_trigger(struct mail_desc *desc, u32 channel);

#endif
