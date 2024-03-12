#ifndef __BSP_RTC_H__
#define __BSP_RTC_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
#endif

#include "hc32_ll_rtc.h"
#include "hc32_ll.h"
#include "hc32_ll_pwc.h"

#define USE_CALENDAR 1 //使用日历功能（由当日时间推算出当月的日期,当前只能推算当日前n天的日期，推算当日后n天的功能待做）

typedef struct UTC8_Time_Struct
{
	stc_rtc_date_t stcCurrentDate;
  stc_rtc_time_t stcCurrentTime;
}UTC8_Time_Typedef;
	
extern UTC8_Time_Typedef UTC8_Time;
extern void BSP_RTC_Config(void);
extern void BSP_RTC_CalendarConfig(void);
extern void BSP_RTC_Get_DateAndTime(UTC8_Time_Typedef * UTC8_Time);
extern uint8_t BSP_RTC_CalcWeek(uint16_t _year, uint8_t _mon, uint8_t _day);
#ifdef __cplusplus
}
#endif
#endif /* __BSP_RTC_H__ */


