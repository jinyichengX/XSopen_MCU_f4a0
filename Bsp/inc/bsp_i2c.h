#ifndef __BSP_I2C_H__
#define __BSP_I2C_H__

#ifdef __cplusplus
extern "C" {
#endif
	
#include "hc32_ll_i2c.h"
#include "hc32_ll_gpio.h"
#include "hc32_ll_pwc.h"
#include "hc32_ll.h"
#include <stdbool.h>

#include "bsp_gpio.h"

//I2C模拟/硬件配置宏，可修改0/1
#define SOFT_I2C_ENABLE 1

//#define BSP_I2C_TIMEOUT 0x40000U

#if !SOFT_I2C_ENABLE
typedef struct
{	
	uint8_t SCL_Port;
	uint16_t SCL_Pin;
	uint32_t SCL_Func;
	
	uint8_t SDA_Port;
	uint16_t SDA_Pin;
	uint32_t SDA_Func;
}PortPinTypedef;

typedef struct I2c_Port_Pin_InitConfigurationStruct
{
	CM_I2C_TypeDef * CM_I2C_UNITx;
	uint32_t I2C_FCGx;
	PortPinTypedef PortPin;
	
	stc_i2c_init_t stcI2cInit;
	
	bool If_Link_DMA;
}I2Cx_InitCfg_Typedef;

extern int32_t BSP_I2CPortPinInit_and_I2CFuncEn(I2Cx_InitCfg_Typedef *);
extern void BSP_I2C_DeInitialize(I2Cx_InitCfg_Typedef * I2Cx_InitCfg);
extern int32_t BSP_I2C_GetDevStatus(I2Cx_InitCfg_Typedef * I2Cx_InitCfg, uint16_t u16DevAddr);
extern int32_t BSP_I2C_Write(I2Cx_InitCfg_Typedef * I2Cx_InitCfg,uint16_t u8DevAddr, uint16_t u16MemAddr, uint8_t u8RegLen, uint8_t *pu8Data, int32_t u32Size, uint32_t u32TimeOut,uint8_t Page_Len);
extern	int32_t I2C_Mem_Write(uint16_t u8DevAddr, uint16_t u16MemAddr, uint8_t *pu8Data, uint32_t u32Size, uint32_t u32TimeOut);
extern int32_t BSP_I2C_Read(I2Cx_InitCfg_Typedef * I2Cx_InitCfg, uint16_t u16DevAddr, const uint8_t *pu8Reg, uint8_t u8RegLen, uint8_t *pu8Buf, uint32_t u32Len);
#else

typedef struct
{	
	io_HandlerType I2C_SDA;
	io_HandlerType I2C_SCL;
}I2C_InitCfg_Typedef;

extern void i2c_Port_Initialize(I2C_InitCfg_Typedef *);
extern void i2c_Start(I2C_InitCfg_Typedef *);
extern void i2c_Stop(I2C_InitCfg_Typedef *);
extern void i2c_SendByte(I2C_InitCfg_Typedef *,uint8_t _ucByte);
extern uint8_t i2c_ReadByte(I2C_InitCfg_Typedef *);
extern uint8_t i2c_WaitAck(I2C_InitCfg_Typedef *);
extern void i2c_Ack(I2C_InitCfg_Typedef *);
extern void i2c_NAck(I2C_InitCfg_Typedef *);
#endif
#ifdef __cplusplus
}
#endif
#endif /* __BSP_I2C_H__ */

