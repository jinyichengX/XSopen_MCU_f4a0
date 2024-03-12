#ifndef __BSP_EXTINT_KEY_H__
#define __BSP_EXTINT_KEY_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "hc32_ll_gpio.h"
#include "hc32_ll.h"

//<<用户在此修改IO端口和引脚，添加按键参考下面的宏定义
#define KEY1_PORT              (GPIO_PORT_A)
#define KEY1_PIN               (GPIO_PIN_00)
#define KEY1_EXTINT_CH         (EXTINT_CH00)
#define KEY1_INT_SRC           (INT_SRC_PORT_EIRQ0)
#define KEY1_INT_IRQn          (INT001_IRQn)
#define KEY1_INT_PRIO          (DDL_IRQ_PRIO_DEFAULT)

#define KEY2_PORT              (GPIO_PORT_A)
#define KEY2_PIN               (GPIO_PIN_01)
#define KEY2_EXTINT_CH         (EXTINT_CH01)
#define KEY2_INT_SRC           (INT_SRC_PORT_EIRQ1)
#define KEY2_INT_IRQn          (INT002_IRQn)
#define KEY2_INT_PRIO          (DDL_IRQ_PRIO_DEFAULT+1)

#define KEY3_PORT              (GPIO_PORT_A)
#define KEY3_PIN               (GPIO_PIN_02)
#define KEY3_EXTINT_CH         (EXTINT_CH02)
#define KEY3_INT_SRC           (INT_SRC_PORT_EIRQ2)
#define KEY3_INT_IRQn          (INT003_IRQn)
#define KEY3_INT_PRIO          (DDL_IRQ_PRIO_DEFAULT+2)

#define KEY4_PORT              (GPIO_PORT_A)
#define KEY4_PIN               (GPIO_PIN_03)
#define KEY4_EXTINT_CH         (EXTINT_CH03)
#define KEY4_INT_SRC           (INT_SRC_PORT_EIRQ3)
#define KEY4_INT_IRQn          (INT004_IRQn)
#define KEY4_INT_PRIO          (DDL_IRQ_PRIO_DEFAULT+3)

#define KEY5_PORT              (GPIO_PORT_A)
#define KEY5_PIN               (GPIO_PIN_04)
#define KEY5_EXTINT_CH         (EXTINT_CH04)
#define KEY5_INT_SRC           (INT_SRC_PORT_EIRQ4)
#define KEY5_INT_IRQn          (INT005_IRQn)
#define KEY5_INT_PRIO          (DDL_IRQ_PRIO_DEFAULT+4)

#define KEY6_PORT              (GPIO_PORT_A)
#define KEY6_PIN               (GPIO_PIN_05)
#define KEY6_EXTINT_CH         (EXTINT_CH05)
#define KEY6_INT_SRC           (INT_SRC_PORT_EIRQ5)
#define KEY6_INT_IRQn          (INT006_IRQn)
#define KEY6_INT_PRIO          (DDL_IRQ_PRIO_DEFAULT+5)

extern void KEY1_Init(void);
extern void KEY2_Init(void);
extern void KEY3_Init(void);
extern void KEY4_Init(void);
extern void KEY5_Init(void);
extern void KEY6_Init(void);
#ifdef __cplusplus
}
#endif
#endif /*__BSP_EXTINT_KEY_H__*/
