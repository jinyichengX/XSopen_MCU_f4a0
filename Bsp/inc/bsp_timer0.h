#ifndef __BSP_TIMER0_H__
#define __BSP_TIMER0_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_tmr0.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll.h"

/* select timer0 function */
//#define TMR0_FUNC_BASE //<<（本工程中TMR0用于普通定时和串口超时功能，使用串口timeout功能时将此行注释）
#ifndef TMR0_FUNC_BASE
#define TMR0_FUNC_USART_TIMEOUT
#endif


#ifdef TMR0_FUNC_BASE
typedef struct Timer0Unitx_Chx_configurationInfo_struct
{
	CM_TMR0_TypeDef * TMR_UNITx;
	
	uint8_t TMR0_CH;

	uint32_t TMR0_CLK;
	
	stc_tmr0_init_t stcTmr0Init;
	
	uint32_t TMRx_CHx_Int_Mode;
	
	stc_irq_signin_config_t stcIrqSignConfig;
	
	uint8_t IRQ_Prio; 
}Tmr0x_Chx_cfgInfoTypedef;

extern Tmr0x_Chx_cfgInfoTypedef Tmr01ChA,Tmr01ChB,Tmr02ChA,Tmr02ChB;

extern void BSP_TMR0_Config(Tmr0x_Chx_cfgInfoTypedef *);
extern void BSP_TMR0_Start(Tmr0x_Chx_cfgInfoTypedef *);
#else

#include "bsp_uart.h"

extern void BSP_TMR0_Config_For_UsartTimeOut(Usart_cfgInfo_Typedef *Usart_cfgInfo);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __BSP_TIMER0_H__ */

