#ifndef __BSP_WDT_H__
#define __BSP_WDT_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_wdt.h"
#include "hc32_ll_rmu.h"
#include "hc32_ll.h"

/* WDT_UDF_FLAG definition */
#define WDT_UDF_FLAG	 0x10000

#define WDT_REG_WRITE(ADDR_PTR,PARA) 			*((uint32_t *)ADDR_PTR) = PARA
#define WDT_REG_VAL(ADDR_PTR) 						*((uint32_t *)ADDR_PTR)
#define WDT_REG_CLEAR_BIT(ADDR_PTR,BIT)		*((uint32_t *)ADDR_PTR) &= ((uint32_t)(~((uint32_t)(BIT))))
#define WDT_REG_GET_BIT(ADDR_PTR,BIT)			(*((uint32_t *)ADDR_PTR)) & ((uint32_t)(BIT))
/* WDT work mode enum begin */
typedef enum
{
	wdt_tri_reset  = 1, //<<看门狗溢出触发复位
	wdt_tri_intr  = 2,	//<<看门狗溢出触发中断
}WDT_WorkMode;
/* WDT work mode enum end */

/* WDT configuration info and function handlers struct begin */
typedef struct wdt_stdfunc
{
	CM_WDT_TypeDef * WDTx;
	WDT_WorkMode wdt_workmode;
	stc_wdt_init_t stcWdtInit;
	void (* fptr_wdtInitHandler)(struct wdt_stdfunc *);
	void (* fptr_wdtOverFlowTriResetDetcHandler)(struct wdt_stdfunc *,uint32_t);
	void (* fptr_wdtFeedHandler)(struct wdt_stdfunc *);
	void (* fptr_wdtIRQHandler)(void);
}wdt_HandlerType;
/* WDT configuration info and function handlers struct end */

extern void WDT_Config(wdt_HandlerType *);
extern void WDT_Feed(wdt_HandlerType *);
extern void WDT_ResetDetc(wdt_HandlerType *,uint32_t);
#ifdef __cplusplus
}
#endif
#endif /*__BSP_WDT_H__*/

