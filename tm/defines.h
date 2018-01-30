#ifndef TM_DEFINES_H
#define TM_DEFINES_H

//Put your global defines for all libraries here

#define FATFS_USE_USB 1
#define USE_USB_OTG_HS
#define STM32F429_DISCOVERY

// SDRAM base address for Bank 2
#define SDRAM_BASE 0xD0000000

// SDRAM size is 8MB
#define SDRAM_SIZE 0x00800000

// 32KB of p-values at the end of memory - up to 32GB of data @ 1MB chunks
#define SDRAM_PVAL_ADDR 0x007F8000
#endif
