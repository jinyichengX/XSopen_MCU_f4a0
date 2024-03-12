#include "bsp_dma.h"
#include "string.h"
void DMA_Config_For_Uart(Usart_cfgInfo_Typedef *Usart_cfgInfo)
{
	int32_t i32Ret;
	stc_dma_init_t stcDmaInit;
	stc_irq_signin_config_t stcIrqSignConfig;
	
	if(Usart_cfgInfo->Linked_DMABase == CM_DMA1)
	{
		FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA1, ENABLE);
	}
	else if(Usart_cfgInfo->Linked_DMABase == CM_DMA2)
	{
		FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA2, ENABLE);
	}
	
	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);
	
	/* USART_RX_DMA */
	(void)DMA_StructInit(&stcDmaInit);
	stcDmaInit.u32IntEn = DMA_INT_ENABLE;
	stcDmaInit.u32BlockSize = 1UL;
	stcDmaInit.u32TransCount = 0;
	stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
	stcDmaInit.u32DestAddr = (uint32_t)(Usart_cfgInfo->rx_buf);
	stcDmaInit.u32SrcAddr = ((uint32_t)(&Usart_cfgInfo->Usart_Unit->DR) + 2UL);
	stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_FIX;
	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;
	i32Ret = DMA_Init(Usart_cfgInfo->Linked_DMABase, Usart_cfgInfo->Linked_DMA_rxChannel, &stcDmaInit);
	
	if (LL_OK == i32Ret) {
		
		if(Usart_cfgInfo->Linked_DMABase == CM_DMA1){
			AOS_SetTriggerEventSrc((uint32_t)(AOS_DMA1_0+4*(Usart_cfgInfo->Linked_DMA_rxChannel)), (en_event_src_t)Usart_cfgInfo->DMA_Trig_rxSource);
		}else if(Usart_cfgInfo->Linked_DMABase == CM_DMA2){
			AOS_SetTriggerEventSrc((uint32_t)(AOS_DMA2_0+4*(Usart_cfgInfo->Linked_DMA_rxChannel)), (en_event_src_t)Usart_cfgInfo->DMA_Trig_rxSource);
		}
		
		/* 使能DMA */
		DMA_Cmd(Usart_cfgInfo->Linked_DMABase, ENABLE);

		//屏蔽所有中断
		DMA_TransCompleteIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_BTC_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);
		DMA_TransCompleteIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_TC_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);

		DMA_ErrIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_REQ_ERR_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);
		DMA_ErrIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_TRANS_ERR_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);
		
		/* 使能DMA通道 */
		(void)DMA_ChCmd(Usart_cfgInfo->Linked_DMABase,  Usart_cfgInfo->Linked_DMA_rxChannel, ENABLE);
  }
	
	/* USART_TX_DMA */
	(void)DMA_StructInit(&stcDmaInit);
	stcDmaInit.u32IntEn = DMA_INT_ENABLE;
	stcDmaInit.u32BlockSize = 1UL;
	stcDmaInit.u32TransCount = 0;
	stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
	stcDmaInit.u32DestAddr = ((uint32_t)&Usart_cfgInfo->Usart_Unit->DR);
	/* 源数据地址暂设为0，启动发送时调用DMA_Reload_Src_For_Uart再重新设置 */
	stcDmaInit.u32SrcAddr = 0ul;
	stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_INC;
	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_FIX;
	i32Ret = DMA_Init(Usart_cfgInfo->Linked_DMABase, Usart_cfgInfo->Linked_DMA_txChannel, &stcDmaInit);
	
	if (LL_OK == i32Ret) {
		if(Usart_cfgInfo->Linked_DMABase == CM_DMA1)
		{
			if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH0) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC0;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH1) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC1;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH2) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC2;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH3) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC3;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH4) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC4;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH5) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC5;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH6) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC6;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH7) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC7;
		}
		else if(Usart_cfgInfo->Linked_DMABase == CM_DMA2)
		{
			if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH0) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA1_TC0;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH1) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA2_TC1;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH2) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA2_TC2;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH3) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA2_TC3;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH4) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA2_TC4;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH5) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA2_TC5;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH6) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA2_TC6;
			else if(Usart_cfgInfo->Linked_DMA_txChannel == DMA_CH7) 
				stcIrqSignConfig.enIntSrc = INT_SRC_DMA2_TC7;
		}
		stcIrqSignConfig.enIRQn  = Usart_cfgInfo->tx_dma_tc_enIRQn;
		if(Usart_cfgInfo->Usart_Unit == CM_USART1){
#if UART1_EN
			stcIrqSignConfig.pfnCallback = &U1_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART2){
#if UART2_EN
			stcIrqSignConfig.pfnCallback = &U2_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART3){
#if UART3_EN
			stcIrqSignConfig.pfnCallback = &U3_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART4){
#if UART4_EN
			stcIrqSignConfig.pfnCallback = &U4_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART5){
#if UART5_EN
			stcIrqSignConfig.pfnCallback = &U5_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART6){
#if UART6_EN
			stcIrqSignConfig.pfnCallback = &U6_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART7){
#if UART7_EN
			stcIrqSignConfig.pfnCallback = &U7_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART8){
#if UART8_EN
			stcIrqSignConfig.pfnCallback = &U8_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART9){
#if UART9_EN
			stcIrqSignConfig.pfnCallback = &U9_TX_DMA_TC_IrqCallback;
#endif
		}else if(Usart_cfgInfo->Usart_Unit == CM_USART10){
#if UART10_EN
			stcIrqSignConfig.pfnCallback = &U10_TX_DMA_TC_IrqCallback;
#endif
		}
		(void)INTC_IrqSignIn(&stcIrqSignConfig);
		NVIC_ClearPendingIRQ(stcIrqSignConfig.enIRQn);
		NVIC_SetPriority(stcIrqSignConfig.enIRQn, DDL_IRQ_PRIO_DEFAULT);
		NVIC_EnableIRQ(stcIrqSignConfig.enIRQn);
		
		if(Usart_cfgInfo->Linked_DMABase == CM_DMA1){
			AOS_SetTriggerEventSrc((uint32_t)(AOS_DMA1_0+4*(Usart_cfgInfo->Linked_DMA_txChannel)), (en_event_src_t)Usart_cfgInfo->DMA_Trig_txSource);
		}else if(Usart_cfgInfo->Linked_DMABase == CM_DMA2){
			AOS_SetTriggerEventSrc((uint32_t)(AOS_DMA2_0+4*(Usart_cfgInfo->Linked_DMA_txChannel)), (en_event_src_t)Usart_cfgInfo->DMA_Trig_txSource);
		}
		
		/* 使能DMA但不使能通道，在发送时使能对应通道 */
		DMA_Cmd(Usart_cfgInfo->Linked_DMABase, ENABLE);

		/* 先屏蔽所有中断 */
		DMA_TransCompleteIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_BTC_CH0<<(Usart_cfgInfo->Linked_DMA_txChannel), DISABLE);
		DMA_TransCompleteIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_TC_CH0<<(Usart_cfgInfo->Linked_DMA_txChannel), DISABLE);

		DMA_ErrIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_REQ_ERR_CH0<<(Usart_cfgInfo->Linked_DMA_txChannel), DISABLE);
		DMA_ErrIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_TRANS_ERR_CH0<<(Usart_cfgInfo->Linked_DMA_txChannel), DISABLE);
		
		/* 再开启传输完成中断 */
		DMA_TransCompleteIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_TC_CH0<<(Usart_cfgInfo->Linked_DMA_txChannel), ENABLE);
  }
}

//此函数在usart1,2,5,6 timeout接收中断/usart3,4,7,8,9,10完成接收使用，用于初始化DMA目的地址
void DMA_Reload_Dest_For_Uart(Usart_cfgInfo_Typedef *Usart_cfgInfo)
{
	/* 重载目的地址需要先失能对应通道（这是个大坑） */
	DMA_MxChCmd(Usart_cfgInfo->Linked_DMABase, 1<<Usart_cfgInfo->Linked_DMA_rxChannel, DISABLE);
	
	/* 重载目的地址 */
	uint32_t *dar = (uint32_t *)Usart_cfgInfo->rx_buf;
	DMA_SetDestAddr(Usart_cfgInfo->Linked_DMABase, Usart_cfgInfo->Linked_DMA_rxChannel,(uint32_t)dar);
	
	/* 使能对应通道 */
	(void)DMA_ChCmd(Usart_cfgInfo->Linked_DMABase,  Usart_cfgInfo->Linked_DMA_rxChannel, ENABLE);
}

//此函数在使用串口发送之前使用，用于初始化DMA源地址
void DMA_Reload_Src_For_Uart(Usart_cfgInfo_Typedef *Usart_cfgInfo, uint8_t *p_tx)
{
	DMA_SetSrcAddr(Usart_cfgInfo->Linked_DMABase, Usart_cfgInfo->Linked_DMA_txChannel,(uint32_t)p_tx);
}


//void DMA_Config_For_I2cDev(Usart_cfgInfo_Typedef *Usart_cfgInfo)
//{
//	int32_t i32Ret;
//	stc_dma_init_t stcDmaInit;
//	
//	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA1, ENABLE);
//	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_DMA2, ENABLE);
//	
//	FCG_Fcg0PeriphClockCmd(FCG0_PERIPH_AOS, ENABLE);
//	
//	/* USART_RX_DMA */
//	(void)DMA_StructInit(&stcDmaInit);
//	stcDmaInit.u32IntEn = DMA_INT_ENABLE;
//	stcDmaInit.u32BlockSize = 1UL;
//	stcDmaInit.u32TransCount = 0;
//	stcDmaInit.u32DataWidth = DMA_DATAWIDTH_8BIT;
//	stcDmaInit.u32DestAddr = (uint32_t)(Usart_cfgInfo->rx_buf);
//	stcDmaInit.u32SrcAddr = ((uint32_t)(&Usart_cfgInfo->Usart_Unit->DR) + 2UL);
//	stcDmaInit.u32SrcAddrInc = DMA_SRC_ADDR_FIX;
//	stcDmaInit.u32DestAddrInc = DMA_DEST_ADDR_INC;
//	i32Ret = DMA_Init(Usart_cfgInfo->Linked_DMABase, Usart_cfgInfo->Linked_DMA_rxChannel, &stcDmaInit);
//	
//	if (LL_OK == i32Ret) {
//		
//		if(Usart_cfgInfo->Linked_DMABase == CM_DMA1){
//			AOS_SetTriggerEventSrc((uint32_t)(AOS_DMA1_0+4*(Usart_cfgInfo->Linked_DMA_rxChannel)), (en_event_src_t)Usart_cfgInfo->DMA_Trig_rxSource);
//		}else if(Usart_cfgInfo->Linked_DMABase == CM_DMA2){
//			AOS_SetTriggerEventSrc((uint32_t)(AOS_DMA2_0+4*(Usart_cfgInfo->Linked_DMA_rxChannel)), (en_event_src_t)Usart_cfgInfo->DMA_Trig_rxSource);
//		}

//		DMA_Cmd(Usart_cfgInfo->Linked_DMABase, ENABLE);

//		//屏蔽所有中断
//		DMA_TransCompleteIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_BTC_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);
//		DMA_TransCompleteIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_TC_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);

//		DMA_ErrIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_REQ_ERR_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);
//		DMA_ErrIntCmd(Usart_cfgInfo->Linked_DMABase, DMA_INT_TRANS_ERR_CH0<<(Usart_cfgInfo->Linked_DMA_rxChannel), DISABLE);

//		(void)DMA_ChCmd(Usart_cfgInfo->Linked_DMABase,  Usart_cfgInfo->Linked_DMA_rxChannel, ENABLE);
//  }
//}















