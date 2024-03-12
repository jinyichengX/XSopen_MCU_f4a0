#ifndef __ILI9341_H__
#define __ILI9341_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "hc32_ll_smc.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll.h"
#include "hc32_ll_gpio.h"
#include <string.h>

#define BSP_ILI9341_CS_PORT             (GPIO_PORT_G)   /* PG10 - EXMC_CE2 */
#define BSP_ILI9341_CS_PIN              (GPIO_PIN_12)

#define BSP_ILI9341_WE_PORT             (GPIO_PORT_C)   /* PC00 - EXMC_WE */
#define BSP_ILI9341_WE_PIN              (GPIO_PIN_00)

#define BSP_ILI9341_OE_PORT             (GPIO_PORT_F)   /* PF11 - EXMC_OE */
#define BSP_ILI9341_OE_PIN              (GPIO_PIN_11)

#define BSP_ILI9341_RS_PORT             (GPIO_PORT_G)   /* PG02 - EXMC_ADD12 for LCD_RS */
#define BSP_ILI9341_RS_PIN              (GPIO_PIN_02)

#define BSP_ILI9341_DATA0_PORT          (GPIO_PORT_D)   /* PD14 - EXMC_DATA0 */
#define BSP_ILI9341_DATA0_PIN           (GPIO_PIN_14)
#define BSP_ILI9341_DATA1_PORT          (GPIO_PORT_D)   /* PD15 - EXMC_DATA1 */
#define BSP_ILI9341_DATA1_PIN           (GPIO_PIN_15)
#define BSP_ILI9341_DATA2_PORT          (GPIO_PORT_D)   /* PD00 - EXMC_DATA2 */
#define BSP_ILI9341_DATA2_PIN           (GPIO_PIN_00)
#define BSP_ILI9341_DATA3_PORT          (GPIO_PORT_D)   /* PD01 - EXMC_DATA3 */
#define BSP_ILI9341_DATA3_PIN           (GPIO_PIN_01)
#define BSP_ILI9341_DATA4_PORT          (GPIO_PORT_E)   /* PE07 - EXMC_DATA4 */
#define BSP_ILI9341_DATA4_PIN           (GPIO_PIN_07)
#define BSP_ILI9341_DATA5_PORT          (GPIO_PORT_E)   /* PE08 - EXMC_DATA5 */
#define BSP_ILI9341_DATA5_PIN           (GPIO_PIN_08)
#define BSP_ILI9341_DATA6_PORT          (GPIO_PORT_E)   /* PE09 - EXMC_DATA6 */
#define BSP_ILI9341_DATA6_PIN           (GPIO_PIN_09)
#define BSP_ILI9341_DATA7_PORT          (GPIO_PORT_E)   /* PE10 - EXMC_DATA7 */
#define BSP_ILI9341_DATA7_PIN           (GPIO_PIN_10)
#define BSP_ILI9341_DATA8_PORT          (GPIO_PORT_E)   /* PE11 - EXMC_DATA8 */
#define BSP_ILI9341_DATA8_PIN           (GPIO_PIN_11)
#define BSP_ILI9341_DATA9_PORT          (GPIO_PORT_E)   /* PE12 - EXMC_DATA9 */
#define BSP_ILI9341_DATA9_PIN           (GPIO_PIN_12)
#define BSP_ILI9341_DATA10_PORT         (GPIO_PORT_E)   /* PE13 - EXMC_DATA10 */
#define BSP_ILI9341_DATA10_PIN          (GPIO_PIN_13)
#define BSP_ILI9341_DATA11_PORT         (GPIO_PORT_E)   /* PE14 - EXMC_DATA11 */
#define BSP_ILI9341_DATA11_PIN          (GPIO_PIN_14)
#define BSP_ILI9341_DATA12_PORT         (GPIO_PORT_E)   /* PE15 - EXMC_DATA12 */
#define BSP_ILI9341_DATA12_PIN          (GPIO_PIN_15)
#define BSP_ILI9341_DATA13_PORT         (GPIO_PORT_D)   /* PD08 - EXMC_DATA13 */
#define BSP_ILI9341_DATA13_PIN          (GPIO_PIN_08)
#define BSP_ILI9341_DATA14_PORT         (GPIO_PORT_D)   /* PD09 - EXMC_DATA14 */
#define BSP_ILI9341_DATA14_PIN          (GPIO_PIN_09)
#define BSP_ILI9341_DATA15_PORT         (GPIO_PORT_D)   /* PD10 - EXMC_DATA15 */
#define BSP_ILI9341_DATA15_PIN          (GPIO_PIN_10)


extern int32_t LCD_SMC_Init(void);
#ifdef __cplusplus
}
#endif
#endif /* __ILI9341_H__ */


