/* WeAct Studio */
/* GD32F303CCT6 Core Board */
/* Taobao: https://weactstudio.taobao.com */
/* Github: https://github.com/WeActTC */
/* Gitee:  https://gitee.com/WeAct-TC */

#include "usb_iap_core.h"
#include "usbd_hw.h"
#include "systick.h"
#include "gpio.h"

#define KEY_RCU  RCU_GPIOA
#define KEY_PORT GPIOA
#define KEY_PIN GPIO_PIN_0

#define LED_PB2_RCU  RCU_GPIOB
#define LED_PB2_PORT GPIOB
#define LED_PB2_PIN  GPIO_PIN_2

usb_dev usbd_iap;

app_func application;
uint32_t app_address;

void jump_to_app(uint32_t addr);

/*!
    \brief      main routine
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	
	/* enable the KEY clock */
	rcu_periph_clock_enable(KEY_RCU);
	rcu_periph_clock_enable(RCU_AF);
	
	/* configure button pin as input */
	gpio_init(KEY_PORT, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ, KEY_PIN);

	/* KEY must be pressed when power on */
	if(gpio_input_bit_get(KEY_PORT,KEY_PIN) != SET) {	
		jump_to_app(APP_LOADED_ADDR);
	}

	systick_config();

	gpio_config();

	usb_reset();

	/* system clocks configuration */
	rcu_config();

	/* USB device configuration */
	usbd_init(&usbd_iap, &iap_desc, &iap_class);

	/* NVIC configuration */
	nvic_config();

	/* enabled USB pull-up */
	usbd_connect(&usbd_iap);

	uint32_t tick1 = get_tick();

	while(gpio_input_bit_get(KEY_PORT,KEY_PIN) == SET)
	{
		gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,(gpio_output_bit_get(LED_PB2_PORT,LED_PB2_PIN)) == SET?RESET:SET);
		delay_lp(80/SysTick_Tick);
	}
	while (1)
	{
		if(gpio_input_bit_get(KEY_PORT,KEY_PIN) == SET) 
		{
			gpio_bit_reset(LED_PB2_PORT,LED_PB2_PIN);
			while(gpio_input_bit_get(KEY_PORT,KEY_PIN) == SET)
			{
				gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,(gpio_output_bit_get(LED_PB2_PORT,LED_PB2_PIN)) == SET?RESET:SET);
				delay_lp(80/SysTick_Tick);
			}
			delay_lp(100/SysTick_Tick);
			/* goto APP */
			NVIC_SystemReset();
		}
		else
		{
			/* PB2 Breathing Lamp test */
			static uint8_t pwmset;
			static uint16_t time;
			static uint8_t timeflag;
			static uint8_t timecount;
			if((get_tick() - tick1 >= 1 ))
			{
				tick1 = get_tick();
				
				 /* Breathing Lamp */
				if(timeflag == 0)
				{
					time ++;
					if(time >= 1600) timeflag = 1;
				}
				else
				{
					time --;
					if(time == 0) timeflag = 0;
				}
				
				/* Duty Cycle Setting */
				pwmset = time/80;

				/* 20ms Pulse Width */
				if(timecount > 20) timecount = 0;
				else timecount ++;

				if(timecount >= pwmset ) gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,RESET);
				else gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,SET);
			 }
		}
		pmu_to_sleepmode(WFI_CMD);
	}
}

void jump_to_app(uint32_t addr)
{
	uint32_t app_address;
	void* (*application)(void);
	
	/* test if user code is programmed starting from address */
	if (((*(__IO uint32_t*)addr) & 0x2FFE0000) == 0x20000000)
	{
		app_address = *(__IO uint32_t*) (addr + 4);
		application = (void* (*)(void)) app_address;

		/* initialize user application's stack pointer */
		__set_MSP(*(__IO uint32_t*) addr);
		
		rcu_deinit();
		
		/* jump to user application */
		application();
	}
}

