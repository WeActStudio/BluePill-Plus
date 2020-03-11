#ifndef __gpio_H
#define __gpio_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"

#define PB2_Pin       GPIO_Pin_2
#define PB2_Clock     RCC_APB2Periph_GPIOB
#define PB2_GPIO_Port GPIOB

void gpio_config(void);

#ifdef __cplusplus
}
#endif

#endif
