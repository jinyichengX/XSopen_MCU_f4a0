#ifndef __BSP_TIMER2_H__
#define __BSP_TIMER2_H__

#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_tmr2.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll.h"

typedef struct Timer2Unitx_Chx_configurationInfo_struct
{
	CM_TMR2_TypeDef * TMR2_UNITx;
	
	uint8_t TMR2_CH;

	uint32_t TMR2_CLK;
	
	stc_tmr2_init_t stcTmr2Init;
	
	uint32_t TMRx_CHx_Int_Type;
	
	stc_irq_signin_config_t stcIrqSignConfig;
	
	uint8_t IRQ_Prio;
}Tmr2x_CHx_cfgInfoTypedef;


extern Tmr2x_CHx_cfgInfoTypedef Tmr2_1_ChA;

extern void BSP_Tmr2Config(Tmr2x_CHx_cfgInfoTypedef * Tmr2x_Chx);
extern void BSP_Tmr2Start(Tmr2x_CHx_cfgInfoTypedef * Tmr2x_Chx);

#ifdef __cplusplus
}
#endif
#endif /* __BSP_TIMER2_H__ */


