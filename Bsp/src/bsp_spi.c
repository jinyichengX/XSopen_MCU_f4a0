#include "bsp_spi.h"

#if !SOFT_SPI_ENABLE

void BSP_SPI_Init(spi_cfg_t spi_cfg)
{
	stc_spi_init_t stcSpiInit;
	stc_dma_init_t stcDmaInit;
	stc_irq_signin_config_t stcIrqSignConfig;

	/* Configure Port */
	GPIO_SetFunc(spi_cfg.spi_cs.port,spi_cfg.spi_cs.pin,spi_cfg.spi_cs.func);
	GPIO_SetFunc(spi_cfg.spi_clk.port,spi_cfg.spi_clk.pin,spi_cfg.spi_clk.func);
	GPIO_SetFunc(spi_cfg.spi_mosi.port,spi_cfg.spi_mosi.pin,spi_cfg.spi_mosi.func);
	GPIO_SetFunc(spi_cfg.spi_miso.port,spi_cfg.spi_miso.pin,spi_cfg.spi_miso.func);

	/* Configuration SPI */
	if(spi_cfg.unit == CM_SPI1)
		FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI1, ENABLE);
	else if(spi_cfg.unit == CM_SPI2)
		FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI2, ENABLE);
	else if(spi_cfg.unit == CM_SPI3)
		FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI3, ENABLE);
	else if(spi_cfg.unit == CM_SPI4)
		FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI4, ENABLE);
	else if(spi_cfg.unit == CM_SPI5)
		FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI5, ENABLE);
	else if(spi_cfg.unit == CM_SPI6)
		FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_SPI6, ENABLE);
	
	SPI_StructInit(&stcSpiInit);
	stcSpiInit.u32WireMode          = SPI_4_WIRE;
	stcSpiInit.u32TransMode         = SPI_FULL_DUPLEX;
	stcSpiInit.u32MasterSlave       = SPI_MASTER;
	stcSpiInit.u32Parity            = SPI_PARITY_INVD;
	stcSpiInit.u32SpiMode           = SPI_MD_0;
	stcSpiInit.u32BaudRatePrescaler = SPI_BR_CLK_DIV64;
	stcSpiInit.u32DataBits          = SPI_DATA_SIZE_8BIT;
	stcSpiInit.u32FirstBit          = SPI_FIRST_MSB;
	stcSpiInit.u32FrameLevel        = SPI_1_FRAME;
	(void)SPI_Init(spi_cfg.unit, &stcSpiInit);

	/* DMA configuration */
	if(spi_cfg.linked_dma_unit == CM_DMA1)
		FCG_Fcg0PeriphClockCmd((FCG0_PERIPH_DMA1 | FCG0_PERIPH_AOS), ENABLE);
	else if(spi_cfg.linked_dma_unit == CM_DMA2)
		FCG_Fcg0PeriphClockCmd((FCG0_PERIPH_DMA2 | FCG0_PERIPH_AOS), ENABLE);
	(void)DMA_StructInit(&stcDmaInit);
	stcDmaInit.u32BlockSize  = 1UL;
	stcDmaInit.u32TransCount = 0;
	stcDmaInit.u32DataWidth  = DMA_DATAWIDTH_8BIT;
	/* Configure TX */
	stcDmaInit.u32SrcAddrInc  = DMA_SRC_ADDR_INC;
	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;
	stcDmaInit.u32SrcAddr     = (uint32_t)(&spi_cfg.tx_buf);
	stcDmaInit.u32DestAddr    = (uint32_t)(&spi_cfg.unit->DR);
	if (LL_OK != DMA_Init(spi_cfg.linked_dma_unit, spi_cfg.tx_link_dma_ch, &stcDmaInit)) {
			for (;;) {
			}
	}
	if(spi_cfg.unit == CM_SPI1)
		AOS_SetTriggerEventSrc(spi_cfg.dma_tx_trig_ch, EVT_SRC_SPI1_SPTI);
	else if(spi_cfg.unit == CM_SPI2)
		AOS_SetTriggerEventSrc(spi_cfg.dma_tx_trig_ch, EVT_SRC_SPI2_SPTI);
	else if(spi_cfg.unit == CM_SPI3)
		AOS_SetTriggerEventSrc(spi_cfg.dma_tx_trig_ch, EVT_SRC_SPI3_SPTI);
	else if(spi_cfg.unit == CM_SPI4)
		AOS_SetTriggerEventSrc(spi_cfg.dma_tx_trig_ch, EVT_SRC_SPI4_SPTI);
	else if(spi_cfg.unit == CM_SPI5)
		AOS_SetTriggerEventSrc(spi_cfg.dma_tx_trig_ch, EVT_SRC_SPI5_SPTI);
	else if(spi_cfg.unit == CM_SPI6)
		AOS_SetTriggerEventSrc(spi_cfg.dma_tx_trig_ch, EVT_SRC_SPI6_SPTI);
	/* Configure RX */
	stcDmaInit.u32IntEn       = DMA_INT_DISABLE;
	stcDmaInit.u32SrcAddrInc  = DMA_SRC_ADDR_FIX;
	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;
	stcDmaInit.u32SrcAddr     = (uint32_t)(&spi_cfg.unit->DR);
	stcDmaInit.u32DestAddr    = (uint32_t)(&spi_cfg.rx_buf);
	if (LL_OK != DMA_Init(spi_cfg.linked_dma_unit, spi_cfg.rx_link_dma_ch, &stcDmaInit)) {
			for (;;) {
			}
	}
	if(spi_cfg.unit == CM_SPI1)
		AOS_SetTriggerEventSrc(spi_cfg.dma_rx_trig_ch, EVT_SRC_SPI1_SPRI);
	else if(spi_cfg.unit == CM_SPI2)
		AOS_SetTriggerEventSrc(spi_cfg.dma_rx_trig_ch, EVT_SRC_SPI2_SPRI);
	else if(spi_cfg.unit == CM_SPI3)
		AOS_SetTriggerEventSrc(spi_cfg.dma_rx_trig_ch, EVT_SRC_SPI3_SPRI);
	else if(spi_cfg.unit == CM_SPI4)
		AOS_SetTriggerEventSrc(spi_cfg.dma_rx_trig_ch, EVT_SRC_SPI4_SPRI);
	else if(spi_cfg.unit == CM_SPI5)
		AOS_SetTriggerEventSrc(spi_cfg.dma_rx_trig_ch, EVT_SRC_SPI5_SPRI);
	else if(spi_cfg.unit == CM_SPI6)
		AOS_SetTriggerEventSrc(spi_cfg.dma_rx_trig_ch, EVT_SRC_SPI6_SPRI);

//	/* DMA receive NVIC configure */
//	stcIrqSignConfig.enIntSrc    = DMA_RX_INT_SRC;
//	stcIrqSignConfig.enIRQn      = DMA_RX_IRQ_NUM;
//	stcIrqSignConfig.pfnCallback = &DMA_TransCompleteCallback;
//	(void)INTC_IrqSignIn(&stcIrqSignConfig);
//	NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
//	NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
//	NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);

	/* Enable DMA and channel */
	DMA_Cmd(spi_cfg.linked_dma_unit, ENABLE);
	DMA_ChCmd(spi_cfg.linked_dma_unit, spi_cfg.tx_link_dma_ch, ENABLE);
	DMA_ChCmd(spi_cfg.linked_dma_unit, spi_cfg.rx_link_dma_ch, ENABLE);
}

int32_t BSP_SPI_WriteRead(const void *pvTxBuf, void *pvRxBuf, uint32_t u32TxLen, uint32_t u32Timeout)
{
	SPI_Trans(CM_SPI1, pvTxBuf, u32TxLen, u32Timeout);
}
#else
void SPI_Delay()
{
		for( int i = 0; i < 1; i++ ){
		__ASM("NOP");
	}	
}

void BSP_SPI_Port_Initialize(SPI_InitCfg_Typedef * SPI_InitCfg)
{
	SPI_InitCfg->SPI_SCK.InitHandler(&SPI_InitCfg->SPI_SCK);
	SPI_InitCfg->SPI_CS.InitHandler(&SPI_InitCfg->SPI_CS);
	SPI_InitCfg->SPI_MOSI.InitHandler(&SPI_InitCfg->SPI_MOSI);
	SPI_InitCfg->SPI_MISO.InitHandler(&SPI_InitCfg->SPI_MISO);
}
/*
* 函数名：uint8_t SPI_WriteReadByte(uint8_t data)
* 输入参数：data -> 要写的一个字节数据
* 输出参数：read_data -> 读取到的一个字节
* 返回值：读到的数据
* 函数作用：模拟 SPI 读写一个字节
*/
uint8_t BSP_SPI_WriteReadByte(SPI_InitCfg_Typedef * SPI_InitCfg,uint8_t data)
{
	uint8_t i = 0;
	uint8_t temp = 0;
	uint8_t read_data = 0xFF;
	for(i=0;i<8;i++) 
	{
		temp = ((data&0x80)==0x80)? 1:0;
		data = data<<1;
		read_data = read_data<<1;
		SPI_InitCfg->SPI_SCK.OutHandler(&SPI_InitCfg->SPI_SCK,(en_functional_state_t)0);
		SPI_InitCfg->SPI_MOSI.OutHandler(&SPI_InitCfg->SPI_MOSI,(en_functional_state_t)temp);
		SPI_Delay();
		SPI_InitCfg->SPI_SCK.OutHandler(&SPI_InitCfg->SPI_SCK,(en_functional_state_t)1);
		SPI_Delay();
		if(SPI_InitCfg->SPI_MISO.GetbitHandler(&SPI_InitCfg->SPI_MISO)) 
		{
			read_data = read_data + 1; 
		} 
	}
	SPI_InitCfg->SPI_SCK.OutHandler(&SPI_InitCfg->SPI_SCK,(en_functional_state_t)0);
	return read_data;
}

void BSP_SPI_TabSelCmd(SPI_InitCfg_Typedef * SPI_InitCfg,en_functional_state_t state)
{
	SPI_InitCfg->SPI_CS.OutHandler(&SPI_InitCfg->SPI_CS,(en_functional_state_t)state);
}

#endif



