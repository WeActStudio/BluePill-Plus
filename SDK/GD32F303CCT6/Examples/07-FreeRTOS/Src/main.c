/* WeAct Studio */
/* GD32F303CCT6 Core Board */
/* Taobao: https://weactstudio.taobao.com */
/* Github: https://github.com/WeActTC */
/* Gitee:  https://gitee.com/WeAct-TC */

#include "gd32f30x.h"
#include <stdio.h>

#include "systick.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"                   // Keil::RTOS:FreeRTOS:Core
#include "task.h"                       // Keil::RTOS:FreeRTOS:Core
#include <limits.h>


TaskHandle_t LEDTaskHandle;
TaskHandle_t KEYTaskHandle;

void gpio_config(void);
void app_main (void *argument);
void thread_LED(void *argument);
void thread_KEY(void *argument);
void jump_to_app(uint32_t addr);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
	#ifdef HID_IAP
  /* Soft :GD32 MCU HID IAP Programmer, Bootloader Size :8KB */
  /* if use HID bootloader ,set ROM start addr 0x8002000 && set vector */
  /* if not use HID bootloader ,set ROM start addr 0x8000000 && Comment out the following code */
  // nvic_vector_table_set(NVIC_VECTTAB_FLASH,0x2000);
	nvic_vector_table_set(NVIC_VECTTAB_FLASH,0x2000);
	#endif

	SystemCoreClockUpdate();
	
	gpio_config();

	usart0_config();
	
	printf("FreeRTOS demo\r\n");
	
  xTaskCreate (app_main, "app_main", 64, NULL, tskIDLE_PRIORITY+1, NULL);

  vTaskStartScheduler();
	
	while(1){}
}



/*----------------------------------------------------------------------------
 *      Main: Initialize and start the application
 *---------------------------------------------------------------------------*/
void app_main (void *argument) 
{
	xTaskCreate (thread_LED, "LEDTask", 64, NULL, tskIDLE_PRIORITY+1, &LEDTaskHandle);
  xTaskCreate (thread_KEY, "KEYTask", 64, NULL, tskIDLE_PRIORITY+1, &KEYTaskHandle);
	
	//vTaskDelay (portMAX_DELAY);
	
	/* delete self task */
	vTaskDelete(NULL);
  while(1);
}

void thread_LED(void *argument)
{
	uint32_t ulNotifiedValue;
	FlagStatus sw_led = SET;
	//uint8_t pcWriteBuffer[256];
	while(1)
	{ 
		if(xTaskNotifyWait(0,ULONG_MAX,&ulNotifiedValue , 500) != pdTRUE)  /* wait for an event flag 0x0001 */
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
			if(ulNotifiedValue &0x01)
			{
				sw_led = sw_led == SET?RESET:SET;
				
				printf("Hello,WeAct Studio\r\n");
				printf("Hello,FreeRTOS\r\n");
			}
		}
	}
}

void thread_KEY(void *argument)
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
					
					xTaskNotify(LEDTaskHandle,0x01,eSetValueWithOverwrite);
					
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
						
						systick_deinit();
						gpio_deinit(LED_PB2_PORT);
						gpio_deinit(KEY_PORT);
						jump_to_app(ISP_ADDR);
					}
				}
			}
		vTaskDelay(10);
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
