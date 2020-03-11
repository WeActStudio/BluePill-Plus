#include "gpio.h"


void gpio_config(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(PB2_Clock, ENABLE);
	
	GPIO_ResetBits(PB2_GPIO_Port,PB2_Pin);
	
  /* Configure PB2 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = PB2_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(PB2_GPIO_Port, &GPIO_InitStructure);
	
}


