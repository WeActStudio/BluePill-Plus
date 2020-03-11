#ifndef FLASH_ACCESS_H
#define FLASH_ACCESS_H

#include "main.h"

#define PAGE_SIZE                     ((uint32_t)(1024))
#define NAND_FLASH_BASE_ADDRESS       ((uint32_t)(0x08000000 + 0x03800))
#define NAND_FLASH_END_ADDRESS        ((uint32_t)(0x08000000 + 0x0B800))

#define BLOCK_SIZE                    PAGE_SIZE
#define BLOCK_NUM                     ((NAND_FLASH_END_ADDRESS - NAND_FLASH_BASE_ADDRESS) / BLOCK_SIZE)

/* function declarations */
uint8_t flashx_init(void);
/* initialize the nand flash */
uint32_t  flash_init (void);
/* read data from multiple blocks of nand flash */
uint32_t  flash_read_multi_blocks (uint8_t* pBuf, uint32_t read_addr, uint16_t block_size, uint32_t block_num);
/* write data to multiple blocks of flash */
uint32_t  flash_write_multi_blocks (uint8_t* pBuf, uint32_t write_addr, uint16_t block_size, uint32_t block_num);

#endif /* FLASH_ACCESS_H */
