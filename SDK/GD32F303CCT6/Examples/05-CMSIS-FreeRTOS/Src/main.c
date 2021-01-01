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

#include "cmsis_os2.h"

osThreadId_t LEDTaskHandle;
osThreadId_t KEYTaskHandle;

const osThreadAttr_t LEDTaskAttr={
	.name = "Led_Task",
	.stack_size = 128,
	.priority = osPriorityNormal,
};
const osThreadAttr_t KEYTaskAttr={
	.name = "Key_Task",
	.stack_size = 128,
	.priority = osPriorityNormal,
};

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
	
	printf("\r\nCMSIS-FreeRTOS demo\r\n");
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  if (osKernelGetState() == osKernelReady) {
    osKernelStart();                    // Start thread execution
  }
	
	while(1){}
}

/*----------------------------------------------------------------------------
 *      Main: Initialize and start the application
 *---------------------------------------------------------------------------*/
void app_main (void *argument) 
{
  LEDTaskHandle = osThreadNew(thread_LED, NULL, &LEDTaskAttr);
	KEYTaskHandle = osThreadNew(thread_KEY, NULL, &KEYTaskAttr);

  //osDelay(osWaitForever);
	
	osThreadTerminate(osThreadGetId());
	
  while(1);
}

void thread_LED(void *argument)
{
	int32_t result;
	FlagStatus sw_led = SET;
	
	while(1)
	{
		result = osThreadFlagsWait(0x0001, osFlagsWaitAny, 500);   /* wait for an event flag 0x0001 */
		if(result <  0)
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
			if(result &0x0001)
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
					
					osThreadFlagsSet(LEDTaskHandle,0x0001);
					
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
		osDelay(10);
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
