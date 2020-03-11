#include "flash_access.h"

uint8_t flashx_init(void)
{
	uint32_t data;
	uint8_t result = 0;
	
	data = (NAND_FLASH_BASE_ADDRESS - 12);
	HAL_FLASH_Unlock();
	for(uint8_t i = 0;i<12;i+=4)
	{
		if(*(uint32_t *)(data+i) != *(uint32_t *)(UID_BASE+i))
		{
			if(*(uint32_t *)(data+i) == 0xFFFFFFFF)
			{
				HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,(data+i),*(uint32_t *)(UID_BASE+i));
			}
			else
			{
				while(1){}
			}
			result++;
		}
	}
	HAL_FLASH_Lock();
	return result;
}

/*!
    \brief      initialize the nand flash
    \param[in]  none
    \param[out] none
    \retval     status
  */
uint32_t  flash_init ()
{
		
	  HAL_FLASH_Unlock();
    
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
	  FLASH_EraseInitTypeDef pEraseInit;
	  uint32_t PAGEError;
    uint32_t i, page;
    uint32_t start_page = (write_addr / block_size) * block_size + NAND_FLASH_BASE_ADDRESS;
    uint32_t *ptrs = (uint32_t *)pBuf;

    page = block_num;
		
	  pEraseInit.NbPages = page;
	  pEraseInit.PageAddress = start_page;
	  pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	  HAL_FLASHEx_Erase(&pEraseInit,&PAGEError);
    for(; page > 0; page--){

        i = 0;

        do{
					  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,start_page,*ptrs++);
            start_page += 4;
        }while(++i < (block_size/4));
    }

    return 0;
}
