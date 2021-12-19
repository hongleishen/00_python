#include "hellboy_map.h"
#define GPIO_NUM_MAX				(255)
#define GPIO_BANK_NO				(0x100)

#define NGPIO_NUM_MAX           (60)

#define GPIO_BASE_ADDR          GPIO0_BASE_ADDR
#define GPIO_BANK_GAP           0x100000

#define GPIO_DB_PERIOD_REG      (0x0)
#define GPIO_CFG_REG(n)         (0x8 + (n * 8))
#define GPIO_STS_REG(n)         (0xC + (n * 8))

#define CFG_BIT_DIR             (0)
#define CFG_BIT_OUT             (1)
#define CFG_BIT_IEN             (2)
#define CFG_BIT_DB_EN           (3)
#define CFG_BIT_INTR_EN         (4)
#define CFG_BIT_INTR_TYPE       (5)
#define CFG_BIT_INTR_MASK       (7)

#define STS_BIT_IN              (0)
#define STS_BIT_INTR            (1)
#define STS_BIT_INTR_NUM        (4)
