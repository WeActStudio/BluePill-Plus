#ifndef usbd_H
#define usbd_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "gd32f30x.h"
#include "usbd_std.h"

extern usbd_core_handle_struct  usb_device_dev;

void usbd_config(void);
void usbd_check_reset(void);

#ifdef __cplusplus
}
#endif

#endif
