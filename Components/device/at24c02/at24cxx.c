#include "at24cxx.h"


#if !SOFT_I2C_ENABLE
stc_i2c_device_t at24c02 = {
		.I2Cx_InitCfg={
			.CM_I2C_UNITx = CM_I2C1,
			.I2C_FCGx = FCG1_PERIPH_I2C1,
			.PortPin = {
				.SCL_Port = GPIO_PORT_D,
				.SCL_Pin = GPIO_PIN_03,
				.SCL_Func = GPIO_FUNC_49,
				
				.SDA_Port = GPIO_PORT_F,
				.SDA_Pin = GPIO_PIN_10,
				.SDA_Func = GPIO_FUNC_48,
			},
		
			.stcI2cInit= {
				.u32Baudrate = 100000,
				.u32SclTime  = 0U,
				.u32ClockDiv = I2C_CLK_DIV16,
			},
			
			.If_Link_DMA = false,
		},
		
    .u32PageSize = 8,
    .u32Capacity = 256,
		.device_ID = 0x50,
		.Dev_mem_addr_len = 1,
		
    .fptr_Delay = DDL_DelayUS, 												//无RTOS
    .fptr_Init = BSP_I2CPortPinInit_and_I2CFuncEn,
    .fptr_DeInit = BSP_I2C_DeInitialize,
    .fptr_WritePage = BSP_I2C_Write,
    .fptr_Read = BSP_I2C_Read,
    .fptr_GetStatus = BSP_I2C_GetDevStatus,

};

void at24cxx_device_creat(stc_i2c_device_t * stc_i2c_device)
{
	stc_i2c_device->fptr_Init(&stc_i2c_device->I2Cx_InitCfg);
	if(stc_i2c_device->I2Cx_InitCfg.If_Link_DMA == true)
	{
		//Configure DMA for I2C
		;
	}
}

void at24cxx_device_write(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *wbuff,int32_t data_size)
{
	  uint16_t u16MemAddrTemp;
	if(stc_i2c_device->Dev_mem_addr_len == 1U)
    u16MemAddrTemp = dev_mem_addr;
	else
    u16MemAddrTemp = (uint16_t)((((uint32_t)dev_mem_addr >> 8) & 0xFFUL) + (((uint32_t)dev_mem_addr << 8) & 0xFF00UL));

	stc_i2c_device->fptr_WritePage(&stc_i2c_device->I2Cx_InitCfg,stc_i2c_device->device_ID,(uint16_t)u16MemAddrTemp,stc_i2c_device->Dev_mem_addr_len,wbuff,data_size,BSP_I2C_TIMEOUT,stc_i2c_device->u32PageSize);
}

void at24cxx_device_read(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *rbuff,uint32_t len)
{
	  uint16_t u16MemAddrTemp;
	if(stc_i2c_device->Dev_mem_addr_len == 1U)
    u16MemAddrTemp = dev_mem_addr;
	else
    u16MemAddrTemp = (uint16_t)((((uint32_t)dev_mem_addr >> 8) & 0xFFUL) + (((uint32_t)dev_mem_addr << 8) & 0xFF00UL));
	
	stc_i2c_device->fptr_Read(&stc_i2c_device->I2Cx_InitCfg,stc_i2c_device->device_ID,(const uint8_t *)&u16MemAddrTemp,stc_i2c_device->Dev_mem_addr_len,rbuff,len);
}
#else
	stc_i2c_device_t at24c02 = {
		.I2Cx_PortPinCfg={
			.I2C_SDA.io_obj.IO_PortSel = GPIO_PORT_F,
			.I2C_SDA.io_obj.IO_PinSel = GPIO_PIN_10,
			.I2C_SDA.io_obj.GpioElec = {
				.u16PinDir = PIN_DIR_OUT,
				.u16PinOutputType = PIN_OUT_TYPE_NMOS, //NMOS开漏输出
				.u16PinDrv = PIN_HIGH_DRV,
			},
				
			.I2C_SCL.io_obj.IO_PortSel = GPIO_PORT_D,
			.I2C_SCL.io_obj.IO_PinSel = GPIO_PIN_03,
			.I2C_SCL.io_obj.GpioElec = {
				.u16PinDir = PIN_DIR_OUT,
				.u16PinOutputType = PIN_OUT_TYPE_NMOS, //NMOS开漏输出
				.u16PinDrv = PIN_HIGH_DRV,
			},
		},
		
    .u32PageSize = 8,
    .u32Capacity = 256,
		.device_ID = 0xA0,
		.Dev_mem_addr_len = 1,
};
	
void at24cxx_device_creat(stc_i2c_device_t * stc_i2c_device)
{
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SCL.InitHandler = xs_GpioInit;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SCL.IoSetIn = xs_GpioSetIn;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SCL.IoSetOut = xs_GpioSetOut;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SCL.OutHandler = xs_GpioOutput;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SCL.ToggleHandler = xs_GpioToggle;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SCL.GetbitHandler = xs_GpioGetBit;
	
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SDA.InitHandler = xs_GpioInit;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SDA.IoSetIn = xs_GpioSetIn;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SDA.IoSetOut = xs_GpioSetOut;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SDA.OutHandler = xs_GpioOutput;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SDA.ToggleHandler = xs_GpioToggle;
	stc_i2c_device->I2Cx_PortPinCfg.I2C_SDA.GetbitHandler = xs_GpioGetBit;
	
	stc_i2c_device->fptr_PortPinInit = i2c_Port_Initialize;
	stc_i2c_device->fptr_Start = i2c_Start;
	stc_i2c_device->fptr_Stop = i2c_Stop;
	stc_i2c_device->fptr_SendByte = i2c_SendByte;
	stc_i2c_device->fptr_ReadByte = i2c_ReadByte;
	stc_i2c_device->fptr_WaitAck = i2c_WaitAck;
	stc_i2c_device->fptr_Ack = i2c_Ack;
	stc_i2c_device->fptr_Nack = i2c_NAck;
	
	stc_i2c_device->fptr_PortPinInit(&stc_i2c_device->I2Cx_PortPinCfg);
}

int8_t at24cxx_device_write(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *wbuff,int32_t data_size)
{
		uint8_t i;uint8_t current_page_left = 0;
	
		if (dev_mem_addr +data_size >  stc_i2c_device->u32Capacity)
			data_size = dev_mem_addr +data_size - stc_i2c_device->u32Capacity;
		
		uint8_t page_num = ((stc_i2c_device->u32PageSize*((dev_mem_addr + data_size)/stc_i2c_device->u32PageSize)) - (stc_i2c_device->u32PageSize*(dev_mem_addr/stc_i2c_device->u32PageSize)))/stc_i2c_device->u32PageSize;

		current_page_left = stc_i2c_device->u32PageSize - dev_mem_addr%stc_i2c_device->u32PageSize; //单页剩余的字节数
		
		if((dev_mem_addr + data_size)%stc_i2c_device->u32PageSize)
			page_num += 1;
		
		while(page_num --)
		{
			if (data_size > 0)
			{
				stc_i2c_device->fptr_Start(&stc_i2c_device->I2Cx_PortPinCfg);
				
				stc_i2c_device->fptr_SendByte(&stc_i2c_device->I2Cx_PortPinCfg,stc_i2c_device->device_ID | 0);
				
				if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg)){
					stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
					return LL_ERR;
				}
				stc_i2c_device->fptr_SendByte(&stc_i2c_device->I2Cx_PortPinCfg,dev_mem_addr | 0);

				if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg)){
						stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
						return LL_ERR;
				}
				
				if(data_size >= stc_i2c_device->u32PageSize)
				{
					if(current_page_left > 0)
					{
						for(i = 0; i < current_page_left; i++)
						{
								stc_i2c_device->fptr_SendByte(&stc_i2c_device->I2Cx_PortPinCfg,*(wbuff++));
							
								if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg)){
										stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
										return LL_ERR;
								}
						}
					}
					else
					{
						for(i = 0; i < stc_i2c_device->u32PageSize; i++)
						{
								stc_i2c_device->fptr_SendByte(&stc_i2c_device->I2Cx_PortPinCfg,*(wbuff++));
							
								if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg))
								{
										stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
										return LL_ERR;
								}
						}
					}
				}
				else
				{
					for(i = 0; i < data_size; i++)
					{
							stc_i2c_device->fptr_SendByte(&stc_i2c_device->I2Cx_PortPinCfg,*(wbuff++));

							if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg))
							{
									stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
									return LL_ERR;
							}
					}
				}
				
				stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
				
				if(current_page_left > 0)
				{
					data_size -= current_page_left;
					dev_mem_addr += current_page_left;
					current_page_left = 0;
				}
				else
				{
					data_size -= stc_i2c_device->u32PageSize;
					dev_mem_addr += stc_i2c_device->u32PageSize;
				}
				
				if(data_size > 0)
					DDL_DelayMS(5); //给足翻页时间
			}
		}
		return LL_OK;
}

int8_t at24cxx_device_read(stc_i2c_device_t * stc_i2c_device,uint32_t dev_mem_addr,uint8_t *rbuff,uint32_t data_size)
{
	uint8_t i;
		
	stc_i2c_device->fptr_Start(&stc_i2c_device->I2Cx_PortPinCfg);
	
	i2c_SendByte(&stc_i2c_device->I2Cx_PortPinCfg, stc_i2c_device->device_ID | 0);
	
	if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg))
	{
		stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
		return LL_ERR;
	}
	
	i2c_SendByte(&stc_i2c_device->I2Cx_PortPinCfg, dev_mem_addr | 0);

	if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg))
	{
			stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
			return LL_ERR;
	}
	
	stc_i2c_device->fptr_Start(&stc_i2c_device->I2Cx_PortPinCfg);
	
	i2c_SendByte(&stc_i2c_device->I2Cx_PortPinCfg, stc_i2c_device->device_ID | 1);

	if(!stc_i2c_device->fptr_WaitAck(&stc_i2c_device->I2Cx_PortPinCfg))
	{
			stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
			return LL_ERR;
	}
	
	for(i = 0; i < data_size; i++)
	{
			rbuff[i] = stc_i2c_device->fptr_ReadByte(&stc_i2c_device->I2Cx_PortPinCfg);//有指针越界风险，待优化
			stc_i2c_device->fptr_Ack(&stc_i2c_device->I2Cx_PortPinCfg);
	}
	
	stc_i2c_device->fptr_Nack(&stc_i2c_device->I2Cx_PortPinCfg);
	
	stc_i2c_device->fptr_Stop(&stc_i2c_device->I2Cx_PortPinCfg);
	
	return LL_OK;
}
#endif
