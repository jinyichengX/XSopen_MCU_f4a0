#ifndef __W25QXX_H__
#define __W25QXX_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "bsp_spi.h"

#include "diskio.h"
#include "ff.h"

#define DUMMY_BYTE 0xFF

#define Q64_CMD_GET_ID						0x9F
#define Q64_CMD_CHIP_ERASE	 			0xC7
#define Q64_CMD_READ							0x03
#define Q64_CMD_WRITE_ENABLE			0x06
#define Q64_CMD_SECTOR_ERASE			0x20
#define Q64_CMD_MINI_BLOCK_ERASE	0x52
#define Q64_CMD_BLOCK_ERASE				0xD8
#define Q64_CMD_PAGE_PROG					0x02
#define Q64_CMD_READ_STATUS1			0x05

#if !SOFT_SPI_ENABLE
typedef struct spi_struct_cfg
{
	/* SPI module infomation */
	spi_cfg_t spi_cfg;
	
	/* SPI device Properties */
	uint16_t Pagesize;
	uint16_t Sectorsize;
	uint16_t MiniBlocksize;
	uint32_t Blocksize;
	uint32_t Capacity;
	uint32_t Device_ID;
	uint32_t Sectornum;
	
	/* Methods  */
	void (*Delay)(uint32_t);
	void (*Init)(spi_cfg_t);
	void (*DeInit)(void);
	void (*Active)(void);
	void (*Inactive)(void);
	int32_t (*Trans_Receive)(const uint8_t *p_Txbuf, const uint8_t *p_Rxbuf,uint32_t t_len,uint32_t u32Timeout);
//	(CM_SPI_TypeDef *SPIx, const void *pvTxBuf, void *pvRxBuf, uint32_t u32Len, uint32_t u32Timeout)
}stc_spi_device_t;
#else
typedef struct SPI_device_configuration_and_handler_struct
{
	/* SPI module infomation */
	SPI_InitCfg_Typedef PortPinCfg;
	
	/* SPI device Properties */
	uint16_t Pagesize;
	uint16_t Sectorsize;
	uint16_t MiniBlocksize;
	uint32_t Blocksize;
	uint32_t Capacity;
	uint32_t Device_ID;
	uint32_t Sectornum;
	/* Methods */
	void (* fptr_InitHandler)(SPI_InitCfg_Typedef *);
	uint8_t (*fptr_WrRdHandler)(SPI_InitCfg_Typedef *,uint8_t);
	void (* fptr_ChipSel)(SPI_InitCfg_Typedef *,en_functional_state_t);
}stc_spi_device_t;

extern stc_spi_device_t w25q64;

extern uint32_t Spi_device_Init(stc_spi_device_t *spi_device);
extern int Spi_device_Write(stc_spi_device_t *spi_device,uint32_t WriteAddr,uint8_t* pBuffer,uint32_t NumByteToWrite);
extern void Spi_device_FullErase(stc_spi_device_t *spi_device);
extern int Spi_device_Read(stc_spi_device_t *spi_device,uint32_t ReadAddr, uint8_t* pBuffer, uint32_t NumByteToRead);
#endif
#ifdef __cplusplus
}
#endif
#endif /* __W25QXX_H__ */

