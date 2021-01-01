/*!
    \file    gd32f30x_usbd_hw.c
    \brief   usbd hardware configure file

    \version 2020-08-01, V3.0.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "usbd_hw.h"

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    uint32_t system_clock = rcu_clock_freq_get(CK_SYS);

    /* enable USB pull-up pin clock */ 
    rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_PULLUP);

    if (48000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1);
    } else if (72000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV1_5);
    } else if (96000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2);
    } else if (120000000U == system_clock) {
        rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5);
    } else {
        /* reserved */
    }

    /* enable USB APB1 clock */
    rcu_periph_clock_enable(RCU_USBD);
}

/*!
    \brief      release PA12 USB DP,RESET PA12
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usb_reset(void)
{
	if(RESET == rcu_flag_get(RCU_FLAG_PORRST))
	{
		rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_PULLUP);
		gpio_init(USB_PULLUP, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, USB_PULLUP_PIN);
		
		gpio_bit_reset(USB_PULLUP,USB_PULLUP_PIN);
		
		delay(250);
	}
	rcu_all_reset_flag_clear();
}

/*!
    \brief      configure interrupt priority
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    /* 2 bits for preemption priority, 2 bits for subpriority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);

    /* enable the USB low priority interrupt */
    nvic_irq_enable((uint8_t)USBD_LP_CAN0_RX0_IRQn, 2U, 0U);

    /* enable the USB low priority interrupt */
    nvic_irq_enable((uint8_t)USBD_HP_CAN0_TX_IRQn, 1U, 0U);
}
