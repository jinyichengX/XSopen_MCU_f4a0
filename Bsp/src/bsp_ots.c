#include "bsp_ots.h"
float m_f32Temperature;

void OtsInitConfig(void)
{
    stc_ots_init_t stcOTSInit;
	
    /* Enable OTS peripheral clock */
    FCG_Fcg3PeriphClockCmd(FCG3_PERIPH_OTS, ENABLE);
	
    (void)OTS_StructInit(&stcOTSInit);
    stcOTSInit.u16ClockSrc = OTS_CLK_XTAL;
    stcOTSInit.stcParaCond.u16ClockFreq = 8U;
    /* Initialize OTS */
    (void)OTS_Init(&stcOTSInit);
}

void Get_Temprature(void)
{	 
	OTS_Start();
	m_f32Temperature = OTS_CalculateTemp();
}
