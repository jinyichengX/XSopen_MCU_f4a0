#include "../inc/bsp_i2c.h"
#include "bsp_gpio.h"

#if !SOFT_I2C_ENABLE
static void BSP_I2C_PORT_PIN_INIT(I2Cx_InitCfg_Typedef * I2Cx_InitCfg)
{
	stc_gpio_init_t stcGpioInit;
	/* Configuration I2C GPIO */
	(void)GPIO_StructInit(&stcGpioInit);
	(void)GPIO_Init(I2Cx_InitCfg->PortPin.SCL_Port, I2Cx_InitCfg->PortPin.SCL_Pin, &stcGpioInit);
	(void)GPIO_Init(I2Cx_InitCfg->PortPin.SDA_Port, I2Cx_InitCfg->PortPin.SDA_Pin,&stcGpioInit);
	GPIO_SetFunc(I2Cx_InitCfg->PortPin.SCL_Port, I2Cx_InitCfg->PortPin.SCL_Pin,I2Cx_InitCfg->PortPin.SCL_Func);
	GPIO_SetFunc(I2Cx_InitCfg->PortPin.SDA_Port, I2Cx_InitCfg->PortPin.SDA_Pin, I2Cx_InitCfg->PortPin.SDA_Func);
}

static int32_t BSP_I2C_FUNC_EN(I2Cx_InitCfg_Typedef *I2Cx_InitCfg)
{
    uint8_t i;
    int32_t i32Ret = LL_ERR;
    float32_t fErr;

    I2C_DeInit(I2Cx_InitCfg->CM_I2C_UNITx);

    for (i = 0U; i < 5U; i++) {
        i32Ret = I2C_Init(I2Cx_InitCfg->CM_I2C_UNITx, &I2Cx_InitCfg->stcI2cInit, &fErr);
        if (LL_OK != i32Ret) {
            I2Cx_InitCfg->stcI2cInit.u32ClockDiv--;
        } else {
            break;
        }
    }

    if (LL_OK == i32Ret) {
        I2C_BusWaitCmd(I2Cx_InitCfg->CM_I2C_UNITx, ENABLE);
    }

    I2C_Cmd(I2Cx_InitCfg->CM_I2C_UNITx, ENABLE);
    return i32Ret;
}


int32_t BSP_I2CPortPinInit_and_I2CFuncEn(I2Cx_InitCfg_Typedef * I2Cx_InitCfg)
{
	/* Initialize I2C Ports and Pins */
	BSP_I2C_PORT_PIN_INIT(I2Cx_InitCfg);
	
	/* Enable I2C Peripheral*/
	FCG_Fcg1PeriphClockCmd(I2Cx_InitCfg->I2C_FCGx, ENABLE);
	return BSP_I2C_FUNC_EN(I2Cx_InitCfg);
}

void BSP_I2C_DeInitialize(I2Cx_InitCfg_Typedef * I2Cx_InitCfg)
{
    /* Initialize I2C port*/
    GPIO_SetFunc(I2Cx_InitCfg->PortPin.SCL_Port, I2Cx_InitCfg->PortPin.SCL_Pin,I2Cx_InitCfg->PortPin.SCL_Func);
    GPIO_SetFunc(I2Cx_InitCfg->PortPin.SDA_Port, I2Cx_InitCfg->PortPin.SDA_Pin, I2Cx_InitCfg->PortPin.SDA_Func);
    I2C_DeInit(I2Cx_InitCfg->CM_I2C_UNITx);
}

int32_t BSP_I2C_GetDevStatus(I2Cx_InitCfg_Typedef * I2Cx_InitCfg, uint16_t u16DevAddr)
{
    int32_t i32Ret;

    i32Ret = I2C_Start(I2Cx_InitCfg->CM_I2C_UNITx, BSP_I2C_TIMEOUT);
    if (LL_OK == i32Ret) {
        i32Ret = I2C_TransAddr(I2Cx_InitCfg->CM_I2C_UNITx, u16DevAddr, I2C_DIR_TX, BSP_I2C_TIMEOUT);

        if (LL_OK == i32Ret) {
            if (SET == I2C_GetStatus(I2Cx_InitCfg->CM_I2C_UNITx, I2C_FLAG_ACKR)) {
                i32Ret = LL_ERR;
            }
        }
    }
    (void)I2C_Stop(I2Cx_InitCfg->CM_I2C_UNITx, BSP_I2C_TIMEOUT);
    return i32Ret;
}

int32_t BSP_I2C_Write(I2Cx_InitCfg_Typedef * I2Cx_InitCfg,uint16_t u8DevAddr,uint16_t u16MemAddr, uint8_t u8RegLen, uint8_t *pu8Data, int32_t u32Size, uint32_t u32TimeOut,uint8_t Page_Len)
{
		int32_t i32Ret;

		uint8_t page_num = (u32Size/Page_Len)+1;
		I2C_Cmd(I2Cx_InitCfg->CM_I2C_UNITx, ENABLE);
		while(page_num --)
		{
			if (u32Size > 0)
			{
				//I2C_CR1 PE位和SWRST位状态复位: 	(PE=0 SWRST=1)通信复位：I2C内部的全部寄存器及内部状态复位 \
																					(SWRST=1 PE=1)内部状态复位：I2C_SR，I2C_DSR寄存器和内部状态机进行复位
				I2C_SWResetCmd(I2Cx_InitCfg->CM_I2C_UNITx, ENABLE);
				I2C_SWResetCmd(I2Cx_InitCfg->CM_I2C_UNITx, DISABLE);
				i32Ret = I2C_Start(I2Cx_InitCfg->CM_I2C_UNITx,u32TimeOut);
				if(LL_OK == i32Ret)
				{
						i32Ret = I2C_TransAddr(I2Cx_InitCfg->CM_I2C_UNITx, u8DevAddr, I2C_DIR_TX, u32TimeOut);

						if(LL_OK == i32Ret)
						{
								i32Ret = I2C_TransData(I2Cx_InitCfg->CM_I2C_UNITx, (uint8_t *)&u16MemAddr, u8RegLen, u32TimeOut);
								if(LL_OK == i32Ret)
								{
										if(u32Size >= Page_Len)
										{
											i32Ret = I2C_TransData(I2Cx_InitCfg->CM_I2C_UNITx, pu8Data, Page_Len, u32TimeOut);
										}
										else
										{
											i32Ret = I2C_TransData(I2Cx_InitCfg->CM_I2C_UNITx, pu8Data, u32Size, u32TimeOut);
										}
								}
						}
				}
				I2C_Stop(I2Cx_InitCfg->CM_I2C_UNITx,u32TimeOut);
			}
			
			u32Size -= Page_Len;
			u16MemAddr += Page_Len;
			pu8Data += Page_Len;
			
			if(u32Size > 0)
			DDL_DelayMS(5); //给足I2C器件翻页时间
		}
		
		I2C_Cmd(I2Cx_InitCfg->CM_I2C_UNITx, DISABLE);

		return i32Ret;
}


int32_t BSP_I2C_Read(I2Cx_InitCfg_Typedef * I2Cx_InitCfg, uint16_t u16DevAddr, const uint8_t *pu8Reg, uint8_t u8RegLen, uint8_t *pu8Buf, uint32_t u32Len)
{
    int32_t i32Ret;
	
		I2C_Cmd(I2Cx_InitCfg->CM_I2C_UNITx, ENABLE);
	
    I2C_SWResetCmd(I2Cx_InitCfg->CM_I2C_UNITx, ENABLE);
    I2C_SWResetCmd(I2Cx_InitCfg->CM_I2C_UNITx, DISABLE);
	
    i32Ret = I2C_Start(I2Cx_InitCfg->CM_I2C_UNITx, BSP_I2C_TIMEOUT);
    if (LL_OK == i32Ret) {
        i32Ret = I2C_TransAddr(I2Cx_InitCfg->CM_I2C_UNITx, u16DevAddr, I2C_DIR_TX, BSP_I2C_TIMEOUT);

        if (LL_OK == i32Ret) {
            i32Ret = I2C_TransData(I2Cx_InitCfg->CM_I2C_UNITx, pu8Reg, u8RegLen, BSP_I2C_TIMEOUT);
            if (LL_OK == i32Ret) {
                i32Ret = I2C_Restart(I2Cx_InitCfg->CM_I2C_UNITx, BSP_I2C_TIMEOUT);
                if (LL_OK == i32Ret) {
                    if (1UL == u32Len) {
                        I2C_AckConfig(I2Cx_InitCfg->CM_I2C_UNITx, I2C_NACK);
                    }

                    i32Ret = I2C_TransAddr(I2Cx_InitCfg->CM_I2C_UNITx, u16DevAddr, I2C_DIR_RX, BSP_I2C_TIMEOUT);
                    if (LL_OK == i32Ret) {
                        i32Ret = I2C_MasterReceiveDataAndStop(I2Cx_InitCfg->CM_I2C_UNITx, pu8Buf, u32Len, BSP_I2C_TIMEOUT);
                    }
                    I2C_AckConfig(I2Cx_InitCfg->CM_I2C_UNITx, I2C_ACK);
                }
            }
        }
    }

    if (LL_OK != i32Ret) {
        (void)I2C_Stop(I2Cx_InitCfg->CM_I2C_UNITx, BSP_I2C_TIMEOUT);
    }

    return i32Ret;
}
#else
static void i2c_Delay()
{
		for( int i = 0; i < 40; i++ ){
		__ASM("NOP");
	}	
}

void i2c_Port_Initialize(I2C_InitCfg_Typedef *I2C_InitCfg)
{
	I2C_InitCfg->I2C_SCL.InitHandler(&I2C_InitCfg->I2C_SCL);
	I2C_InitCfg->I2C_SDA.InitHandler(&I2C_InitCfg->I2C_SDA);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: i2c_Start
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(I2C_InitCfg_Typedef *I2C_InitCfg)
{
	I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)1);
	I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)1);
	i2c_Delay();
	I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)0);
	i2c_Delay();
	I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)0);
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Stop
*	功能说明: i2c_Stop
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(I2C_InitCfg_Typedef *I2C_InitCfg)
{
	I2C_InitCfg->I2C_SDA.IoSetOut(&I2C_InitCfg->I2C_SDA);
	
	I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)0);
	I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)0);
	i2c_Delay();
	I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)1);
	i2c_Delay();
	I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)1);
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU产生8个时钟，并发送1byte
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(I2C_InitCfg_Typedef *I2C_InitCfg,uint8_t _ucByte)
{
	for(uint8_t i = 0 ; i<8 ; i++)
	{
		if(_ucByte & 0x80) 
		{
			I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)1);
		}
		else
		{			
			I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)0);
		}
		_ucByte <<= 1;
		i2c_Delay();
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)1);
		i2c_Delay();
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)0);
		i2c_Delay();
	}
	I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)1);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU产生8个时钟，并读取1byte
*	形    参：无
*	返 回 值: value
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(I2C_InitCfg_Typedef *I2C_InitCfg)
{
	uint8_t i;
	
	uint8_t value = 0;
	
	I2C_InitCfg->I2C_SDA.IoSetIn(&I2C_InitCfg->I2C_SDA);
	for(i = 0;i < 8;i++)
	{
		value <<= 1;
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)1);
		i2c_Delay();
		if (I2C_InitCfg->I2C_SDA.GetbitHandler(&I2C_InitCfg->I2C_SDA))
		{
				value++;
		}
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)0);
		i2c_Delay();
	}
	I2C_InitCfg->I2C_SDA.IoSetOut(&I2C_InitCfg->I2C_SDA);
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生1个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(I2C_InitCfg_Typedef *I2C_InitCfg)
{
		uint8_t re;
		I2C_InitCfg->I2C_SDA.IoSetIn(&I2C_InitCfg->I2C_SDA);//数据总线设置为输入
		i2c_Delay();
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)1);	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
		i2c_Delay();

		if (I2C_InitCfg->I2C_SDA.GetbitHandler(&I2C_InitCfg->I2C_SDA))	/* CPU读取SDA口线状态 */
		{
				re = 0;
		}
		else
		{
				re = 1;
		}

		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)0);
		i2c_Delay();
		I2C_InitCfg->I2C_SDA.IoSetOut(&I2C_InitCfg->I2C_SDA);
		return re;
}
/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(I2C_InitCfg_Typedef *I2C_InitCfg)
{
		I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)0);	/* CPU驱动SDA = 0 */
		i2c_Delay();
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)1);	/* CPU产生1个时钟 */
		i2c_Delay();
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)0);
		i2c_Delay();
		I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)1);	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(I2C_InitCfg_Typedef *I2C_InitCfg)
{
		I2C_InitCfg->I2C_SDA.OutHandler(&I2C_InitCfg->I2C_SDA,(en_functional_state_t)1);	/* CPU驱动SDA = 1 */
		i2c_Delay();
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)1);	/* CPU产生1个时钟 */
		i2c_Delay();
		I2C_InitCfg->I2C_SCL.OutHandler(&I2C_InitCfg->I2C_SCL,(en_functional_state_t)0);
		i2c_Delay();
}
#endif


