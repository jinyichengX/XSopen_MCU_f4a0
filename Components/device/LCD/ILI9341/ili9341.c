#include "ili9341.h"
#define BSP_NT35510_MATCH_ADDR          (0x60UL)
#define BSP_NT35510_MASK_ADDR           (EXMC_SMC_ADDR_MASK_16MB)
#define BSP_NT35510_CHIP                (EXMC_SMC_CHIP3)
#define SMC_MAX_TIMEOUT                 (0x100000UL)
static void LCD_Port_Init(void)
{
    stc_gpio_init_t stcGpioInit;

    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16PinDrv = PIN_HIGH_DRV;

    /* LCD_CS */
    (void)GPIO_Init(BSP_ILI9341_CS_PORT, BSP_ILI9341_CS_PIN, &stcGpioInit);

    /* LCD_WE */
    (void)GPIO_Init(BSP_ILI9341_WE_PORT, BSP_ILI9341_WE_PIN, &stcGpioInit);

    /* LCD_OE */
    (void)GPIO_Init(BSP_ILI9341_OE_PORT, BSP_ILI9341_OE_PIN, &stcGpioInit);
	
		/* LCD_RS */
    (void)GPIO_Init(BSP_ILI9341_RS_PORT, BSP_ILI9341_RS_PIN, &stcGpioInit);

    /* SMC_DATA[0:15] */
    (void)GPIO_Init(BSP_ILI9341_DATA0_PORT,  BSP_ILI9341_DATA0_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA1_PORT,  BSP_ILI9341_DATA1_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA2_PORT,  BSP_ILI9341_DATA2_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA3_PORT,  BSP_ILI9341_DATA3_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA4_PORT,  BSP_ILI9341_DATA4_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA5_PORT,  BSP_ILI9341_DATA5_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA6_PORT,  BSP_ILI9341_DATA6_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA7_PORT,  BSP_ILI9341_DATA7_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA8_PORT,  BSP_ILI9341_DATA8_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA9_PORT,  BSP_ILI9341_DATA9_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA10_PORT, BSP_ILI9341_DATA10_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA11_PORT, BSP_ILI9341_DATA11_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA12_PORT, BSP_ILI9341_DATA12_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA13_PORT, BSP_ILI9341_DATA13_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA14_PORT, BSP_ILI9341_DATA14_PIN, &stcGpioInit);
    (void)GPIO_Init(BSP_ILI9341_DATA15_PORT, BSP_ILI9341_DATA15_PIN, &stcGpioInit);

    /* LCD_DATA[0:15] */
    GPIO_SetFunc(BSP_ILI9341_DATA0_PORT,  BSP_ILI9341_DATA0_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA1_PORT,  BSP_ILI9341_DATA1_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA2_PORT,  BSP_ILI9341_DATA2_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA3_PORT,  BSP_ILI9341_DATA3_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA4_PORT,  BSP_ILI9341_DATA4_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA5_PORT,  BSP_ILI9341_DATA5_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA6_PORT,  BSP_ILI9341_DATA6_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA7_PORT,  BSP_ILI9341_DATA7_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA8_PORT,  BSP_ILI9341_DATA8_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA9_PORT,  BSP_ILI9341_DATA9_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA10_PORT, BSP_ILI9341_DATA10_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA11_PORT, BSP_ILI9341_DATA11_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA12_PORT, BSP_ILI9341_DATA12_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA13_PORT, BSP_ILI9341_DATA13_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA14_PORT, BSP_ILI9341_DATA14_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_DATA15_PORT, BSP_ILI9341_DATA15_PIN, GPIO_FUNC_12);

    GPIO_SetFunc(BSP_ILI9341_CS_PORT, BSP_ILI9341_CS_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_RS_PORT, BSP_ILI9341_RS_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_WE_PORT, BSP_ILI9341_WE_PIN, GPIO_FUNC_12);
    GPIO_SetFunc(BSP_ILI9341_OE_PORT, BSP_ILI9341_OE_PIN, GPIO_FUNC_12);
}

/**
 * @brief  Initializes LCD low level.
 * @param  None
 * @retval int32_t:
 *           - LL_OK:                   Initialize successfully.
 *           - LL_ERR:                  Initialize unsuccessfully.
 */
int32_t LCD_SMC_Init(void)
{
    __IO uint32_t u32To = 0UL;
    int32_t i32Ret = LL_OK;
    stc_exmc_smc_init_t stcSmcInit;
    stc_exmc_smc_chip_config_t stcChipConfig;
    stc_exmc_smc_timing_config_t stcTimingConfig;
    en_flag_status_t enChipStatus = RESET;
    en_flag_status_t enTimingStatus = RESET;

    LCD_Port_Init();

    /* Enable SMC clock */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_SMC, ENABLE);

    /* Enable SMC. */
    EXMC_SMC_Cmd(ENABLE);

    EXMC_SMC_ExitLowPower();

    while (EXMC_SMC_READY != EXMC_SMC_GetStatus()) {
        if (u32To > SMC_MAX_TIMEOUT) {
            i32Ret = LL_ERR;
            break;
        }
        u32To++;
    }

    if (LL_OK == i32Ret) {
        /* Configure SMC width && CS &chip & timing. */
        (void)EXMC_SMC_StructInit(&stcSmcInit);
        stcSmcInit.stcChipConfig.u32AddrMatch = BSP_NT35510_MATCH_ADDR;
        stcSmcInit.stcChipConfig.u32AddrMask  = BSP_NT35510_MASK_ADDR;
        stcSmcInit.stcChipConfig.u32MemoryWidth  = EXMC_SMC_MEMORY_WIDTH_16BIT;
        stcSmcInit.stcChipConfig.u32BAA  = EXMC_SMC_BAA_PORT_DISABLE;
        stcSmcInit.stcChipConfig.u32ADV  = EXMC_SMC_ADV_PORT_DISABLE;
        stcSmcInit.stcChipConfig.u32BLS  = EXMC_SMC_BLS_SYNC_CS;
        stcSmcInit.stcChipConfig.u32ReadBurstLen  = EXMC_SMC_READ_BURST_4BEAT;
        stcSmcInit.stcChipConfig.u32WriteBurstLen  = EXMC_SMC_WRITE_BURST_4BEAT;
        stcSmcInit.stcChipConfig.u32ReadMode  = EXMC_SMC_READ_SYNC;
        stcSmcInit.stcChipConfig.u32WriteMode  = EXMC_SMC_WRITE_SYNC;
        stcSmcInit.stcTimingConfig.u8RC = 8U;
        stcSmcInit.stcTimingConfig.u8WC = 4U;
        stcSmcInit.stcTimingConfig.u8CEOE = 1U;
        stcSmcInit.stcTimingConfig.u8WP = 2U;
        stcSmcInit.stcTimingConfig.u8PC = 4U;
        stcSmcInit.stcTimingConfig.u8TR = 1U;
        (void)EXMC_SMC_Init(BSP_NT35510_CHIP, &stcSmcInit);

        /* Set command: updateregs */
        EXMC_SMC_SetCommand(BSP_NT35510_CHIP, EXMC_SMC_CMD_UPDATEREGS, 0UL, 0UL);

        /* Check timing status */
        u32To = 0UL;
        while ((enChipStatus != SET) || (enTimingStatus != SET)) {
            (void)EXMC_SMC_GetTimingConfig(BSP_NT35510_CHIP, &stcTimingConfig);
            if (0 == memcmp(&stcTimingConfig, &stcSmcInit.stcTimingConfig, sizeof(stcTimingConfig))) {
                enTimingStatus = SET;
            }

            (void)EXMC_SMC_GetChipConfig(BSP_NT35510_CHIP, &stcChipConfig);
            if (0 == memcmp(&stcChipConfig, &stcSmcInit.stcChipConfig, sizeof(stcChipConfig))) {
                enChipStatus = SET;
            }

            if (u32To > SMC_MAX_TIMEOUT) {
                i32Ret = LL_ERR;
                break;
            }
            u32To++;
        }
    }

    return i32Ret;
}

