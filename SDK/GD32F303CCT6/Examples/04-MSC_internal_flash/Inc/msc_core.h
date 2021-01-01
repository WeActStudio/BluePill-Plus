/*!
    \file  msc_core.h
    \brief the header file of USB MSC device class core functions

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    All rights reserved.

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

#ifndef MSC_CORE_H
#define MSC_CORE_H

#include "usbd_core.h"
#include "usbd_std.h"

#define STORAGE_LUN_NUM                   1

#define MSC_EPIN_SIZE                     MSC_DATA_PACKET_SIZE
#define MSC_EPOUT_SIZE                    MSC_DATA_PACKET_SIZE

#define BBB_GET_MAX_LUN                   0xFE
#define BBB_RESET                         0xFF

#define USB_LANGID_STRING_SIZE            0x04
#define USB_MSC_CONFIG_DESC_SIZE          32
#define USB_STR_DESC_MAX_SIZE             64

/* USB configuration descriptor struct */
typedef struct
{
    usb_descriptor_configuration_struct        Config;

    usb_descriptor_interface_struct            MSC_Interface;
    usb_descriptor_endpoint_struct             MSC_INEndpoint;
    usb_descriptor_endpoint_struct             MSC_OUTEndpoint;
} usb_descriptor_configuration_set_struct;

extern void* const usbd_strings[USB_STRING_COUNT];
extern const usb_descriptor_device_struct device_descripter;
extern usb_descriptor_configuration_set_struct configuration_descriptor;

/* function declarations */
/* initialize the MSC device */
usbd_status_enum msc_init (void *pudev, uint8_t config_index);
/* de-initialize the MSC device */
usbd_status_enum msc_deinit (void *pudev, uint8_t config_index);
/* handle the MSC class-specific and standard requests */
usbd_status_enum msc_req_handler (void *pudev, usb_device_req_struct *req);
/* handle data stage */
usbd_status_enum msc_data_handler (void *pudev, usbd_dir_enum rx_tx, uint8_t ep_id);

#endif  /* MSC_CORE_H */
