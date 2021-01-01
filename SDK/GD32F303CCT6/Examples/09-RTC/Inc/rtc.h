#ifndef rtc_H
#define rtc_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "gd32f30x.h"

typedef struct
{
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
}rtc_time_t;

void rtc_config(uint8_t config,uint32_t interrupt);
void rtc_time_set(rtc_time_t rtc_time);
rtc_time_t rtc_time_get(void);

void rtc_demo (void);

#ifdef __cplusplus
}
#endif

#endif
