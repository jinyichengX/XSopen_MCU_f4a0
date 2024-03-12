#include "bsp_dac.h"

DAC_cfgInfo_Typedef dac_obj1 = {
	.Unit = CM_DAC1,
	.channel = DAC_CH1,
	.work_mode = vref,
	.DAC_UNIT_PORT = GPIO_PORT_A,
	.DAC_UNIT_PIN = GPIO_PIN_04,
};

/**
 * @brief    Enable DAC peripheral clock
 * @param    [in] enUnit  The selected DAC unit
 * @retval   None
 */
static void DAC_PClkEnable(CM_DAC_TypeDef *  Unit)
{
    uint32_t u32PClk;
	
		if(Unit == CM_DAC1) u32PClk = PWC_FCG3_DAC1;
		else if(Unit == CM_DAC2) u32PClk = PWC_FCG3_DAC2;

    /* Enable DAC peripheral clock. */
    FCG_Fcg3PeriphClockCmd(u32PClk, ENABLE);
}
/**
 * @brief    Start single DAC conversions
 * @param    [in] pDac       A pointer of DAC handler
 * @retval   None
 */
void DAC_StartConversion(DAC_cfgInfo_Typedef * dac,float voltage)
{
	uint16_t dac_vref_data = 0;
	
	dac_vref_data = (uint16_t)(float)(((float)4096*voltage)/((float)DAC_VREFH - (float)DAC_VREFL));
    /* Enalbe AMP */
#ifdef SUPPORT_AMP
    (void)DAC_AMPCmd(dac->Unit, dac->channel, ENABLE);
#endif

    (void)DAC_Start(dac->Unit, dac->channel);

#ifdef SUPPORT_AMP
    /* delay 3us before setting data*/
    DDL_DelayMS(1U);
#endif
	
	DAC_SetChData(dac->Unit, dac->channel, dac_vref_data);
}
void DAC_StopConversion(DAC_cfgInfo_Typedef * dac)
{
	DAC_SetChData(dac->Unit, dac->channel, 0);
	
#ifdef SUPPORT_AMP
		/* delay 3us before setting data*/
    DDL_DelayMS(1U);
#endif
	
	(void)DAC_Stop(dac->Unit, dac->channel);
	
#ifdef SUPPORT_AMP
	(void)DAC_AMPCmd(dac->Unit, dac->channel, DISABLE);
#endif
}
void DAC_Config(DAC_cfgInfo_Typedef * dac)
{
	stc_dac_init_t stInit;
	stc_gpio_init_t stcGpioInit;
	
	DAC_PClkEnable(dac->Unit);
	/* Init DAC by default value: source from data register and output enabled*/
	DAC_DeInit(dac->Unit);
	(void)DAC_StructInit(&stInit);
	(void)DAC_Init(dac->Unit, dac->channel, &stInit);
	
	(void)GPIO_StructInit(&stcGpioInit);
	stcGpioInit.u16PinAttr = PIN_ATTR_ANALOG;
	(void)GPIO_Init(dac->DAC_UNIT_PORT, dac->DAC_UNIT_PIN, &stcGpioInit);
}



