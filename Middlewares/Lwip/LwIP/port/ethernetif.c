#include "hc32_ll.h"
//#include "ev_hc32f4a0_lqfp176_bsp.h"
#include <string.h>
#include <stdlib.h>
#include "ethernetif.h"

#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "ethernetif.h"
#include "ev_hc32f4a0_lqfp176_tca9539.h"
/* Define those to better describe your network interface. */
#define IFNAME0 'h'
#define IFNAME1 'd'

/* PHY hardware reset time */
#define PHY_HW_RESET_DELAY                (0x40U)
/* Global Ethernet handle*/
static stc_eth_handle_t EthHandle;
__ALIGN_BEGIN static stc_eth_dma_desc_t EthDmaTxDscrTab[ETH_TX_BUF_NUM];
/* Ethernet Rx DMA Descriptor */
__ALIGN_BEGIN static stc_eth_dma_desc_t EthDmaRxDscrTab[ETH_RX_BUF_NUM];
/* Ethernet Transmit Buffer */
__ALIGN_BEGIN static uint8_t EthTxBuff[ETH_TX_BUF_NUM][ETH_TX_BUF_SIZE];
/* Ethernet Receive Buffer */
__ALIGN_BEGIN static uint8_t EthRxBuff[ETH_RX_BUF_NUM][ETH_RX_BUF_SIZE];

/**
* @brief Initializes the Ethernet GPIO.
* @param None
* @retval None
*/
static void Ethernet_GpioInit(void)
{
	/* ETH_RST */
	BSP_IO_ConfigPortPin(EIO_PORT1, EIO_ETH_RST, EIO_DIR_OUT);
	BSP_IO_WritePortPin(EIO_PORT1, EIO_ETH_RST, (uint8_t)DISABLE);
	SysTick_Delay(PHY_HW_RESET_DELAY);
	BSP_IO_WritePortPin(EIO_PORT1, EIO_ETH_RST, (uint8_t)ENABLE);
	SysTick_Delay(PHY_HW_RESET_DELAY);
	/* Configure MII/RMII selection IO for ETH */
#ifdef ETH_INTERFACE_RMII
	/* Ethernet RMII pins configuration */
	/*
	ETH_SMI_MDIO ----------------> PA2
	ETH_SMI_MDC -----------------> PC1
	ETH_RMII_TX_EN --------------> PG11
	ETH_RMII_TXD0 ---------------> PG13
	ETH_RMII_TXD1 ---------------> PG14
	ETH_RMII_REF_CLK ------------> PA1
	ETH_RMII_CRS_DV -------------> PA7
	ETH_RMII_RXD0 ---------------> PC4
	ETH_RMII_RXD1 ---------------> PC5
	ETH_RMII_RX_ER --------------> PI10
	*/
	/* Configure PA1, PA2 and PA7 */
	GPIO_SetFunc(GPIO_PORT_A, (GPIO_PIN_01 | GPIO_PIN_02 | GPIO_PIN_07),
	GPIO_FUNC_11 );
	/* Configure PC1, PC4 and PC5 */
	GPIO_SetFunc(GPIO_PORT_C, (GPIO_PIN_01 | GPIO_PIN_04 | GPIO_PIN_05),
	GPIO_FUNC_11 );
	/* Configure PG11, PG13 and PG14 */
	GPIO_SetFunc(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14),
	GPIO_FUNC_11 );
	/* Configure PI10 */
	GPIO_SetFunc(GPIO_PORT_I, GPIO_PIN_10,
	GPIO_FUNC_11 );
#else
	/* Ethernet MII pins configuration */
	/*
	ETH_SMI_MDIO ----------------> PA2
	ETH_SMI_MDC -----------------> PC1
	ETH_MII_TX_CLK --------------> PB6
	ETH_MII_TX_EN ---------------> PG11
	ETH_MII_TXD0 ----------------> PG13
	ETH_MII_TXD1 ----------------> PG14
	ETH_MII_TXD2 ----------------> PB9
	ETH_MII_TXD3 ----------------> PB8
	ETH_MII_RX_CLK --------------> PA1
	ETH_MII_RX_DV ---------------> PA7
	ETH_MII_RXD0 ----------------> PC4
	ETH_MII_RXD1 ----------------> PC5
	ETH_MII_RXD2 ----------------> PB0
	ETH_MII_RXD3 ----------------> PB1
	ETH_MII_RX_ER ---------------> PI10
	ETH_MII_CRS -----------------> PH2
	ETH_MII_COL -----------------> PH3
	*/
	/* Configure PA1, PA2 and PA7 */
	GPIO_SetFunc(GPIO_PORT_A, (GPIO_PIN_01 | GPIO_PIN_02 | GPIO_PIN_07),GPIO_FUNC_11);
	/* Configure PB0, PB1, PB6, PB8 and PB9 */
	GPIO_SetFunc(GPIO_PORT_B, (GPIO_PIN_00 | GPIO_PIN_01 | GPIO_PIN_06 |GPIO_PIN_08 | GPIO_PIN_09), GPIO_FUNC_11);
	/* Configure PC1, PC4 and PC5 */
	GPIO_SetFunc(GPIO_PORT_C, (GPIO_PIN_01 | GPIO_PIN_04 | GPIO_PIN_05),GPIO_FUNC_11);
	/* Configure PG11, PG13 and PG14 */
	GPIO_SetFunc(GPIO_PORT_G, (GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14),GPIO_FUNC_11);
	/* Configure PH2, PH3 */
	GPIO_SetFunc(GPIO_PORT_H, (GPIO_PIN_02 | GPIO_PIN_03),GPIO_FUNC_11);
	/* Configure PI10 */
	GPIO_SetFunc(GPIO_PORT_I, GPIO_PIN_10,GPIO_FUNC_11);
#endif
}
/**
* @brief In this function, the hardware should be initialized.
* Called from ethernetif_init().
*
* @param netif the already initialized lwip network interface structure
* for this ethernetif
*/
static void low_level_init(struct netif *netif)
{
	stc_eth_init_t stcEthInit;
	uint16_t u16RegVal;
	/* Enable ETH clock */
	FCG_Fcg1PeriphClockCmd(FCG1_PERIPH_ETHMAC, ENABLE);
	/* Init Ethernet GPIO */
	Ethernet_GpioInit();
	/* Reset ETHERNET */
	(void)ETH_DeInit();
	/* Configure structure initialization */
	(void)ETH_CommStructInit(&EthHandle.stcCommInit);
	(void)ETH_StructInit(&stcEthInit);
#ifdef ETH_INTERFACE_RMII
	EthHandle.stcCommInit.u32Interface = ETH_MAC_IF_RMII;
#else
	EthHandle.stcCommInit.u32Interface = ETH_MAC_IF_MII;
#endif
	/* Configure ethernet peripheral */
	if (LL_OK == ETH_Init(&EthHandle, &stcEthInit))
	{
		/* Set netif link flag */
		netif->flags |= NETIF_FLAG_LINK_UP;
	}
	/* Initialize Tx Descriptors list: Chain Mode */
	(void)ETH_DMA_TxDescListInit(&EthHandle, EthDmaTxDscrTab, &EthTxBuff[0][0],ETH_TX_BUF_NUM);
	/* Initialize Rx Descriptors list: Chain Mode */
	(void)ETH_DMA_RxDescListInit(&EthHandle, EthDmaRxDscrTab, &EthRxBuff[0][0],ETH_RX_BUF_NUM);
	/* set MAC hardware address length */
	netif->hwaddr_len = (u8_t)ETH_HWADDR_LEN;
	/* set MAC hardware address */
	netif->hwaddr[0] = (EthHandle.stcCommInit).au8MacAddr[0];
	netif->hwaddr[1] = (EthHandle.stcCommInit).au8MacAddr[1];
	netif->hwaddr[2] = (EthHandle.stcCommInit).au8MacAddr[2];
	netif->hwaddr[3] = (EthHandle.stcCommInit).au8MacAddr[3];
	netif->hwaddr[4] = (EthHandle.stcCommInit).au8MacAddr[4];
	netif->hwaddr[5] = (EthHandle.stcCommInit).au8MacAddr[5];
	/* maximum transfer unit */
	netif->mtu = 1500U;
	/* device capabilities */
	/* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;
	/* Enable MAC and DMA transmission and reception */
	(void)ETH_Start();
	/* Configure PHY LED mode */
	u16RegVal = PHY_PAGE_ADDR_7;
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_PSR, u16RegVal);
	(void)ETH_PHY_ReadReg(&EthHandle, PHY_P7_IWLFR, &u16RegVal);
	MODIFY_REG16(u16RegVal, PHY_LED_SELECT, PHY_LED_SELECT_10);
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_P7_IWLFR, u16RegVal);
	u16RegVal = PHY_PAGE_ADDR_0;
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_PSR, u16RegVal);
#ifdef ETH_INTERFACE_RMII
	/* Disable Power Saving Mode */
	(void)ETH_PHY_ReadReg(&EthHandle, PHY_PSMR, &u16RegVal);
	CLR_REG16_BIT(u16RegVal, PHY_EN_PWR_SAVE);
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_PSMR, u16RegVal);
	/* Configure PHY to generate an interrupt when Eth Link state changes */
	u16RegVal = PHY_PAGE_ADDR_7;
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_PSR, u16RegVal);
	/* Enable Interrupt on change of link status */
	(void)ETH_PHY_ReadReg(&EthHandle, PHY_P7_IWLFR, &u16RegVal);
	SET_REG16_BIT(u16RegVal, PHY_INT_LINK_CHANGE);
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_P7_IWLFR, u16RegVal);
	u16RegVal = PHY_PAGE_ADDR_0;
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_PSR, u16RegVal);
#endif
}
/**
* @brief This function should do the actual transmission of the packet. The packet is
* contained in the pbuf that is passed to the function. This pbuf
* might be chained.
*
* @param netif the lwip network interface structure for this ethernetif
* @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
* @return ERR_OK if the packet could be sent
* an err_t value if the packet couldn't be sent
*
* @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
* strange results. You might consider waiting for space in the DMA queue
* to become available since the stack doesn't retry to send a packet
* dropped because of memory failure (except for the TCP timers).
*/
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
	err_t errval;
	struct pbuf *q;
	uint8_t *txBuffer;
	__IO stc_eth_dma_desc_t *DmaTxDesc;
	uint32_t byteCnt;
	uint32_t frameLength = 0UL;
	uint32_t bufferOffset;
	uint32_t payloadOffset;
	DmaTxDesc = EthHandle.stcTxDesc;
	txBuffer = (uint8_t *)((EthHandle.stcTxDesc)->u32Buf1Addr);
	bufferOffset = 0UL;
	/* Copy frame from pbufs to driver buffers */
	for (q = p; q != NULL; q = q->next)
	{
		/* If this buffer isn't available, goto error */
		if (0UL != (DmaTxDesc->u32ControlStatus & ETH_DMA_TXDESC_OWN))
		{
			errval = (err_t)ERR_USE;
			goto error;
		}
		/* Get bytes in current lwIP buffer */
		byteCnt = q->len;
		payloadOffset = 0UL;
		/* Check if the length of data to copy is bigger than Tx buffer size */
		while ((byteCnt + bufferOffset) > ETH_TX_BUF_SIZE)
		{
			/* Copy data to Tx buffer*/
			(void)memcpy((uint8_t *)&(txBuffer[bufferOffset]), (uint8_t *)&(((uint8_t *)q->payload)[payloadOffset]), (ETH_TX_BUF_SIZE - bufferOffset));
			/* Point to next descriptor */
			DmaTxDesc = (stc_eth_dma_desc_t *)(DmaTxDesc->u32Buf2NextDescAddr);
			/* Check if the buffer is available */
			if (0UL != (DmaTxDesc->u32ControlStatus & ETH_DMA_TXDESC_OWN))
			{
				errval = (err_t)ERR_USE;
				goto error;
			}
			txBuffer = (uint8_t *)(DmaTxDesc->u32Buf1Addr);
			byteCnt = byteCnt - (ETH_TX_BUF_SIZE - bufferOffset);
			payloadOffset = payloadOffset + (ETH_TX_BUF_SIZE - bufferOffset);
			frameLength = frameLength + (ETH_TX_BUF_SIZE - bufferOffset);
			bufferOffset = 0UL;
		}
		/* Copy the remaining bytes */
		(void)memcpy((uint8_t *)&(txBuffer[bufferOffset]), (uint8_t *)&(((uint8_t *)q->payload)[payloadOffset]), byteCnt);
		bufferOffset = bufferOffset + byteCnt;
		frameLength = frameLength + byteCnt;
	}
	/* Prepare transmit descriptors to give to DMA */
	(void)ETH_DMA_SetTransFrame(&EthHandle, frameLength);
	errval = (err_t)ERR_OK;
error:
	/* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume
	transmission */
	if (0UL != (READ_REG32_BIT(CM_ETH->DMA_DMASTSR, ETH_DMA_FLAG_UNS)))
	{
		/* Clear DMA UNS flag */
		WRITE_REG32(CM_ETH->DMA_DMASTSR, ETH_DMA_FLAG_UNS);
		/* Resume DMA transmission */
		WRITE_REG32(CM_ETH->DMA_TXPOLLR, 0UL);
	}
	return errval;
}
/**
* @brief Should allocate a pbuf and transfer the bytes of the incoming
* packet from the interface into the pbuf.
*
* @param netif the lwip network interface structure for this ethernetif
* @return a pbuf filled with the received packet (including MAC header)
* NULL on memory error
*/
static struct pbuf *low_level_input(struct netif *netif)
{
	struct pbuf *p = NULL;
	struct pbuf *q;
	uint32_t len;
	uint8_t *rxBuffer;
	__IO stc_eth_dma_desc_t *DmaRxDesc;
	uint32_t byteCnt;
	uint32_t bufferOffset;
	uint32_t payloadOffset;
	uint32_t i;
	/* Get received frame */
	if (LL_OK != ETH_DMA_GetReceiveFrame(&EthHandle))
	{
		return NULL;
	}
	/* Obtain the size of the packet */
	len = (EthHandle.stcRxFrame).u32Len;
	rxBuffer = (uint8_t *)(EthHandle.stcRxFrame).u32Buf;
	if (len > 0UL)
	{
		/* Allocate a pbuf chain of pbufs from the Lwip buffer pool */
		p = pbuf_alloc(PBUF_RAW, (uint16_t)len, PBUF_POOL);
	}
	if (p != NULL)
	{
		DmaRxDesc = (EthHandle.stcRxFrame).pstcFSDesc;
		bufferOffset = 0UL;
		for (q = p; q != NULL; q = q->next)
		{
			byteCnt = q->len;
			payloadOffset = 0UL;
			/* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
			while ((byteCnt + bufferOffset) > ETH_RX_BUF_SIZE)
			{
				/* Copy data to pbuf */
				(void)memcpy((uint8_t *)&(((uint8_t *)q->payload)[payloadOffset]), (uint8_t
				*)&(rxBuffer[bufferOffset]), (ETH_RX_BUF_SIZE - bufferOffset));
				/* Point to next descriptor */
				DmaRxDesc = (stc_eth_dma_desc_t *)(DmaRxDesc->u32Buf2NextDescAddr);
				rxBuffer = (uint8_t *)(DmaRxDesc->u32Buf1Addr);
				byteCnt = byteCnt - (ETH_RX_BUF_SIZE - bufferOffset);
				payloadOffset = payloadOffset + (ETH_RX_BUF_SIZE - bufferOffset);
				bufferOffset = 0UL;
			}
			/* Copy remaining data in pbuf */
			(void)memcpy((uint8_t *)&(((uint8_t *)q->payload)[payloadOffset]), (uint8_t*)&(rxBuffer[bufferOffset]), byteCnt);
			bufferOffset = bufferOffset + byteCnt;
		}
	}
	/* Release descriptors to DMA */
	/* Point to first descriptor */
	DmaRxDesc = (EthHandle.stcRxFrame).pstcFSDesc;
	for (i = 0UL; i < (EthHandle.stcRxFrame).u32SegCount; i++)
	{
		DmaRxDesc->u32ControlStatus |= ETH_DMA_RXDESC_OWN;
		DmaRxDesc = (stc_eth_dma_desc_t *)(DmaRxDesc->u32Buf2NextDescAddr);
	}
	/* Clear Segment_Count */
	(EthHandle.stcRxFrame).u32SegCount = 0UL;
	/* When Rx Buffer unavailable flag is set, clear it and resume reception */
	if (0UL != (READ_REG32_BIT(CM_ETH->DMA_DMASTSR, ETH_DMA_FLAG_RUS)))
	{
		/* Clear DMA RUS flag */
		WRITE_REG32(CM_ETH->DMA_DMASTSR, ETH_DMA_FLAG_RUS);
		/* Resume DMA reception */
		WRITE_REG32(CM_ETH->DMA_RXPOLLR, 0UL);
	}
	return p;
}
/**
* @brief This function should be called when a packet is ready to be read
* from the interface. It uses the function low_level_input() that
* should handle the actual reception of bytes from the network
* interface. Then the type of the received packet is determined and
* the appropriate input function is called.
*
* @param netif the lwip network interface structure for this ethernetif
*/
void ethernetif_input(struct netif *netif)
{
	err_t err;
	struct pbuf *p;
	/* Move received packet into a new pbuf */
	p = low_level_input(netif);
	/* No packet could be read, silently ignore this */
	if (p == NULL)
	{
		return;
	}
	/* Entry point to the LwIP stack */
	err = netif->input(p, netif);
	if (err != (err_t)ERR_OK)
	{
		LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
		(void)pbuf_free(p);
	}
}
/**
* @brief Should be called at the beginning of the program to set up the
* network interface. It calls the function low_level_init() to do the
* actual setup of the hardware.
*
* This function should be passed as a parameter to netif_add().
*
* @param netif the lwip network interface structure for this ethernetif
* @return ERR_OK if the IF is initialized
* ERR_MEM if private data couldn't be allocated
* any other err_t on error
*/
err_t ethernetif_init(struct netif *netif)
{
	#if LWIP_NETIF_HOSTNAME
	/* Initialize interface hostname */
	netif->hostname = "lwip";
	#endif /* LWIP_NETIF_HOSTNAME */
	netif->name[0] = IFNAME0;
	netif->name[1] = IFNAME1;
	/* We directly use etharp_output() here to save a function call.
	* You can instead declare your own function an call etharp_output()
	* from it if you have to do some checks before sending (e.g. if link
	* is available...) */
	netif->output = &etharp_output;
	netif->linkoutput = &low_level_output;
	/* initialize the hardware */
	low_level_init(netif);
	return (err_t)ERR_OK;
}

/**
* @brief Returns the current time in milliseconds when LWIP_TIMERS == 1 and NO_SYS
== 1
* @param None
* @retval Current Time value
*/
u32_t sys_now(void)
{
	return SysTick_GetTick();
}

/**
* @brief Check the netif link status.
* @param netif the network interface
* @retval None
*/
void EthernetIF_CheckLink(struct netif *netif)
{
	static uint8_t u8PreStatus = 0U;
	uint16_t u16RegVal = 0U;
	/* Read PHY_BSR */
	(void)ETH_PHY_ReadReg(&EthHandle, PHY_BSR, &u16RegVal);
	/* Check whether the link is up or down*/
	if ((0x0000U != u16RegVal) && (0xFFFFU != u16RegVal))
	{
		if ((0U != (u16RegVal & PHY_LINK_STATUS)) && (0U == u8PreStatus))
		{
			netif_set_link_up(netif);
			u8PreStatus = 1U;
		}
		if ((0U == (u16RegVal & PHY_LINK_STATUS)) && (1U == u8PreStatus))
		{
			netif_set_link_down(netif);
			u8PreStatus = 0U;
		}
	}
}
/**
* @brief Update the netif link status.
* @param netif the network interface
* @retval None
*/
void EthernetIF_UpdateLink(struct netif *netif)
{
	uint16_t u16RegVal;
	u16RegVal = PHY_PAGE_ADDR_0;
	(void)ETH_PHY_WriteReg(&EthHandle, PHY_PSR, u16RegVal);
	/* Read PHY_IISDR */
	(void)ETH_PHY_ReadReg(&EthHandle, PHY_IISDR, &u16RegVal);
	/* Check whether the link interrupt has occurred or not */
	if (0U != (u16RegVal & PHY_FLAG_LINK_STATUS_CHANGE))
	{
		/* Read PHY_BSR */
		(void)ETH_PHY_ReadReg(&EthHandle, PHY_BSR, &u16RegVal);
		if ((0x0000U != u16RegVal) && (0xFFFFU != u16RegVal))
		{
			if (!netif_is_link_up(netif))
			{
				/* Wait until the auto-negotiation will be completed */
				SysTick_Delay(2U);
				(void)ETH_PHY_ReadReg(&EthHandle, PHY_BSR, &u16RegVal);
			}
			/* Check whether the link is up or down*/
			if (0U != (u16RegVal & PHY_LINK_STATUS))
			{
				netif_set_link_up(netif);
			}
			else
			{
				netif_set_link_down(netif);
			}
		}
	}
}
/**
* @brief Link callback function
* @note This function is called on change of link status to update low level
* driver configuration.
* @param netif The network interface
* @retval None
*/
void EthernetIF_LinkCallback(struct netif *netif)
{
	__IO uint32_t tickStart = 0UL;
	uint16_t u16RegVal = 0U;
	__IO int negoResult = LL_ERR;
	if (netif_is_link_up(netif))
	{
		/* Restart the auto-negotiation */
		if (ETH_AUTO_NEGO_DISABLE != (EthHandle.stcCommInit).u16AutoNego)
		{
			/* Enable Auto-Negotiation */
			(void)ETH_PHY_ReadReg(&EthHandle, PHY_BCR, &u16RegVal);
			u16RegVal |= PHY_AUTONEGOTIATION;
			(void)ETH_PHY_WriteReg(&EthHandle, PHY_BCR, u16RegVal);
			/* Wait until the auto-negotiation will be completed */
			tickStart = SysTick_GetTick();
			do
			{
			(void)ETH_PHY_ReadReg(&EthHandle, PHY_BSR, &u16RegVal);
			if (PHY_AUTONEGO_COMPLETE == (u16RegVal &
			PHY_AUTONEGO_COMPLETE))
			{
			break;
			}
			/* Check for the Timeout (3s) */
			} while ((SysTick_GetTick() - tickStart) <= 3000U);
			if (PHY_AUTONEGO_COMPLETE == (u16RegVal &
			PHY_AUTONEGO_COMPLETE))
			{
				negoResult = LL_OK;
				/* Configure ETH duplex mode according to the result of automatic negotiation */
				if (0U != (u16RegVal & (PHY_100BASE_TX_FD | PHY_10BASE_T_FD)))
				{
					(EthHandle.stcCommInit).u32DuplexMode = ETH_MAC_DUPLEX_MD_FULL;
				}
				else
				{
					(EthHandle.stcCommInit).u32DuplexMode = ETH_MAC_DUPLEX_MD_HALF;
				}
				/* Configure ETH speed according to the result of automatic negotiation */
				if (0U != (u16RegVal & (PHY_100BASE_TX_FD | PHY_100BASE_TX_HD)))
				{
					(EthHandle.stcCommInit).u32Speed = ETH_MAC_SPEED_100M;
				}
				else
				{
					(EthHandle.stcCommInit).u32Speed = ETH_MAC_SPEED_10M;
				}
			}
		}
		/* AutoNegotiation disable or failed*/
		if (LL_ERR == negoResult)
		{
			(void)ETH_PHY_ReadReg(&EthHandle, PHY_BCR, &u16RegVal);
			u16RegVal &= ~0x2100;
			/* Set MAC Speed and Duplex Mode to PHY */
			(void)ETH_PHY_WriteReg(&EthHandle, PHY_BCR,
			((uint16_t)((EthHandle.stcCommInit).u32DuplexMode >> 3U) |
			(uint16_t)((EthHandle.stcCommInit).u32Speed >> 1U) | u16RegVal));
		}
		/* ETH MAC Re-Configuration */
		ETH_MAC_SetDuplexSpeed((EthHandle.stcCommInit).u32DuplexMode,
		(EthHandle.stcCommInit).u32Speed);
		/* Restart MAC interface */
		(void)ETH_Start();
	}
	else
	{
	/* Stop MAC interface */
	(void)ETH_Stop();
	}
	/* Notify link status change */
	EthernetIF_NotifyLinkChange(netif);
}
/**
* @brief Notify link status change.
* @param netif the network interface
* @retval None
*/
__WEAKDEF void EthernetIF_NotifyLinkChange(struct netif *netif)
{
	/* This is function could be implemented in user file
	when the callback is needed */
}
