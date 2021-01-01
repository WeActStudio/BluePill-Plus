#ifndef usart_H
#define usart_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "gd32f30x.h"
#include <stdio.h>

#define _value(value) #value
#define printf_name(name) printf(_value(> #name)),printf("\r\n");
#define printf_value(name,value)  printf(_value(> #name)),printf(" "),printf(#value),printf(" :%d\r\n",value);

void usart0_config(void);

#ifdef __cplusplus
}
#endif

#endif
