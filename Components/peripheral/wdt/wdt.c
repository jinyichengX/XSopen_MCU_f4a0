#include "wdt.h"

//用户定义WDT对象
wdt_HandlerType wdt1 = {
	.WDTx = CM_WDT,																				//用户不可配置，因为看门狗唯一
	.wdt_workmode = wdt_tri_reset,												//用户可配置
	.stcWdtInit = {																				//用户可配置
		WDT_CNT_PERIOD16384,
		WDT_CLK_DIV512,
		WDT_RANGE_0TO25PCT,
		WDT_LPM_CNT_STOP,
		WDT_EXP_TYPE_RST
	},
	.fptr_wdtInitHandler = WDT_Config, 										//用户不可配置
	.fptr_wdtOverFlowTriResetDetcHandler = WDT_ResetDetc,	//用户不可配置
	.fptr_wdtFeedHandler = WDT_Feed,											//用户不可配置
	.fptr_wdtIRQHandler = NULL														//用户不可配置
};

