#include "bsp_extint_key.h"
#include "led.h"

/**
 * @brief  KEY1 External interrupt Ch.0 callback function
 * @param  None
 * @retval None
 */
static void EXTINT_KEY1_IrqCallback(void)
{
    if (SET == EXTINT_GetExtIntStatus(KEY1_EXTINT_CH)) {
        while (PIN_RESET == GPIO_ReadInputPins(KEY1_PORT, KEY1_PIN)) {
            ;
        }
        EXTINT_ClearExtIntStatus(KEY1_EXTINT_CH);
    }
}

/**
 * @brief  KEY2 External interrupt Ch.0 callback function
 * @param  None
 * @retval None
 */
static void EXTINT_KEY2_IrqCallback(void)
{
    if (SET == EXTINT_GetExtIntStatus(KEY2_EXTINT_CH)) {
        while (PIN_RESET == GPIO_ReadInputPins(KEY2_PORT, KEY2_PIN)) {
            ;
        }
        EXTINT_ClearExtIntStatus(KEY2_EXTINT_CH);
    }
}

/**
 * @brief  KEY3 External interrupt Ch.0 callback function
 * @param  None
 * @retval None
 */
static void EXTINT_KEY3_IrqCallback(void)
{
    if (SET == EXTINT_GetExtIntStatus(KEY3_EXTINT_CH)) {
        while (PIN_RESET == GPIO_ReadInputPins(KEY3_PORT, KEY3_PIN)) {
            ;
        }
        EXTINT_ClearExtIntStatus(KEY3_EXTINT_CH);
    }
}

/**
 * @brief  KEY4 External interrupt Ch.0 callback function
 * @param  None
 * @retval None
 */
static void EXTINT_KEY4_IrqCallback(void)
{
    if (SET == EXTINT_GetExtIntStatus(KEY4_EXTINT_CH)) {
        while (PIN_RESET == GPIO_ReadInputPins(KEY4_PORT, KEY4_PIN)) {
            ;
        }
        EXTINT_ClearExtIntStatus(KEY4_EXTINT_CH);
    }
}

/**
 * @brief  KEY5 External interrupt Ch.0 callback function
 * @param  None
 * @retval None
 */
static void EXTINT_KEY5_IrqCallback(void)
{
    if (SET == EXTINT_GetExtIntStatus(KEY5_EXTINT_CH)) {
        while (PIN_RESET == GPIO_ReadInputPins(KEY5_PORT, KEY5_PIN)) {
            ;
        }
        EXTINT_ClearExtIntStatus(KEY5_EXTINT_CH);
    }
}

/**
 * @brief  KEY6 External interrupt Ch.0 callback function
 * @param  None
 * @retval None
 */
static void EXTINT_KEY6_IrqCallback(void)
{
    if (SET == EXTINT_GetExtIntStatus(KEY6_EXTINT_CH)) {
        while (PIN_RESET == GPIO_ReadInputPins(KEY6_PORT, KEY6_PIN)) {
            ;
        }
        EXTINT_ClearExtIntStatus(KEY6_EXTINT_CH);
    }
}

/**
 * @brief  KEY1 initialize
 * @param  None
 * @retval None
 */
void KEY1_Init(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(KEY1_PORT, KEY1_PIN, &stcGpioInit);

    /* ExtInt config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter      = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV8;
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;//下降沿触发
    (void)EXTINT_Init(KEY1_EXTINT_CH, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY1_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY1_INT_IRQn;
    stcIrqSignConfig.pfnCallback = &EXTINT_KEY1_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, KEY1_INT_PRIO);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

/**
 * @brief  KEY2 initialize
 * @param  None
 * @retval None
 */
void KEY2_Init(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(KEY2_PORT, KEY2_PIN, &stcGpioInit);

    /* ExtInt config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter      = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV8;
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;//下降沿触发
    (void)EXTINT_Init(KEY2_EXTINT_CH, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY2_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY2_INT_IRQn;
    stcIrqSignConfig.pfnCallback = &EXTINT_KEY2_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, KEY2_INT_PRIO);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

/**
 * @brief  KEY3 initialize
 * @param  None
 * @retval None
 */
void KEY3_Init(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(KEY3_PORT, KEY3_PIN, &stcGpioInit);

    /* ExtInt config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter      = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV8;
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;//下降沿触发
    (void)EXTINT_Init(KEY3_EXTINT_CH, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY3_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY3_INT_IRQn;
    stcIrqSignConfig.pfnCallback = &EXTINT_KEY3_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, KEY3_INT_PRIO);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

/**
 * @brief  KEY4 initialize
 * @param  None
 * @retval None
 */
void KEY4_Init(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(KEY4_PORT, KEY4_PIN, &stcGpioInit);

    /* ExtInt config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter      = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV8;
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;//下降沿触发
    (void)EXTINT_Init(KEY4_EXTINT_CH, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY4_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY4_INT_IRQn;
    stcIrqSignConfig.pfnCallback = &EXTINT_KEY4_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, KEY4_INT_PRIO);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

/**
 * @brief  KEY5 initialize
 * @param  None
 * @retval None
 */
void KEY5_Init(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(KEY5_PORT, KEY5_PIN, &stcGpioInit);

    /* ExtInt config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter      = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV8;
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;//下降沿触发
    (void)EXTINT_Init(KEY5_EXTINT_CH, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY5_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY5_INT_IRQn;
    stcIrqSignConfig.pfnCallback = &EXTINT_KEY5_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, KEY5_INT_PRIO);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

/**
 * @brief  KEY6 initialize
 * @param  None
 * @retval None
 */
void KEY6_Init(void)
{
    stc_extint_init_t stcExtIntInit;
    stc_irq_signin_config_t stcIrqSignConfig;
    stc_gpio_init_t stcGpioInit;

    /* GPIO config */
    (void)GPIO_StructInit(&stcGpioInit);
    stcGpioInit.u16ExtInt = PIN_EXTINT_ON;
    stcGpioInit.u16PullUp = PIN_PU_ON;
    (void)GPIO_Init(KEY6_PORT, KEY6_PIN, &stcGpioInit);

    /* ExtInt config */
    (void)EXTINT_StructInit(&stcExtIntInit);
    stcExtIntInit.u32Filter      = EXTINT_FILTER_ON;
    stcExtIntInit.u32FilterClock = EXTINT_FCLK_DIV8;
    stcExtIntInit.u32Edge = EXTINT_TRIG_FALLING;				//下降沿触发
    (void)EXTINT_Init(KEY6_EXTINT_CH, &stcExtIntInit);

    /* IRQ sign-in */
    stcIrqSignConfig.enIntSrc = KEY2_INT_SRC;
    stcIrqSignConfig.enIRQn   = KEY2_INT_IRQn;
    stcIrqSignConfig.pfnCallback = &EXTINT_KEY6_IrqCallback;
    (void)INTC_IrqSignIn(&stcIrqSignConfig);

    /* NVIC config */
    NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
    NVIC_SetPriority(stcIrqSignConfig.enIRQn, KEY6_INT_PRIO);
    NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
}

