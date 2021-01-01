#include "rtc.h"
#include <stddef.h>

/*!
    \brief      configure the RTC
    \param[in]  config: 1 RTC not yet configured
												0 RTC configured
								interrupt:  RTC_INT_SECOND   second interrupt enable
														RTC_INT_ALARM    alarm interrupt enable
														RTC_INT_OVERFLOW overflow interrupt enable
														NULL 						 not config interrupt
    \param[out] none
    \retval     none
*/
void rtc_config(uint8_t config,uint32_t interrupt)
{
    /* enable PMU and BKPI clocks */
		if(config == 1) 
			rcu_periph_clock_enable(RCU_BKPI);
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();

    if(config == 1) 
		{
			/* reset backup domain */
			bkp_deinit();

			/* enable LXTAL */
			rcu_osci_on(RCU_LXTAL);
			/* wait till LXTAL is ready */
			rcu_osci_stab_wait(RCU_LXTAL);
			
			/* select RCU_LXTAL as RTC clock source */
			rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);

			/* enable RTC Clock */
			rcu_periph_clock_enable(RCU_RTC);
		}

    /* wait for RTC registers synchronization */
    rtc_register_sync_wait();

    /* wait until last write operation on RTC registers has finished */
    rtc_lwoff_wait();
		
		if(interrupt != NULL) 
		{
			nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
			nvic_irq_enable(RTC_IRQn,1,0);
			
			/* enable the RTC second interrupt*/
			rtc_interrupt_enable(interrupt);

			/* wait until last write operation on RTC registers has finished */
			rtc_lwoff_wait();
		}
		if(config == 1) 
		{
			/* set RTC prescaler: set RTC period to 1s */
			rtc_prescaler_set(32767);

			/* wait until last write operation on RTC registers has finished */
			rtc_lwoff_wait();
		}
}

/*!
    \brief      set rtc time 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_time_set(rtc_time_t rtc_time)
{
	uint32_t tmp;
	tmp = rtc_time.hour*3600 + rtc_time.min*60 + rtc_time.sec;
	/* wait until last write operation on RTC registers has finished */
	rtc_lwoff_wait();
	/* change the current time */
	rtc_counter_set(tmp);
	/* wait until last write operation on RTC registers has finished */
	rtc_lwoff_wait();
}

/*!
    \brief      get rtc time 
    \param[in]  none
    \param[out] none
    \retval     none
*/
rtc_time_t rtc_time_get(void)
{
	uint32_t tmp;
	rtc_time_t time;
	
	tmp = rtc_counter_get();
	/* compute  hours */
	time.hour = tmp / 3600;
	/* compute minutes */
	time.min = (tmp % 3600) / 60;
	/* compute seconds */
	time.sec = (tmp % 3600) % 60;
	
	return time;
}

#include "usart.h"

void rtc_demo (void)
{
	printf_name(RTC Demo);
	if (bkp_read_data(BKP_DATA_0) != 0xA5A5)
	{
		printf("RTC not yet configured ...\r\n");
		
		rtc_time_t time_set;
		rtc_config(1,RTC_INT_SECOND);
		time_set.hour = 12;
		time_set.min = 0;
		time_set.sec = 0;
		rtc_time_set(time_set);
		bkp_write_data(BKP_DATA_0, 0xA5A5);
		
	}
	else
	{
		printf("RTC configured ... \r\n");
		rtc_config(0,RTC_INT_SECOND);
	}
}
