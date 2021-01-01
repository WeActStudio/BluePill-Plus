/*!
    /file  usbd_conf.h
    /brief usb device driver basic configuration

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

#ifndef USBD_CONF_H
#define USBD_CONF_H

#include "gd32f30x.h"

/* define if low power mode is enabled; it allows entering the device into DEEP_SLEEP mode
   following USB suspend event and wakes up after the USB wakeup event is received. */
#define USB_DEVICE_LOW_PWR_MODE_SUPPORT

//#define USE_EXTERNAL_MEDIA
#define USE_INTERNAL_MEDIA

#define USBD_CFG_MAX_NUM              1
#define USBD_ITF_MAX_NUM              1
#define USB_STRING_COUNT              4

#define PAGE_SIZE                     ((uint32_t)(2048))
#define NAND_FLASH_BASE_ADDRESS       ((uint32_t)(0x08000000 + 0x05000))
#define NAND_FLASH_END_ADDRESS        ((uint32_t)(0x08000000 + 0x3FFFF))

/* endpoint count used by the MSC device */
#define MSC_IN_EP                     EP1_IN
#define MSC_OUT_EP                    EP2_OUT
#define MSC_DATA_PACKET_SIZE          64
#define MSC_MEDIA_PACKET_SIZE         PAGE_SIZE

#define BLOCK_SIZE                    PAGE_SIZE
#define BLOCK_NUM                     ((NAND_FLASH_END_ADDRESS - NAND_FLASH_BASE_ADDRESS) / BLOCK_SIZE)

/* endpoint count used by the MSC device */
#define EP_COUNT                      (3)

/* base address of the allocation buffer, used for buffer descriptor table and packet memory */
#define BUFFER_ADDRESS                (0x0000)

#endif /* USBD_CONF_H */
