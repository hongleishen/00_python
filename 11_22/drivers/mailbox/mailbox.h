#ifndef __MAILBOX_H__
#define __MAILBOX_H__
#include "imi_type.h"
#include "vic_int.h"

/*********************** mailbox **************************/

#define REG_A2B_INTEN			(0x00)
#define REG_A2B_STATUS			(0x04)
#define REG_A2B_CMD_0			(0x08)
#define REG_A2B_TRIGGER0		(0x0C)
#define REG_A2B_CMD_1			(0x10)
#define REG_A2B_TRIGGER1		(0x14)
#define REG_A2B_CMD_2			(0x18)
#define REG_A2B_TRIGGER2		(0x1C)
#define REG_A2B_CMD_3			(0x20)
#define REG_A2B_TRIGGER3		(0x24)

#define REG_B2A_INTEN			(0x28)
#define REG_B2A_STATUS			(0x2C)
#define REG_B2A_CMD_0			(0x30)
#define REG_B2A_TRIGGER0		(0x34)
#define REG_B2A_CMD_1			(0x38)
#define REG_B2A_TRIGGER1		(0x3C)
#define REG_B2A_CMD_2			(0x40)
#define REG_B2A_TRIGGER2		(0x44)
#define REG_B2A_CMD_3			(0x48)
#define REG_B2A_TRIGGER3		(0x4C)
/**********************************************************/

enum {
	MAILBOX_CHANNEL0,
	MAILBOX_CHANNEL1,
	MAILBOX_CHANNEL2,
	MAILBOX_CHANNEL3
};

struct mail_desc {
   	u32 base_addr;  //b base address
   	u32 mbox_id;
	u32 cross_addr; //a base address
};

typedef void (* _a2b_enable)(struct mail_desc*);
typedef void (* _a2b_clear)(struct mail_desc*);
typedef u32 (* _a2b_status)(struct mail_desc*);
typedef void (* _a2b_trigger)(struct mail_desc*, u32);
typedef void (* _b2a_enable)(struct mail_desc*);
typedef void (* _b2a_clear)(struct mail_desc*);
typedef u32 (* _b2a_status)(struct mail_desc*);
typedef void (* _b2a_trigger)(struct mail_desc*, u32);

struct mailbox_driver {
	_a2b_enable a2b_enable;
	_a2b_clear a2b_clear;
	_a2b_status a2b_status;
	_a2b_trigger a2b_trigger;
	_b2a_enable b2a_enable;
	_b2a_clear b2a_clear;
	_b2a_status b2a_status;
	_b2a_trigger b2a_trigger;
};

struct mail_desc imi_mail_desc;
irqreturn_t mailbox_a2b00_handler(int irq, void *arg);
irqreturn_t mailbox_b2a00_handler(int irq, void *arg);

#endif
