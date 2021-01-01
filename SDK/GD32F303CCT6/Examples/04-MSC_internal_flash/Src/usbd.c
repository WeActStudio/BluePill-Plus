#include "usbd.h"
#include "msc_core.h"
#include "bbb_scsi_driver.h"

#include "gpio.h"
#include "systick.h"

usbd_core_handle_struct  usb_device_dev;

void usbd_config(void)
{
	/* enable the power clock */
	rcu_periph_clock_enable(RCU_PMU);

	/* configure USB model clock from PLL clock */
	rcu_usb_clock_config(RCU_CKUSB_CKPLL_DIV2_5);

	/* enable USB APB1 clock */
	rcu_periph_clock_enable(RCU_USBD);

	usb_device_dev.dev_desc = (uint8_t *)&device_descripter;
	usb_device_dev.config_desc = (uint8_t *)&configuration_descriptor;
	usb_device_dev.strings = usbd_strings;
	usb_device_dev.class_init = msc_init;
	usb_device_dev.class_deinit = msc_deinit;
	usb_device_dev.class_req_handler = msc_req_handler;
	usb_device_dev.class_data_handler = msc_data_handler;
	usbd_core_init(&usb_device_dev);
	
	/* 1 bit for pre-emption priority, 3 bits for subpriority */
	nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);

	/* enable the USB low priority interrupt */
	nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn, 1, 0);
	
	/* now the usb device is connected */
	usb_device_dev.status = USBD_CONNECTED;
}

/*!
    \brief      release PA12 USB DP,RESET PA12
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usbd_check_reset(void)
{
	if(RESET == rcu_flag_get(RCU_FLAG_PORRST))
	{
		rcu_periph_clock_enable(RCC_AHBPeriph_GPIO_PULLUP);
		gpio_init(USB_PULLUP, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, USB_PULLUP_PIN);
		
		gpio_bit_reset(USB_PULLUP,USB_PULLUP_PIN);
		
		gpio_bit_set(LED_PB2_PORT,LED_PB2_PIN);
		delay(50/SysTick_Tick);
		gpio_bit_reset(LED_PB2_PORT,LED_PB2_PIN);
		delay(100/SysTick_Tick);
		gpio_bit_set(LED_PB2_PORT,LED_PB2_PIN);
		delay(50/SysTick_Tick);
		gpio_bit_reset(LED_PB2_PORT,LED_PB2_PIN);		
	}
	rcu_all_reset_flag_clear();
}

