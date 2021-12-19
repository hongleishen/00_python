
#define NGPIO_NUM               (128)

#define GPAD_BASE_ADDR          (0x20F08400)
#define GIOM_CFG0               (GPAD_BASE_ADDR + 0x0) /* gpad 0~31 */
#define GIOM_CFG1               (GPAD_BASE_ADDR + 0x4) /* gpad 32~63 */
#define GIOM_CFG2               (GPAD_BASE_ADDR + 0x8) /* gpad 64~95 */
#define GIOM_CFG3               (GPAD_BASE_ADDR + 0xC) /* gpad 96~127 */
#define GMUX_CFG                (GPAD_BASE_ADDR + 0x80)

#define GPIO_DB_PERIOD_REG      (GPIO_BASE_ADDR + 0x0)
#define GPIO_CFG_REG(n)         (GPIO_BASE_ADDR + 0x8 + (n * 8))
#define GPIO_STS_REG(n)         (GPIO_BASE_ADDR + 0xC + (n * 8))
#define GPIO_MINT0_REG          (GPIO_BASE_ADDR + 0x1000) /* gpio 0~31, read only*/
#define GPIO_MINT1_REG          (GPIO_BASE_ADDR + 0x1004) /* gpio 32~63, read only*/
#define GPIO_MINT2_REG          (GPIO_BASE_ADDR + 0x1008) /* gpio 64~95, read only*/
#define GPIO_MINT3_REG          (GPIO_BASE_ADDR + 0x100C) /* gpio 96~127, read only*/

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

