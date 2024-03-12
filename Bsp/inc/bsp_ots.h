#ifndef BSP_I2C_H
#define BSP_I2C_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "hc32_ll_ots.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll.h"

extern float m_f32Temperature;
extern void OtsInitConfig(void);
extern void Get_Temprature(void);
#ifdef __cplusplus
}
#endif
#endif
	
	
