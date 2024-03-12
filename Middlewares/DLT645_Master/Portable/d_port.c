#include "dlt645.h"
#include "dlt645Conf.h"
#include "dlt645_data.h"
#include <cmsis_armcc.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "stream_buffer.h"
#include "timers.h"
__WEAK void dlt_utly_t(uint8_t * buf,uint8_t len)
{
	//usr send function
	return;
}

__WEAK int dlt_utly_r(struct dlt645 * dlt645,uint8_t * buf)
{
	int rec_s;
	//usr recv function
	return rec_s;
}

