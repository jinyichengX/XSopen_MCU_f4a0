#ifndef WDT_H
#define WDT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_wdt.h"

extern wdt_HandlerType wdt1;

#define WDT_INIT(WDTX_PTR)					WDTX_PTR.fptr_wdtInitHandler(&WDTX_PTR)
#define WDT_FEED(WDTX_PTR)					WDTX_PTR.fptr_wdtFeedHandler(&WDTX_PTR)
#define IF_WDT_TRI_RESET(WDTX_PTR)	WDTX_PTR.fptr_wdtOverFlowTriResetDetcHandler(&WDTX_PTR,WDT_UDF_FLAG)
#define WDT_START_COUNT(WDTX_PTR)		WDT_FEED(WDTX_PTR)

//启动看门狗时的调用顺序，初始化时调用
/*WDT_INIT(wdt1);
WDT_START_COUNT(wdt1);
IF_WDT_TRI_RESET(wdt1);*/
//喂狗WDT_FEED(WDTX_PTR)时机用户根据实际评估

#endif /* WDT_H */
