#include "bsp_wdt.h"

/**
 * @brief  Get WDT flag status.
 * @param  [in] u32Flag                 WDT flag type
 *         This parameter can be one or any combination of the following values:
 *           @arg WDT_FLAG_UDF:         Count underflow flag
 *           @arg WDT_FLAG_REFRESH:     Refresh error flag
 *           @arg WDT_FLAG_ALL:         All of the above
 * @retval An @ref en_flag_status_t enumeration type value.
 */
static en_flag_status_t Usr_WDT_GetStatus(CM_WDT_TypeDef * WDTx,uint32_t u32Flag)
{
    en_flag_status_t enFlagSta = RESET;

    if (0UL != (WDT_REG_GET_BIT(WDTx->SR, u32Flag))) {
        enFlagSta = SET;
    }

    return enFlagSta;
}

/**
 * @brief  Clear WDT flag.
 * @param  [in] u32Flag                 WDT flag type
 *         This parameter can be one or any combination of the following values:
 *           @arg WDT_FLAG_UDF:         Count underflow flag
 *           @arg WDT_FLAG_REFRESH:     Refresh error flag
 *           @arg WDT_FLAG_ALL:         All of the above
 * @retval int32_t:
 *           - LL_OK: Clear flag success
 *           - LL_ERR_TIMEOUT: Clear flag timeout
 */
static int32_t Usr_WDT_ClearStatus(CM_WDT_TypeDef * WDTx,uint32_t u32Flag)
{
    __IO uint32_t u32Count;
    int32_t i32Ret = LL_OK;

    WDT_REG_CLEAR_BIT(WDTx->SR, u32Flag);
    /* Waiting for FLAG bit clear */
    u32Count = 5 * (HCLK_VALUE / 20000UL);
    while (0UL != READ_REG32_BIT(WDTx->SR, u32Flag)) {
        if (0UL == u32Count) {
            i32Ret = LL_ERR_TIMEOUT;
            break;
        }
        u32Count--;
    }

    return i32Ret;
}

/**
 *******************************************************************************
 * @brief  WDT configuration.
 * @param  None
 * @retval None
 * *****************************************************************************/
void WDT_Config(wdt_HandlerType * wdt_handler)
{
    (void)WDT_Init(&wdt_handler->stcWdtInit);
}

/**
 ******************************************************************************
 * @brief  Clear WDT overflow flag 
 * @param  None
 * @retval None
 ******************************************************************************/
void WDT_ResetDetc(wdt_HandlerType * wdt_handler,uint32_t overflow_flag)
{
	/* Get RMU information */
	if (SET == RMU_GetStatus(RMU_FLAG_WDT)) {
//			u32ResetSrc = RMU_FLAG_WDT;
//			BSP_LED_On(LED_RED);
		if(SET == Usr_WDT_GetStatus(wdt_handler->WDTx,overflow_flag)){
			Usr_WDT_ClearStatus(wdt_handler->WDTx,overflow_flag);
			//用户可添加打印看门狗溢出复位异常信息
			;
		}
	}
	RMU_ClearStatus();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}

/**
 ******************************************************************************
 * @brief  Feed WDT
 * @param  None
 * @retval None
 ******************************************************************************/
void WDT_Feed(wdt_HandlerType * wdt_handler)
{
		WDT_REG_WRITE(&wdt_handler->WDTx->RR,0x0123UL);
		WDT_REG_WRITE(&wdt_handler->WDTx->RR,0x3210UL);
	
		while(0x1 == WDT_REG_VAL(wdt_handler->WDTx->RR)){
			break;
		}
}
