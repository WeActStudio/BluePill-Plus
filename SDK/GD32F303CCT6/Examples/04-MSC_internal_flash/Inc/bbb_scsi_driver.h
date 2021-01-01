/*!
    \file  bbb_scsi_driver.h
    \brief the header file of the bbb_scsi_driver.c

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

#ifndef BBB_SCSI_DRIVER_H
#define BBB_SCSI_DRIVER_H

#include "usbd_std.h"

#define BBB_CBW_SIGNATURE              0x43425355
#define BBB_CBW_LENGTH                 31

#define BBB_CSW_SIGNATURE              0x53425355
#define BBB_CSW_LENGTH                 13

#define SCSI_CMD_LENGTH                16

/* transport stage define */
#define COMMAND_STAGE                  0x00
#define DATA_IN_STAGE                  0x01
#define DATA_OUT_STAGE                 0x02
#define STATUS_STAGE                   0x03

/* SCSI command define */
#define INQUIRY                        0x12
#define READ_FORMAT_CAPACITIES         0x23
#define READ_CAPACITY                  0x25
#define READ_10                        0x28
#define WRITE_10                       0x2A
#define REQUEST_SENSE                  0x03
#define TEST_UNIT_READY                0x00
#define SCSI_MODE_SENSE6               0x1A
#define SCSI_ALLOW_MEDIUM_REMOVAL      0x1EU
#define SCSI_START_STOP_UNIT           0x1BU

/* the array length define */
#define DISK_INQUIRY_DATA_LENGTH       36
#define FORMAT_CAPACITIES_DATA_LENGTH  12
#define READ_CAPACITIES_DATA_LENGTH    8
#define REQUEST_SENSE_DATA_LENGTH      18
#define SENSE6_DATA_LENGTH             8

#define USB_PULLUP                      GPIOA
#define USB_PULLUP_PIN                  GPIO_PIN_12
#define RCC_AHBPeriph_GPIO_PULLUP       RCU_GPIOA

typedef struct
{
    uint32_t dCBWSignature;
    uint32_t dCBWTag;
    uint32_t dCBWDataTransferLength;
    uint8_t  bmCBWFlags;
    uint8_t  bCBWLUN;
    uint8_t  bCBWCBLength;
    uint8_t  CBWCB[16];
} bbb_cbw_struct;

typedef struct
{
    uint32_t dCSWSignature;
    uint32_t dCSWTag;
    uint32_t dCSWDataResidue;
    uint8_t  bCSWStatus;
} bbb_csw_struct;

/* function declarations */
/* parse the SCSI commands from the CBW */
void scsi_command_parse(void);
/* set the CSW struct */
void bbb_set_csw(uint32_t csw_tag, uint32_t csw_data_residue, uint8_t csw_status);
/* report array to host */
void bbb_report_array_to_host(void *pudev, uint8_t *p_data, uint32_t report_length);
/* process the scsi command */
void process_scsi_command(void *pudev);

#endif /* BBB_SCSI_DRIVER_H */
