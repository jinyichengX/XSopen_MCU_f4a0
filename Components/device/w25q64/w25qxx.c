#include "w25qxx.h"

#if !SOFT_SPI_ENABLE
stc_spi_device_t w25q64 = {
	.spi_cfg = {
		/* spi instance and clk */
		.unit = CM_SPI1,
		/* DMA related */
		.linked_dma_unit = CM_DMA1,
		.tx_link_dma_ch = DMA_CH0,
		.dma_tx_trig_ch = AOS_DMA1_0,
		.rx_link_dma_ch = DMA_CH1,
		.dma_rx_trig_ch = AOS_DMA1_1,
		/* spi io */
		.spi_cs = {.port = GPIO_PORT_C,.pin = GPIO_PIN_10,.func = GPIO_FUNC_47},
		.spi_clk = {.port = GPIO_PORT_C,.pin = GPIO_PIN_12,.func = GPIO_FUNC_40},
		.spi_mosi = {.port = GPIO_PORT_B,.pin = GPIO_PIN_05,.func = GPIO_FUNC_41},
		.spi_miso = {.port = GPIO_PORT_B,.pin = GPIO_PIN_07,.func = GPIO_FUNC_42},
	},
	.Pagesize = 256,					//256B
	.Sectorsize = 4096, 			//4K
	.MiniBlocksize = 32*1024,	//32K
	.Blocksize = 64*1024, 		//64K
	.Capacity = 8*1024*1024,	//8M
	.Sectornum = 2048,				//2048 sectors
	
//	.Delay    = DDL_DelayMS,
	 .Init   = BSP_SPI_Init,
//   .DeInit = SPI_DeInit(BSP_SPI_UNIT);
//	.Trans_Receive  = BSP_SPI_WriteRead,
};

//Usr creat W25Q128、W25Q256、W25Q512......
stc_spi_device_t OTHER_DEV = {

};
#else
stc_spi_device_t w25q64 = {
	.PortPinCfg = {
		.SPI_SCK.io_obj.IO_PortSel = GPIO_PORT_C,
		.SPI_SCK.io_obj.IO_PinSel = GPIO_PIN_06,
		.SPI_SCK.io_obj.GpioElec = {.u16PinDir = PIN_DIR_OUT,.u16PinDrv = PIN_HIGH_DRV,},
		
		.SPI_CS.io_obj.IO_PortSel = GPIO_PORT_C,
		.SPI_CS.io_obj.IO_PinSel = GPIO_PIN_07,
		.SPI_CS.io_obj.GpioElec = {.u16PinDir = PIN_DIR_OUT,.u16PinDrv = PIN_HIGH_DRV,},
		
		.SPI_MOSI.io_obj.IO_PortSel = GPIO_PORT_B,
		.SPI_MOSI.io_obj.IO_PinSel = GPIO_PIN_14,
		.SPI_MOSI.io_obj.GpioElec = {.u16PinDir = PIN_DIR_OUT,.u16PinDrv = PIN_HIGH_DRV,},
		
		.SPI_MISO.io_obj.IO_PortSel = GPIO_PORT_I,
		.SPI_MISO.io_obj.IO_PinSel = GPIO_PIN_02,
		.SPI_MISO.io_obj.GpioElec = {.u16PinDir = PIN_DIR_IN,},
	},

	.Pagesize = 256,					//256B
	.Sectorsize = 4096, 			//4K
	.MiniBlocksize = 32*1024,	//32K
	.Blocksize = 64*1024, 		//64K
	.Capacity = 16*1024*1024,	//16M
	.Sectornum = 4096,				//4096 sectors
};

//Usr creat W25Q128、W25Q256、W25Q512......
stc_spi_device_t OTHER_DEV = {
	.Pagesize = 256,					//256B
	.Sectorsize = 4096, 			//4K
	.MiniBlocksize = 32*1024,	//32K
	.Blocksize = 64*1024, 		//64K
	.Capacity = 8*1024*1024,	//8M
	.Sectornum = 2048,				//2048 sectors
};

uint32_t Spi_device_Init(stc_spi_device_t *spi_device)
{
	uint32_t ID = 0;
	spi_device->PortPinCfg.SPI_SCK.InitHandler = xs_GpioInit;
	spi_device->PortPinCfg.SPI_SCK.OutHandler = xs_GpioOutput;
	
	spi_device->PortPinCfg.SPI_CS.InitHandler = xs_GpioInit;
	spi_device->PortPinCfg.SPI_CS.OutHandler = xs_GpioOutput;
	
	spi_device->PortPinCfg.SPI_MOSI.InitHandler = xs_GpioInit;
	spi_device->PortPinCfg.SPI_MOSI.OutHandler = xs_GpioOutput;
	
	spi_device->PortPinCfg.SPI_MISO.InitHandler = xs_GpioInit;
	spi_device->PortPinCfg.SPI_MISO.GetbitHandler = xs_GpioGetBit;
	
	spi_device->fptr_ChipSel = BSP_SPI_TabSelCmd;
	spi_device->fptr_WrRdHandler = BSP_SPI_WriteReadByte;
	spi_device->fptr_InitHandler = BSP_SPI_Port_Initialize;
	
	spi_device->fptr_InitHandler(&spi_device->PortPinCfg);
	
	//取消片选
	spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);

	DDL_DelayMS(1);

	if(spi_device == &w25q64){
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,Q64_CMD_GET_ID);
		ID |= (spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,DUMMY_BYTE)) << 16;
		ID |= (spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,DUMMY_BYTE) << 8);
		ID |= spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,DUMMY_BYTE);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);
		
		w25q64.Device_ID = ID;
	}else if(spi_device == &OTHER_DEV){
		;
	}
	if(ID != 0)
		return RES_OK;
	else 
		return RES_ERROR;
}

static uint32_t Spi_device_ReadStatus(stc_spi_device_t *spi_device)
{
	uint32_t status = 0;
	if(spi_device == &w25q64)
	{
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
    spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg, Q64_CMD_READ_STATUS1);
		status |= spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg, DUMMY_BYTE);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);
	}else if(spi_device == &OTHER_DEV){
	}
	return status;
}

static void Spi_device_Write_Enable(stc_spi_device_t *spi_device)
{	
	if(spi_device == &w25q64)
	{
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,Q64_CMD_WRITE_ENABLE);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);  
	}else if(spi_device == &OTHER_DEV){
	}
}

static void Spi_device_SectorErase(stc_spi_device_t *spi_device,uint32_t EraseAddr)
{
	if(spi_device == &w25q64)
	{
		Spi_device_Write_Enable(spi_device);
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,Q64_CMD_SECTOR_ERASE);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(EraseAddr & 0xFF0000) >> 16);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(EraseAddr & 0xFF00) >> 8);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,EraseAddr & 0xFF);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);
		
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
	}
	else if(spi_device == &OTHER_DEV)
	{
		
	}
}

static void Spi_device_MiniBlockErase(stc_spi_device_t *spi_device,uint32_t EraseAddr)
{
	if(spi_device == &w25q64)
	{
		Spi_device_Write_Enable(spi_device);
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,Q64_CMD_MINI_BLOCK_ERASE);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(EraseAddr & 0xFF0000) >> 16);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(EraseAddr & 0xFF00) >> 8);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,EraseAddr & 0xFF);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);
		
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
	}
	else if(spi_device == &OTHER_DEV)
	{
		
	}
}

static void Spi_device_BlockErase(stc_spi_device_t *spi_device,uint32_t EraseAddr)
{
	if(spi_device == &w25q64)
	{
		Spi_device_Write_Enable(spi_device);
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,Q64_CMD_BLOCK_ERASE);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(EraseAddr & 0xFF0000) >> 16);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(EraseAddr & 0xFF00) >> 8);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,EraseAddr & 0xFF);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);
		
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
	}
	else if(spi_device == &OTHER_DEV)
	{
		
	}
}

static int8_t Spi_device_WritePage(stc_spi_device_t *spi_device, uint32_t WriteAddr, uint8_t* pBuffer, uint32_t NumByteToWrite)
{
	if(spi_device->Pagesize < NumByteToWrite)
		return LL_ERR;
	if(spi_device == &w25q64)
	{
		Spi_device_Write_Enable(spi_device);
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,Q64_CMD_PAGE_PROG);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(WriteAddr & 0xFF0000) >> 16);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,(WriteAddr & 0xFF00) >> 8);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,WriteAddr & 0xFF);
		
		while (NumByteToWrite--) {
			spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg, *pBuffer);
			pBuffer++; 
		}
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);
		
		while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
	}
	else if(spi_device == &OTHER_DEV){
		
	}
	return LL_OK;
}

static int8_t Spi_device_WritePage_NoCheck(stc_spi_device_t *spi_device,uint32_t WriteAddr,uint8_t* pBuffer,uint32_t NumByteToWrite)   
{ 			 		 
	uint32_t page_remain = 0;	
	
	if(NumByteToWrite == 0)
		return LL_OK;
	
	/* 当前页剩余的字节数 */
	page_remain = spi_device->Pagesize - WriteAddr%spi_device->Pagesize;
	
	/* 如果要写入字节的大小<单页剩余字节数，说明不需要换页，则将要写入字节数赋值给pageremain */
	if(NumByteToWrite <= page_remain)
		page_remain = NumByteToWrite;
	
	while(1)
	{	 
		/* 写一页 */
		Spi_device_WritePage(spi_device,WriteAddr,pBuffer,page_remain);	 
		
		/* 如果要写入的字节和单页剩余字数正好相等，写入结束 */
		if(NumByteToWrite == page_remain)	
		{	
			break;
		}
		/* 如果要写入的字节>单页剩余字节，需要换页 */
	 	else
		{
			pBuffer += page_remain;
			/* 写入地址=写入地址+剩余字节数，相当于下一页的开始地址 */
			WriteAddr += page_remain;
			NumByteToWrite -= page_remain;

			/* 如果要写入的数据还大于页大小 */
			if(NumByteToWrite > spi_device->Pagesize)
			{
				/* 单页剩余字节数等于页大小 */
				page_remain = spi_device->Pagesize;
			} 
			else
			{	
				/* 不够一页个字节了，要写入数据字节给当页剩余字节数 */
				page_remain = NumByteToWrite;
			}
		}
	}
	return LL_OK;	
}

/* 整片擦除（对于w25q64是8Mbytes） */
void Spi_device_FullErase(stc_spi_device_t *spi_device)
{   
	if(spi_device == &w25q64)
	{
    Spi_device_Write_Enable(spi_device);
    while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
		
  	spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
    spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg, Q64_CMD_CHIP_ERASE);
			
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);     
		
    while(((Spi_device_ReadStatus(spi_device))&0x01) == 0x01);
		
	}else if(spi_device == &OTHER_DEV){
		
	}
}

int Spi_device_Write(stc_spi_device_t *spi_device,uint32_t WriteAddr,uint8_t* pBuffer,uint32_t NumByteToWrite)
{
	uint16_t sector_remain = 0;
	uint8_t * px_wr = pBuffer;
	uint32_t wr_size = 0;
	uint16_t sector_erase_num = 0;
	uint32_t sector_erase_addr = 0;
	uint8_t read_buf[spi_device->Sectorsize];
	
	if(NumByteToWrite == 0)
		return LL_OK;
	
	if((WriteAddr + NumByteToWrite) > spi_device->Capacity)
		NumByteToWrite = spi_device->Capacity - WriteAddr + 1;
	
	/* 计算需要擦除的扇区个数 */
	sector_erase_num = ((spi_device->Sectorsize*((WriteAddr + NumByteToWrite)/spi_device->Sectorsize))\
												- (spi_device->Sectorsize*(WriteAddr/spi_device->Sectorsize)))/spi_device->Sectorsize;
	
	if((WriteAddr + NumByteToWrite)%spi_device->Sectorsize)
		sector_erase_num += 1;
	
	/* 计算当前扇区剩余字节数 */
	sector_remain = spi_device->Sectorsize - WriteAddr%spi_device->Sectorsize;
	
	/* 从当前扇区开始依次向后擦除 */
	for(sector_erase_addr = spi_device->Sectorsize*(WriteAddr/spi_device->Sectorsize);
					sector_erase_num > 0;
					sector_erase_addr+=spi_device->Sectorsize,sector_erase_num--
		 )
	{
		/* 擦除前将当前扇区原始数据读出到read_buf */
		Spi_device_Read(spi_device,sector_erase_addr,read_buf,spi_device->Sectorsize);
		
		/* 擦除扇区 */
		Spi_device_SectorErase(spi_device,sector_erase_addr);
		
		if(sector_remain >= NumByteToWrite){
			wr_size = NumByteToWrite;
		}else{
			wr_size = sector_remain;
		}
		
		/* 刷新read_buf中的数据 */
		memcpy((void *)(read_buf+(spi_device->Sectorsize - sector_remain)),px_wr,wr_size);
		
		/* 将read_buf数据重新写入 */
		Spi_device_WritePage_NoCheck(spi_device,sector_erase_addr,read_buf,spi_device->Sectorsize);
		
		px_wr += wr_size;
		sector_remain = spi_device->Sectorsize;
		NumByteToWrite = NumByteToWrite - wr_size;
	}
	
	return LL_OK;
}

int Spi_device_Read(stc_spi_device_t *spi_device,uint32_t ReadAddr, uint8_t* pBuffer, uint32_t NumByteToRead)
{
	if(NumByteToRead == 0)
		return LL_OK;
	
	if(spi_device->Capacity < ReadAddr+NumByteToRead)
		NumByteToRead = ReadAddr+NumByteToRead - spi_device->Capacity + 1;
	
	if(spi_device == &w25q64)
	{
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)0);
		
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,Q64_CMD_READ);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg,	(ReadAddr & 0xFF0000) >> 16);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg, (ReadAddr& 0xFF00) >> 8);
		spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg, ReadAddr & 0xFF);
		
		/* 读数据不作特殊处理，直接读即可 */
		while (NumByteToRead--)
		{ 
			*pBuffer = spi_device->fptr_WrRdHandler(&spi_device->PortPinCfg, DUMMY_BYTE);
			pBuffer++; 
		}
		
		spi_device->fptr_ChipSel(&spi_device->PortPinCfg,(en_functional_state_t)1);
		
	}else if(spi_device == &OTHER_DEV){
	
	}
	
	return LL_OK;
}
#endif
