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

void gpio_config(void);
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
	
	rcu_all_reset_flag_clear();
	
	systick_config();
	
	gpio_config();
	
	usart0_config();
		
	uint32_t tick1,tick2;
	tick1 = tick2 = get_tick();
	
	printf("\r\nStandby_WakeUP_Pin demo\r\n");
	printf("Press KEY to Enter Standby Mode\r\n");
	
	rcu_periph_clock_enable(RCU_PMU);
	
	pmu_wakeup_pin_disable();
	
	while(1)
	{
		
		if(get_tick() > tick1)
		{
			tick1 = get_tick() + 500 / SysTick_Tick;
			
			/* toggle LED */
			gpio_bit_write(LED_PB2_PORT,LED_PB2_PIN,(gpio_output_bit_get(LED_PB2_PORT,LED_PB2_PIN)) == SET?RESET:SET);

		}
		
		if(get_tick() > tick2)
		{
			tick2 = get_tick() + 10 / SysTick_Tick;
			
			static uint32_t PressTimeCount = 0;
			
			static FlagStatus IsPress = RESET;
			if(gpio_input_bit_get(KEY_PORT,KEY_PIN) != SET)
			{
				if(IsPress == SET)
				{
					IsPress = RESET;
					
					/* KEY click */
					printf("Enter Standby Mode, SW debug will lost\r\n");
					printf("Press KEY again to Quit Standby Mode && System Reset\r\n");
					
					delay_lp(100);
					gpio_deinit(LED_PB2_PORT);
					gpio_deinit(KEY_PORT);
					usart_deinit(USART0);
					systick_deinit();
					/* wakeup pin enable */
					pmu_wakeup_pin_enable();
					/* enter standbymode && wait for exit event */
					pmu_to_standbymode(WFI_CMD);
					
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
		}
		
		pmu_to_sleepmode (WFI_CMD);
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
