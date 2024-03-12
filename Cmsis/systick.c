#include "systick.h"
#include "led.h"
static uint32_t m_u32TickStep = 0;
static uint32_t m_u32TickCount = 0;
//使用FreeRTOS需要将SysTick_Handler注释
//void SysTick_Handler(void)
//{
//	Led1_Toggle;
//}

int32_t SysTick_Init(uint32_t u32Freq)
{
    int32_t i32Ret = LL_ERR;

    if ((0UL != u32Freq) && (u32Freq <= 1000UL)) {
        m_u32TickStep = 1000UL / u32Freq;
        /* Configure the SysTick interrupt */
        if (0UL == SysTick_Config(HCLK_VALUE / u32Freq)) {
            i32Ret = LL_OK;
        }
    }

    return i32Ret;
}

void SysTick_Delay(uint32_t u32Delay)
{
    const uint32_t tickStart = SysTick_GetTick();
    uint32_t tickEnd = u32Delay;
    uint32_t tickMax;

    if (m_u32TickStep != 0UL) {
        tickMax = 0xFFFFFFFFUL / m_u32TickStep * m_u32TickStep;
        /* Add a freq to guarantee minimum wait */
        if ((u32Delay >= tickMax) || ((tickMax - u32Delay) < m_u32TickStep)) {
            tickEnd = tickMax;
        }
        while ((SysTick_GetTick() - tickStart) < tickEnd) {
        }
    }
}

void SysTick_IncTick(void)
{
    m_u32TickCount += m_u32TickStep;
}

uint32_t SysTick_GetTick(void)
{
    return m_u32TickCount;
}

void SysTick_Suspend(void)
{
    /* Disable SysTick Interrupt */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void SysTick_Resume(void)
{
    /* Enable SysTick Interrupt */
    SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}

