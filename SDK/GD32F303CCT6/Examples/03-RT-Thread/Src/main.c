/* WeAct Studio */
/* GD32F303CCT6 Core Board */
/* Taobao: https://weactstudio.taobao.com */
/* Github: https://github.com/WeActTC */
/* Gitee:  https://gitee.com/WeAct-TC */

#include "gd32f30x.h"
#include <stdio.h>

#include "usart.h"
#include "gpio.h"

#include <rtthread.h>
#include <rthw.h>

rt_thread_t LED_thread = RT_NULL;
rt_thread_t KEY_Thread = RT_NULL;
rt_sem_t LED_sem_SW;

static void thread_LED(void *parameter);
static void thread_KEY(void *parameter);
void gpio_config(void);
void jump_to_app(uint32_t addr);

int main(void)
{
   /* Soft :GD32 MCU HID IAP Programmer, Bootloader Size :8KB */
   /* if use HID bootloader ,set ROM start addr 0x8002000 && set vector */
   /* if not use HID bootloader ,set ROM start addr 0x8000000 && Comment out the following code */
   // nvic_vector_table_set(NVIC_VECTTAB_FLASH,0x2000);
	 /* see RTOS/board.c */

	gpio_config();
	
	usart0_config();
	
	LED_thread = rt_thread_create("LED", thread_LED,RT_NULL , 128, 5, 5);
	KEY_Thread = rt_thread_create("KEY", thread_KEY,RT_NULL , 128, 5, 5);
	
	LED_sem_SW = rt_sem_create("LED_SW",0,RT_IPC_FLAG_FIFO);
	
	rt_thread_startup(LED_thread);
	rt_thread_startup(KEY_Thread);
	
	printf("RT-Thread Demo\r\n");
	
	rt_thread_t self;
	self = rt_thread_self();
  rt_thread_delete(self);
}

static void thread_LED(void *parameter)
{
	static rt_err_t result;
	FlagStatus sw_led = SET;
	
	while(1)
	{
		result = rt_sem_take(LED_sem_SW,  rt_tick_from_millisecond(500));
		if(result !=  RT_EOK)
		{
			if(sw_led == SET)
			{
				/* toggle LED */
				gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,(gpio_output_bit_get(LED_PB2_PORT,LED_PB2_PIN)) == SET?RESET:SET);
			}
			else
			{
				gpio_bit_reset(LED_PB2_PORT,LED_PB2_PIN);
			}
		}
		else
		{
			sw_led = sw_led == SET?RESET:SET;
			printf("Hello,WeAct Studio\r\n");
			printf("Hello,RT-Thread\r\n");
		}
	}
}

static void thread_KEY(void *parameter)
{
	uint32_t PressTimeCount = 0;
	FlagStatus IsPress = RESET;
	while(1)
	{
			if(gpio_input_bit_get(KEY_PORT,KEY_PIN) != SET)
			{
				if(IsPress == SET)
				{
					IsPress = RESET;
					
					/* KEY click */
					
					rt_sem_release(LED_sem_SW);
					
					PressTimeCount = 0;
				}
			}
			else
			{
				if(IsPress == RESET)
				{
					IsPress = SET;
				}
				PressTimeCount ++;
				
				/*0.5s KEY long press */
				if(PressTimeCount >= 50)
				{
					/* long press , system reset and enter the IAP bootloader */
					if(SCB->VTOR != 0)
					{
						printf("Enter IAP Bootloader ...\r\n");
						NVIC_SystemReset();
					}
					else
					{
						printf("Enter system ISP Bootloader ...\r\n");
						
						#define ISP_ADDR ((uint32_t)0x1FFFF000)
						#include "systick.h"
            
						systick_deinit();
						gpio_deinit(LED_PB2_PORT);
						gpio_deinit(KEY_PORT);
						jump_to_app(ISP_ADDR);
					}
				}
			}
		rt_thread_delay(rt_tick_from_millisecond(10));
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

		/* jump to application */
		application();
	}
}
