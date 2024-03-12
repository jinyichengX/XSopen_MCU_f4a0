#include "bsp_timer0.h"
#include "led.h"

#ifdef TMR0_FUNC_BASE
/**
 * @brief  TMR0 compare interrupt callback function.
 * @param  None
 * @retval None
 */
static void TMR01_A_CompareIrqCallback(void)
{
    TMR0_ClearStatus(CM_TMR0_1, TMR0_FLAG_CMP_A);
	
		/* usr interrupt processing code begin */
		Led1_Toggle;//测试用
	
		/* usr interrupt processing code end */
}

static void TMR01_B_CompareIrqCallback(void)
{
    TMR0_ClearStatus(CM_TMR0_1, TMR0_FLAG_CMP_B);
	
		/* usr interrupt processing code begin */
	
			Led1_Toggle;//测试用
	
		/* usr interrupt processing code end */
}

static void TMR02_A_CompareIrqCallback(void)
{
    TMR0_ClearStatus(CM_TMR0_2, TMR0_FLAG_CMP_A);
	
		/* usr interrupt processing code begin */
	
		Led1_Toggle;//测试用
	
		/* usr interrupt processing code end */
}

static void TMR02_B_CompareIrqCallback(void)
{
    TMR0_ClearStatus(CM_TMR0_2, TMR0_FLAG_CMP_B);
	
		/* usr interrupt processing code begin */
	
		Led1_Toggle;//测试用
	
		/* usr interrupt processing code end */
}


Tmr0x_Chx_cfgInfoTypedef Tmr01ChA = {
	.TMR_UNITx = CM_TMR0_1,
	
	.TMR0_CLK = FCG2_PERIPH_TMR0_1,
	
	.TMR0_CH = TMR0_CH_A,
	
	.TMRx_CHx_Int_Mode = TMR0_INT_CMP_A,
	
	.stcTmr0Init = {
		.u32ClockSrc = TMR0_CLK_SRC_XTAL32,
		.u32ClockDiv = TMR0_CLK_DIV16,
		.u32Func = TMR0_FUNC_CMP,
		.u16CompareValue = (uint16_t)(XTAL32_VALUE / 16U / 2U - 1U),
	},
	.stcIrqSignConfig = {
		.enIntSrc    = INT_SRC_TMR0_1_CMP_A,	//中断源选择
    .enIRQn      = INT007_IRQn,						//分配中断号
    .pfnCallback = TMR01_A_CompareIrqCallback,	//中断回调函数
	},
	.IRQ_Prio = 20,
};

Tmr0x_Chx_cfgInfoTypedef Tmr01ChB = {
	.TMR_UNITx = CM_TMR0_1,
	
	.TMR0_CLK = FCG2_PERIPH_TMR0_1,
	
	.TMR0_CH = TMR0_CH_B,
	
	.TMRx_CHx_Int_Mode = TMR0_INT_CMP_B,
	
	.stcTmr0Init = {
		.u32ClockSrc = TMR0_CLK_SRC_XTAL32,
		.u32ClockDiv = TMR0_CLK_DIV16,
		.u32Func = TMR0_FUNC_CMP,
		.u16CompareValue = (uint16_t)(XTAL32_VALUE / 16U / 2U - 1U),
	},
	.stcIrqSignConfig = {
		.enIntSrc    = INT_SRC_TMR0_1_CMP_B,	//中断源选择
    .enIRQn      = INT008_IRQn,						//分配中断号
    .pfnCallback = TMR01_B_CompareIrqCallback,	//中断回调函数
	},
	.IRQ_Prio = 21,
};

Tmr0x_Chx_cfgInfoTypedef Tmr02ChA = {
	.TMR_UNITx = CM_TMR0_2,
	
	.TMR0_CLK = FCG2_PERIPH_TMR0_2,
	
	.TMR0_CH = TMR0_CH_A,
	
	.TMRx_CHx_Int_Mode = TMR0_INT_CMP_A,
	
	.stcTmr0Init = {
		.u32ClockSrc = TMR0_CLK_SRC_XTAL32,
		.u32ClockDiv = TMR0_CLK_DIV16,
		.u32Func = TMR0_FUNC_CMP,
		.u16CompareValue = (uint16_t)(XTAL32_VALUE / 16U / 2U - 1U),
	},
	.stcIrqSignConfig = {
		.enIntSrc    = INT_SRC_TMR0_2_CMP_A,	//中断源选择
    .enIRQn      = INT009_IRQn,						//分配中断号
    .pfnCallback = TMR02_A_CompareIrqCallback,	//中断回调函数
	},
	.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,
};

Tmr0x_Chx_cfgInfoTypedef Tmr02ChB = {
	.TMR_UNITx = CM_TMR0_2,
	
	.TMR0_CLK = FCG2_PERIPH_TMR0_2,
	
	.TMR0_CH = TMR0_CH_B,
	
	.TMRx_CHx_Int_Mode = TMR0_INT_CMP_B,
	
	.stcTmr0Init = {
		.u32ClockSrc = TMR0_CLK_SRC_XTAL32,
		.u32ClockDiv = TMR0_CLK_DIV16,
		.u32Func = TMR0_FUNC_CMP,
		.u16CompareValue = (uint16_t)(XTAL32_VALUE / 16U / 2U - 1U),
	},
	.stcIrqSignConfig = {
		.enIntSrc    = INT_SRC_TMR0_2_CMP_B,	//中断源选择
    .enIRQn      = INT010_IRQn,						//分配中断号
    .pfnCallback = TMR02_B_CompareIrqCallback,	//中断回调函数
	},
	.IRQ_Prio = DDL_IRQ_PRIO_DEFAULT,
};


/**
 * @brief  Configure TMR0.
 * @note   In asynchronous clock, If you want to write a TMR0 register, you need to wait for
 *         at least 3 asynchronous clock cycles after the last write operation!
 * @param  None
 * @retval None
 */
void BSP_TMR0_Config(Tmr0x_Chx_cfgInfoTypedef * Tmr0x_Chx_cfgInfo)
{
    /* Enable timer0 clock */
    FCG_Fcg2PeriphClockCmd(Tmr0x_Chx_cfgInfo->TMR0_CLK, ENABLE);

    (void)TMR0_Init(Tmr0x_Chx_cfgInfo->TMR_UNITx, Tmr0x_Chx_cfgInfo->TMR0_CH, &Tmr0x_Chx_cfgInfo->stcTmr0Init);
    DDL_DelayMS(1U);
	
    TMR0_IntCmd(Tmr0x_Chx_cfgInfo->TMR_UNITx, Tmr0x_Chx_cfgInfo->TMRx_CHx_Int_Mode, ENABLE);
    DDL_DelayMS(1U);

    /* Interrupt configuration */
    (void)INTC_IrqSignIn(&Tmr0x_Chx_cfgInfo->stcIrqSignConfig);
    NVIC_ClearPendingIRQ(Tmr0x_Chx_cfgInfo->stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(Tmr0x_Chx_cfgInfo->stcIrqSignConfig.enIRQn, Tmr0x_Chx_cfgInfo->IRQ_Prio);
    NVIC_EnableIRQ(Tmr0x_Chx_cfgInfo->stcIrqSignConfig.enIRQn);
	
		DDL_DelayMS(1U);
}

void BSP_TMR0_Start(Tmr0x_Chx_cfgInfoTypedef * Tmr0x_Chx_cfgInfo)
{
	int i;
	for(i = 0;i<5;i++){
		TMR0_Start(Tmr0x_Chx_cfgInfo->TMR_UNITx, Tmr0x_Chx_cfgInfo->TMR0_CH);
		DDL_DelayMS(10U);	//<<测试时发现在此处加上适当延时等待定时器启动才行
	}
}
#else
void BSP_TMR0_Config_For_UsartTimeOut(Usart_cfgInfo_Typedef *Usart_cfgInfo)
{
	stc_tmr0_init_t stcTmr0Init;
	
	/* Enable timer0 clock */
	if((Usart_cfgInfo->Usart_Unit == CM_USART1)||(Usart_cfgInfo->Usart_Unit == CM_USART2)){
		FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR0_1,ENABLE);
	}else if((Usart_cfgInfo->Usart_Unit == CM_USART5)||(Usart_cfgInfo->Usart_Unit == CM_USART6)){
		FCG_Fcg2PeriphClockCmd(FCG2_PERIPH_TMR0_2,ENABLE);
	}
	
	stcTmr0Init.u32ClockSrc = TMR0_CLK_SRC_XTAL32,
	stcTmr0Init.u32ClockDiv = TMR0_CLK_DIV16,
	stcTmr0Init.u32Func = TMR0_FUNC_CMP,
	stcTmr0Init.u16CompareValue = Usart_cfgInfo->usart_rx_timeoout_time,
	
	(void)TMR0_Init(Usart_cfgInfo->TimerBase, Usart_cfgInfo->TimerChannel, &stcTmr0Init);
	DDL_DelayMS(1U);

	/* Config timer0 hardware trigger */
	TMR0_HWClearCondCmd(Usart_cfgInfo->TimerBase, Usart_cfgInfo->TimerChannel, ENABLE);
	TMR0_HWStartCondCmd(Usart_cfgInfo->TimerBase, Usart_cfgInfo->TimerChannel, ENABLE);
	
	DDL_DelayMS(1U);
}
#endif

