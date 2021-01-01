/*!
    \file  flash_access.c
    \brief flash access functions

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

#include "usbd_conf.h"
#include "flash_access.h"

/* pages 0 and 1 base and end addresses */
uint32_t bak[128];

/*!
    \brief      initialize the nand flash
    \param[in]  none
    \param[out] none
    \retval     status
  */
uint32_t  flash_init ()
{
    fmc_unlock();

    return 0;
}

/*!
    \brief      read data from multiple blocks of nand flash
    \param[in]  pBuf: pointer to user buffer
    \param[in]  read_addr: address to be read
    \param[in]  block_size: size of block
    \param[in]  block_num: number of block
    \param[out] none
    \retval     status
*/
uint32_t  flash_read_multi_blocks (uint8_t *pBuf, uint32_t read_addr, uint16_t block_size, uint32_t block_num)
{
    uint32_t i;
    uint8_t *pSource = (uint8_t *)(read_addr + NAND_FLASH_BASE_ADDRESS);

    /* Data transfer */
    while (block_num--) {
        for (i = 0; i < block_size; i++) {
            *pBuf++ = *pSource++;
        }
    }

    return 0;
}

/*!
    \brief      write data to multiple blocks of flash
    \param[in]  pBuf: pointer to user buffer
    \param[in]  write_addr: address to be write
    \param[in]  block_size: block size
    \param[in]  block_num: number of block
    \param[out] none
    \retval     status
*/
uint32_t flash_write_multi_blocks (uint8_t *pBuf,
                           uint32_t write_addr,
                           uint16_t block_size,
                           uint32_t block_num)
{
    uint32_t i, page;
    uint32_t start_page = (write_addr / PAGE_SIZE) * PAGE_SIZE + NAND_FLASH_BASE_ADDRESS;
    uint32_t *ptrs = (uint32_t *)pBuf;

    page = block_num;

    for(; page > 0; page--){
        fmc_page_erase(start_page);

        i = 0;

        do{
            fmc_word_program(start_page, *ptrs++);
            start_page += 4;
        }while(++i < 512);
    }

    return 0;
}
