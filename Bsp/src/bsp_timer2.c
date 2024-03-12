#include "bsp_timer2.h"
#include "led.h"

/**
 * @brief  Timer2 counter comparison match interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMR2_1_Cmp_IrqCallback(void)
{
    TMR2_ClearStatus(CM_TMR2_1, TMR2_FLAG_MATCH_CH_A);
}


Tmr2x_CHx_cfgInfoTypedef Tmr2_1_ChA = {
	.TMR2_UNITx = CM_TMR2_1,
	
	.TMR2_CLK = FCG2_PERIPH_TMR2_1,
	
	.TMR2_CH = TMR2_CH_A,
	
	.TMRx_CHx_Int_Type = TMR2_INT_MATCH_CH_A,
	
	.stcTmr2Init = {
		.u32ClockSrc = TMR2_CLK_XTAL32,	//用户更改
		.u32ClockDiv = TMR2_CLK_DIV2,//用户更改
		.u32Func = TMR2_FUNC_CMP,
		.u32CompareValue = (60000UL - 1U),//用户更改
	},
	.stcIrqSignConfig = {
		.enIntSrc    = INT_SRC_TMR2_1_CMP_A,	//中断源选择
    .enIRQn      = INT007_IRQn,						//分配中断号
    .pfnCallback = TMR2_1_Cmp_IrqCallback,	//中断回调函数
	},
	.IRQ_Prio = 20,
};

/**
 * @brief  Timer2 configuration.
 * @param  None
 * @retval None
 */
void BSP_Tmr2Config(Tmr2x_CHx_cfgInfoTypedef * Tmr2x_Chx)
{
    FCG_Fcg2PeriphClockCmd(Tmr2x_Chx->TMR2_CLK, ENABLE);

    (void)TMR2_Init(Tmr2x_Chx->TMR2_UNITx, Tmr2x_Chx->TMR2_CH, &Tmr2x_Chx->stcTmr2Init);

    (void)INTC_IrqSignIn(&Tmr2x_Chx->stcIrqSignConfig);
    NVIC_ClearPendingIRQ(Tmr2x_Chx->stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(Tmr2x_Chx->stcIrqSignConfig.enIRQn, Tmr2x_Chx->IRQ_Prio);
    NVIC_EnableIRQ(Tmr2x_Chx->stcIrqSignConfig.enIRQn);

		/* Enable the specified interrupts of Timer2. */
    TMR2_IntCmd(Tmr2x_Chx->TMR2_UNITx, Tmr2x_Chx->TMRx_CHx_Int_Type, ENABLE);
#if (TMR2_USE_HW_TRIG > 0U)
    /* 5. Configures hardware trigger condition if needed. */
    Tmr2TriggerCondConfig();
#endif /* #if (TMR2_USE_HW_TRIG > 0U) */
}


void BSP_Tmr2Start(Tmr2x_CHx_cfgInfoTypedef * Tmr2x_Chx)
{
	TMR2_Start(Tmr2x_Chx->TMR2_UNITx, Tmr2x_Chx->TMR2_CH);
}


