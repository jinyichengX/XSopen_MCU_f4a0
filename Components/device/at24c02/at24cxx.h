#ifndef __AT24CXX_H__
#define __AT24CXX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_i2c.h"

#if !SOFT_I2C_ENABLE
typedef struct I2C_device_configuration_and_handler_struct
{
	/* I2C module infomation */
	I2Cx_InitCfg_Typedef I2Cx_InitCfg;
	
	 /* I2C device Properties */
	uint32_t u32PageSize;
	uint32_t u32Capacity;
	uint16_t device_ID;
	uint16_t Dev_mem_addr_len;
	
	/* Methods */
	void (*fptr_Delay)(uint32_t);
	int32_t (*fptr_Init)(I2Cx_InitCfg_Typedef *);
	void (*fptr_DeInit)(I2Cx_InitCfg_Typedef *);
	
	int32_t (*fptr_WritePage)(I2Cx_InitCfg_Typedef * I2Cx_InitCfg,uint16_t u8DevAddr, uint16_t u16MemAddr, uint8_t u8RegLen, uint8_t *pu8Data, int32_t u32Size, uint32_t u32TimeOut,uint8_t Page_Len);
	int32_t (*fptr_Read)(I2Cx_InitCfg_Typedef * , uint16_t , const uint8_t *, uint8_t , uint8_t *, uint32_t);
	int32_t (*fptr_GetStatus)(I2Cx_InitCfg_Typedef * I2Cx_InitCfg, uint16_t u16DevAddr);
	
	/* Function paras */
}stc_i2c_device_t;


extern stc_i2c_device_t at24c02;
extern void at24cxx_device_creat(stc_i2c_device_t * stc_i2c_device);
extern void at24cxx_device_write(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *wbuff,int32_t data_size);
extern void at24cxx_device_read(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *rbuff,uint32_t len);
#else

typedef struct I2C_device_configuration_and_handler_struct
{
	/* I2C module infomation */
	I2C_InitCfg_Typedef I2Cx_PortPinCfg;
	
	 /* I2C device Properties */
	uint32_t u32PageSize;
	uint32_t u32Capacity;
	uint16_t device_ID;
	uint16_t Dev_mem_addr_len;
	
	/* Methods */
	void (*fptr_PortPinInit)(I2C_InitCfg_Typedef *);
	void (*fptr_Start)(I2C_InitCfg_Typedef *);
	void (*fptr_Stop)(I2C_InitCfg_Typedef *);
	void (*fptr_SendByte)(I2C_InitCfg_Typedef *,uint8_t _ucByte);
	uint8_t (*fptr_ReadByte)(I2C_InitCfg_Typedef *);
	uint8_t (*fptr_WaitAck)(I2C_InitCfg_Typedef *);
	void (*fptr_Ack)(I2C_InitCfg_Typedef *);
	void (*fptr_Nack)(I2C_InitCfg_Typedef *);
}stc_i2c_device_t;

extern stc_i2c_device_t at24c02;

extern void at24cxx_device_creat(stc_i2c_device_t * stc_i2c_device);
extern int8_t at24cxx_device_write(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *wbuff,int32_t data_size);
extern int8_t at24cxx_device_read(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *rbuff,uint32_t data_size);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __AT24CXX_H__ */

