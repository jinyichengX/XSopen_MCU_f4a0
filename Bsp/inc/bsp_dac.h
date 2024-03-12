#ifndef __BSP_DAC_H__
#define __BSP_DAC_H__

#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_dac.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll.h"

#define DAC_VREFH 3.3F
#define DAC_VREFL 0

#define SUPPORT_AMP

typedef enum 
{
	sin_wave  = 1,
	vref = 2,
}dac_work_mode;
//#define CM_DAC1                              ((CM_DAC_TypeDef *)CM_DAC1_BASE)
typedef struct
{
	CM_DAC_TypeDef *  Unit;
	uint16_t channel;

	dac_work_mode work_mode;
	
	uint8_t DAC_UNIT_PORT;
	uint16_t DAC_UNIT_PIN;
}DAC_cfgInfo_Typedef;

extern DAC_cfgInfo_Typedef dac_obj1,dac_obj2,dac_obj3,dac_obj4;

extern void DAC_Config(DAC_cfgInfo_Typedef * dac);
extern void DAC_StartConversion(DAC_cfgInfo_Typedef * dac,float voltage);
extern void DAC_StopConversion(DAC_cfgInfo_Typedef * dac);

#ifdef __cplusplus
}
#endif
#endif /* __BSP_DAC_H__ */
