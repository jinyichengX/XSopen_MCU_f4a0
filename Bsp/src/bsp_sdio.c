#include "bsp_sdio.h"
#include "bsp_uart.h"
stc_sd_handle_t SdHandle;
uint8_t u8WriteBlocks[SD_CARD_BLK_SIZE * SD_CARD_BLK_NUM] = {1,2,3,4,5,6,11,22,33,44,55,4,3,5,3,0x33,0xeb,0x90,0x89,0x99};
uint8_t u8ReadBlocks[SD_CARD_BLK_SIZE * SD_CARD_BLK_NUM];
	
#if SD_TRANS_MD != SD_TRANS_MD_POLLING
__IO static uint8_t u8TxCompleteFlag = 0U, u8RxCompleteFlag = 0U, u8TxRxErrorFlag = 0U;
#endif
	
#if SD_TRANS_MD != SD_TRANS_MD_POLLING
/**
 * @brief  SDIOC transfer complete interrupt callback function.
 * @param  None
 * @retval None
 */
static void SdCard_TransCompleteIrqCallback(void)
{
    SD_IRQHandler(&SdHandle);
}

/**
 * @brief  SDIOC TX complete callback function.
 * @param  None
 * @retval None
 */
void SD_TxCompleteCallback(stc_sd_handle_t *handle)
{
    u8TxCompleteFlag = 1U;
}

/**
 * @brief  SDIOC RX complete callback function.
 * @param  None
 * @retval None
 */
void SD_RxCompleteCallback(stc_sd_handle_t *handle)
{
    u8RxCompleteFlag = 1U;
}

/**
 * @brief  SDIOC error callback function.
 * @param  None
 * @retval None
 */
void SD_ErrorCallback(stc_sd_handle_t *handle)
{
    u8TxRxErrorFlag = 1U;
}
#endif
/**
 * @brief  Get SD card insert state.
 * @param  None
 * @retval An @ref en_flag_status_t enumeration type value.
 */
static en_flag_status_t SdCard_GetInsertState(void)
{
    en_flag_status_t enFlagSta = SET;

    /* Check SD card detect pin */
    if (0U != GPIO_ReadInputPins(SDIOC_CD_PORT, SDIOC_CD_PIN)) {
        enFlagSta = RESET;
    }

    return enFlagSta;
}
#if SD_TRANS_MD == SD_TRANS_MD_DMA
/**
 * @brief  Initializes the SD DMA.
 * @param  None
 * @retval None
 */
static void SdCard_DMAInit(void)
{
    stc_dma_init_t stcDmaInit;

    /* Enable DMA and AOS clock */
    FCG_Fcg0PeriphClockCmd(SDIOC_DMA_CLK, ENABLE);

    (void)DMA_StructInit(&stcDmaInit);
    /* Configure SD DMA Transfer */
    stcDmaInit.u32IntEn         = DMA_INT_DISABLE;
    stcDmaInit.u32DataWidth     = DMA_DATAWIDTH_32BIT;
    stcDmaInit.u32SrcAddrInc    = DMA_SRC_ADDR_INC;
    stcDmaInit.u32DestAddrInc   = DMA_DEST_ADDR_FIX;
    if (LL_OK != DMA_Init(SDIOC_DMA_UNIT, SDIOC_DMA_TX_CH, &stcDmaInit)) {
        for (;;) {
        }
    }
    /* Configure SD DMA Receive */
    stcDmaInit.u32SrcAddrInc    = DMA_SRC_ADDR_FIX;
    stcDmaInit.u32DestAddrInc   = DMA_DEST_ADDR_INC;
    if (LL_OK != DMA_Init(SDIOC_DMA_UNIT, SDIOC_DMA_RX_CH, &stcDmaInit)) {
        for (;;)
        {}
    }

    AOS_SetTriggerEventSrc(SDIOC_DMA_TX_TRIG_CH, SDIOC_DMA_TX_TRIG_SRC);
    AOS_SetTriggerEventSrc(SDIOC_DMA_RX_TRIG_CH, SDIOC_DMA_RX_TRIG_SRC);
    /* Enable DMA */
    DMA_Cmd(SDIOC_DMA_UNIT, ENABLE);
}
#endif

/**
 * @brief  SD card configuration.
 * @param  None
 * @retval None
 */
uint32_t SdCard_Config(void)
{		
		uint32_t res = 0;
		stc_gpio_init_t stc_gpio_init;
		(void)GPIO_StructInit(&stc_gpio_init);

		stc_gpio_init.u16PinDir = PIN_DIR_IN;
		stc_gpio_init.u16PinState = PIN_STAT_SET;
#if SD_TRANS_MD != SD_TRANS_MD_POLLING
    stc_irq_signin_config_t stcIrqSignConfig;

    /* Interrupt configuration */
    stcIrqSignConfig.enIntSrc    = SIDOC_SD_INT_SRC;
    stcIrqSignConfig.enIRQn      = SIDOC_SD_IRQ;
    stcIrqSignConfig.pfnCallback = &SdCard_TransCompleteIrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
#endif

    /* Enable SDIOC clock */
    FCG_Fcg1PeriphClockCmd(SDIOC_SD_CLK, ENABLE);
    /* SDIOC pins configuration */
		GPIO_Init(SDIOC_CD_PORT,SDIOC_CD_PIN, &stc_gpio_init);
    GPIO_SetFunc(SDIOC_CK_PORT,  SDIOC_CK_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_CMD_PORT, SDIOC_CMD_PIN, GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D0_PORT,  SDIOC_D0_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D1_PORT,  SDIOC_D1_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D2_PORT,  SDIOC_D2_PIN,  GPIO_FUNC_9);
    GPIO_SetFunc(SDIOC_D3_PORT,  SDIOC_D3_PIN,  GPIO_FUNC_9);

    /* Configure structure initialization */
    SdHandle.SDIOCx                     = SDIOC_SD_UINT;
    SdHandle.stcSdiocInit.u32Mode       = SDIOC_MD_SD;
    SdHandle.stcSdiocInit.u8CardDetect  = SDIOC_CARD_DETECT_CD_PIN_LVL;
    SdHandle.stcSdiocInit.u8SpeedMode   = SDIOC_SPEED_MD_HIGH;
    SdHandle.stcSdiocInit.u8BusWidth    = SDIOC_BUS_WIDTH_4BIT;
    SdHandle.stcSdiocInit.u16ClockDiv   = SDIOC_CLK_DIV2;
#if SD_TRANS_MD == SD_TRANS_MD_DMA
    SdCard_DMAInit();
    SdHandle.DMAx      = SDIOC_DMA_UNIT;
    SdHandle.u8DmaTxCh = SDIOC_DMA_TX_CH;
    SdHandle.u8DmaRxCh = SDIOC_DMA_RX_CH;
#else
    SdHandle.DMAx    = NULL;
#endif

    /* Reset and init SDIOC */
    if (LL_OK != SDIOC_SWReset(SdHandle.SDIOCx, SDIOC_SW_RST_ALL)) {
				printf("Reset SDIOC failed!\r\n");
				res = 1;
				return res;
    } 
		else if (SET != SdCard_GetInsertState()) {
				printf("No SD card insertion!\r\n");
				res = 1;
				return res;
    } 
		else if (LL_OK != SD_Init(&SdHandle)) {
				printf("SD card initialize failed!\r\n");
				res = 1;
				return res;
    } else {
    }
		res = LL_OK;
		return res;
}

#if !USE_SD_FOR_FATFS
int32_t SD_Write(stc_sd_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCount,uint8_t *pu8Data, uint32_t u32Timeout)
{
	int32_t i32Ret;
	i32Ret = SD_WriteBlocks(handle, u32BlockAddr, u16BlockCount, pu8Data, u32Timeout);
	
	return i32Ret;
}

int32_t SD_Read(stc_sd_handle_t *handle, uint32_t u32BlockAddr, uint16_t u16BlockCount,uint8_t *pu8Data, uint32_t u32Timeout)
{
	int32_t i32Ret;
	i32Ret = SD_ReadBlocks(handle, u32BlockAddr, u16BlockCount, pu8Data, u32Timeout);
	return i32Ret;
}
int32_t SdCard_Erase(void)
{
    uint32_t i;
    int32_t i32Ret = LL_OK;

    /* Initialize read/write blocks */
    (void)memset(u8ReadBlocks, 0x20, (SD_CARD_BLK_SIZE * SD_CARD_BLK_NUM));
    /* Erase SD card */
    if (LL_OK != SD_Erase(&SdHandle, 0UL, SD_CARD_BLK_NUM)) {
        i32Ret = LL_ERR;
    }

    /* Read SD card */
#if SD_TRANS_MD == SD_TRANS_MD_POLLING
    if (LL_OK != SD_ReadBlocks(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8ReadBlocks, 2000UL)) {
        i32Ret = LL_ERR;
    }
#elif SD_TRANS_MD == SD_TRANS_MD_INT
    u8RxCompleteFlag = 0U;
    if (LL_OK != SD_ReadBlocks_INT(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8ReadBlocks)) {
        i32Ret = LL_ERR;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCompleteFlag) && (0U == u8TxRxErrorFlag)) {
    }
#else
    u8RxCompleteFlag = 0U;
    if (LL_OK != SD_ReadBlocks_DMA(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8ReadBlocks)) {
        i32Ret = LL_ERR;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCompleteFlag) && (0U == u8TxRxErrorFlag)) {
    }
#endif

    /* Check whether data value is OxFF or 0x00 after erase SD card */
    for (i = 0UL; i < (SD_CARD_BLK_SIZE * SD_CARD_BLK_NUM); i++) {
        if ((0x00U != u8ReadBlocks[i]) && (0xFFU != u8ReadBlocks[i])) {
            i32Ret = LL_ERR;
            break;
        }
    }
#if SD_TRANS_MD != SD_TRANS_MD_POLLING
    if (0U != u8TxRxErrorFlag) {
        i32Ret = LL_ERR;
    }
#endif
    if (LL_OK != i32Ret) {
        DDL_Printf("SD card erase failed!\r\n");
    }

    return i32Ret;
}
/* ²âÊÔÓÃ */
int32_t SdCard_RdWrMultiBlockTest(void)
{
    int32_t i32Ret = LL_OK;

    /* Write SD card */
#if SD_TRANS_MD == SD_TRANS_MD_POLLING
    if (LL_OK != SD_WriteBlocks(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8WriteBlocks, 2000U)) {
        i32Ret = LL_ERR;
    }
#elif SD_TRANS_MD == SD_TRANS_MD_INT
    u8TxCompleteFlag = 0U;
    if (LL_OK != SD_WriteBlocks_INT(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8WriteBlocks)) {
        i32Ret = LL_ERR;
    }
    /* Wait for transfer completed */
    while ((0U == u8TxCompleteFlag) && (0U == u8TxRxErrorFlag)) {
    }
#else
    u8TxCompleteFlag = 0U;
    if (LL_OK != SD_WriteBlocks_DMA(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8WriteBlocks)) {
        i32Ret = LL_ERR;
    }
    /* Wait for transfer completed */
    while ((0U == u8TxCompleteFlag) && (0U == u8TxRxErrorFlag)) {
    }
#endif

    /* Read SD card */
#if SD_TRANS_MD == SD_TRANS_MD_POLLING
    if (LL_OK != SD_ReadBlocks(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8ReadBlocks, 2000UL)) {
        i32Ret = LL_ERR;
    }
#elif SD_TRANS_MD == SD_TRANS_MD_INT
    u8RxCompleteFlag = 0U;
    if (LL_OK != SD_ReadBlocks_INT(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8ReadBlocks)) {
        i32Ret = LL_ERR;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCompleteFlag) && (0U == u8TxRxErrorFlag)) {
    }
#else
    u8RxCompleteFlag = 0U;
    if (LL_OK != SD_ReadBlocks_DMA(&SdHandle, 0UL, SD_CARD_BLK_NUM, (uint8_t *)u8ReadBlocks)) {
        i32Ret = LL_ERR;
    }
    /* Wait for transfer completed */
    while ((0U == u8RxCompleteFlag) && (0U == u8TxRxErrorFlag)) {
    }
#endif

    /* Check data value */
    if (0 != memcmp(u8WriteBlocks, u8ReadBlocks, (SD_CARD_BLK_SIZE * SD_CARD_BLK_NUM))) {
        i32Ret = LL_ERR;
    }
#if SD_TRANS_MD != SD_TRANS_MD_POLLING
    if (0U != u8TxRxErrorFlag) {
        i32Ret = LL_ERR;
    }
#endif
    if (LL_OK != i32Ret) {
        DDL_Printf("SD card multi-block read/write failed!\r\n");
    }

    return i32Ret;
}
#endif
#if USE_SD_FOR_FATFS
/**
* @brief Initializes a Drive
* @param lun: Not used
* @retval DSTATUS: Operation status
*/
DRESULT SD_Initialize(BYTE lun)
{
	DRESULT res = RES_ERROR;
	
	/* Initialize SD card */
	if (RES_OK == (DRESULT)SdCard_Config())
	{
			res = RES_OK;
	}
		
	return res;
}
/**
 ******************************************************************************
* @brief Writes Sector(s)
* @param lun: Not used
* @param buff: Pointer to data to be written
* @param sector: Sector address (LBA)
* @param count: Number of sectors to write (1..128)
* @retval DRESULT: Operation result
*******************************************************************************/
DRESULT SD_Write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = RES_ERROR;
	
	if (RES_OK == SD_WriteBlocks(&SdHandle, (uint32_t)sector, (uint16_t)count, (uint8_t *)buff,
			SD_RW_TIMEOUT_TIME))
	{
		res = RES_OK;
	}
	
	return res;
}
/**
* @brief Reads Sector(s)
* @param lun: Not used
* @param buff: Pointer to data buffer used to store read data
* @param sector: Sector address (LBA)
* @param count: Number of sectors to read (1..128)
* @retval DRESULT: Operation result
*/
DRESULT SD_Read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
	DRESULT res = RES_ERROR;
	if (RES_OK == SD_ReadBlocks(&SdHandle, (uint32_t)sector, (uint16_t)count, (uint8_t *)buff,
	SD_RW_TIMEOUT_TIME))
	{
//		/* Wait until the read operation is finished */
//		while (RES_OK != SDCard_GetCardTransState())
//		{
//		}
		res = RES_OK;
	}
	return res;
}
/**
* @brief I/O control operation
* @param lun: Not used
* @param cmd: Control code
* @param buff: Pointer to buffer used to send/receive data
* @retval DRESULT: Operation result
*/
DRESULT SD_Ioctl(BYTE lun, BYTE cmd, void *buff)
{
	DRESULT res;
	stc_sd_card_info_t stcCardInfo;
	//if (0U != (SdStat & (DSTATUS)STA_NOINIT))
	if (0)
	{
		res = RES_NOTRDY;
	}
	else
	{
		switch (cmd)
		{
			/* Make sure that no pending write process */
			case CTRL_SYNC:
				res = RES_OK;
				break;
			/* Get number of sectors on the disk (DWORD) */
			case GET_SECTOR_COUNT :
				(void)SD_GetCardInfo(&SdHandle, &stcCardInfo);
				*(DWORD*)buff = stcCardInfo.u32BlockNum;
				res = RES_OK;
				break;
			/* Get R/W sector size (WORD) */
			case GET_SECTOR_SIZE:
				(void)SD_GetCardInfo(&SdHandle, &stcCardInfo);
				*(WORD*)buff = (uint16_t)stcCardInfo.u32LogBlockSize;
				res = RES_OK;
				break;
			/* Get erase block size in unit of sector (DWORD) */
			case GET_BLOCK_SIZE:
				(void)SD_GetCardInfo(&SdHandle, &stcCardInfo);
				*(DWORD*)buff = stcCardInfo.u32LogBlockSize / SD_CARD_BLK_SIZE;
				res = RES_OK;
				break;
			default:
				res = RES_PARERR;
				break;
		}
	}
	return res;
}
#endif

