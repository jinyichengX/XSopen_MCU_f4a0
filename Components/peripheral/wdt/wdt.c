#include "wdt.h"

//�û�����WDT����
wdt_HandlerType wdt1 = {
	.WDTx = CM_WDT,																				//�û��������ã���Ϊ���Ź�Ψһ
	.wdt_workmode = wdt_tri_reset,												//�û�������
	.stcWdtInit = {																				//�û�������
		WDT_CNT_PERIOD16384,
		WDT_CLK_DIV512,
		WDT_RANGE_0TO25PCT,
		WDT_LPM_CNT_STOP,
		WDT_EXP_TYPE_RST
	},
	.fptr_wdtInitHandler = WDT_Config, 										//�û���������
	.fptr_wdtOverFlowTriResetDetcHandler = WDT_ResetDetc,	//�û���������
	.fptr_wdtFeedHandler = WDT_Feed,											//�û���������
	.fptr_wdtIRQHandler = NULL														//�û���������
};

