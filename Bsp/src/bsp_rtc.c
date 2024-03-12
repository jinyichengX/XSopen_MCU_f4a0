#include "bsp_rtc.h"


UTC8_Time_Typedef UTC8_Time;

/**
 * @brief  RTC calendar configuration.
 * @param  None
 * @retval None
 */
void BSP_RTC_CalendarConfig(void)
{
    stc_rtc_date_t stcRtcDate;
    stc_rtc_time_t stcRtcTime;

    /* Date configuration */
    stcRtcDate.u8Year    = 22U;
    stcRtcDate.u8Month   = 12;
    stcRtcDate.u8Day     = 22U;
    stcRtcDate.u8Weekday = 4;

    /* Time configuration */
    stcRtcTime.u8Hour   = 21U;
    stcRtcTime.u8Minute = 59U;
    stcRtcTime.u8Second = 55U;
    stcRtcTime.u8AmPm   = RTC_HOUR_24H;

    if (LL_OK != RTC_SetDate(RTC_DATA_FMT_DEC, &stcRtcDate)) {
			/* error handle */
       ;
    }

    if (LL_OK != RTC_SetTime(RTC_DATA_FMT_DEC, &stcRtcTime)) {
			/* error handle */
        ;
    }
}

void BSP_RTC_Config(void)
{
	stc_rtc_init_t stcRtcInit;
	/* RTC stopped */
	if (DISABLE == RTC_GetCounterState()) {
			/* Reset the VBAT area */
			PWC_VBAT_Reset();
			/* Reset RTC counter */
			if (LL_ERR_TIMEOUT == RTC_DeInit()) {
				/* error handle */
					;
			} else {
					/* Configure structure initialization */
					(void)RTC_StructInit(&stcRtcInit);

					/* Configuration RTC structure */
					stcRtcInit.u8ClockSrc   = RTC_CLK_SRC_LRC;
					stcRtcInit.u8HourFormat = RTC_HOUR_FMT_24H;
					stcRtcInit.u8IntPeriod  = RTC_INT_PERIOD_PER_SEC;
					(void)RTC_Init(&stcRtcInit);

//					/* Update date and time */
//					BSP_RTC_CalendarConfig();
//					/* Enable period interrupt */
//					RTC_IntCmd(RTC_INT_PERIOD, ENABLE);
					/* Startup RTC count */
					RTC_Cmd(ENABLE);
			}
	}
}

void BSP_RTC_Get_DateAndTime(UTC8_Time_Typedef * UTC8_Time)
{
	RTC_GetDate(RTC_DATA_FMT_DEC, &UTC8_Time->stcCurrentDate);
	RTC_GetTime(RTC_DATA_FMT_DEC, &UTC8_Time->stcCurrentTime);
}

#if USE_CALENDAR
static uint8_t BSP_CalcFebruaryDays(uint16_t year)
{
  if((year%4==0 && year%100!=0)||year%400==0)//判断闰年的条件 
  {
		return 29;
  } 
  else
  {
		return 28;
  } 
}

uint8_t BSP_CalcCurrentMonthDays(uint16_t _year, uint8_t _mon)
{
	uint8_t yy = _year-((_year/100)*100);
	uint8_t mm = _mon;
	
	if(BSP_CalcFebruaryDays(yy) == 29) //若今年是闰年
	{
		if(mm == 2)
		{
			return 29;
		}else if((mm == 2) || (mm == 4) || (mm == 6) || (mm == 9) || (mm == 11))
		{
			return 30;
		}else
		{
			return 31;
		}
	}
	else
	{
		if(mm == 2)
		{
			return 28;
		}else if((mm == 2) || (mm == 4) || (mm == 6) || (mm == 9) || (mm == 11))
		{
			return 30;
		}else
		{
			return 31;
		}
	}
}

static uint8_t BSP_CalcLastMonthDays(uint16_t _year, uint8_t _mon)
{
	uint16_t yy = _year-((_year/100)*100);
	uint8_t mm = _mon;
	if(mm == 1)
	{
		yy -= 1;
		mm = 12;
	}
	else
	{
		mm -= 1;
	}
	if(BSP_CalcFebruaryDays(yy) == 29) //若今年或去年是闰年
	{
		if(mm == 2)
		{
			return 29;
		}else if((mm == 2) || (mm == 4) || (mm == 6) || (mm == 9) || (mm == 11))
		{
			return 30;
		}else
		{
			return 31;
		}
	}
	else
	{
		if(mm == 2)
		{
			return 28;
		}else if((mm == 2) || (mm == 4) || (mm == 6) || (mm == 9) || (mm == 11))
		{
			return 30;
		}else
		{
			return 31;
		}
	}
	
}

//数据格式为十进制yymm
void BSP_RTC_CalcLastSeveralMonths(uint16_t _year, uint8_t _mon,uint8_t _mon_num)
{
	int8_t i = 0;
	uint8_t year_date = _year-((_year/100)*100);
	uint8_t month_date = _mon;
	
	if(_mon_num == 0)
		return;
	
	volatile uint32_t _mon_arr[_mon_num];
	
	for(i = _mon_num-1;i >= 0;i --)
	{
		_mon_arr[i] = month_date+year_date*100;
		if(month_date == 1)
		{
			month_date = 12;
			year_date -= 1;
		}else{
			month_date = month_date - 1;
		}
	}
}

//数据格式为十进制yymmdd
void BSP_RTC_CalcLastSeveralDays(uint16_t _year, uint8_t _mon, uint8_t _day,uint8_t _day_num)
{
	uint8_t year_date = _year-((_year/100)*100);
	uint8_t month_date = _mon;
	uint8_t day_date = _day;
	int8_t i = 0;
	
	if(_day_num == 0)
		return;
	
	uint32_t _day_arr[_day_num];
	//刷新day_date_arr中的数据
	for(i = _day_num - 1;i >= 0;i --)
	{
		_day_arr[i] = day_date+month_date*100+year_date*10000;
		if(day_date == 1)
		{
			if(month_date == 1)
			{
				month_date = 12;
				year_date -= 1;
			}
			else
			{
				month_date = month_date - 1;
			}
			day_date = BSP_CalcLastMonthDays(2000+_day_arr[i]/10000,(_day_arr[i]-(((2000+_day_arr[i]/10000)-2000)*10000))/100);
		}
		else
		{
			day_date -= 1;
		}
	}
}
/**
 * @brief  RTC_CalcWeek.
 * @note   Calculate what day is it today.
 * @param  _year _mon _day(example:1949 10 1)
 * @retval 1~7,6 and 7 repesent Saturday and Sunday independently.
 */
uint8_t BSP_RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day)
{
	uint8_t y, c, m, d;
	int16_t w;

	if (_mon >= 3)
	{
		m = _mon;
		y = _year % 100;
		c = _year / 100;
		d = _day;
	}
	else
	{
		m = _mon + 12;
		y = (_year - 1) % 100;
		c = (_year - 1) / 100;
		d = _day;
	}

	w = y + y / 4 +  c / 4 - 2 * c + ((uint16_t)26*(m+1))/10 + d - 1;
	if (w == 0)
	{
		w = 7;
	}
	else if (w < 0)
	{
		w = 7 - (-w) % 7;
	}
	else
	{
		w = w % 7;
	}
	
	if (w == 0)
	{
		w = 7;
	}
	
	return w;
}
#endif

