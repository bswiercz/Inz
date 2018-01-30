/**
 * @author  Bartek Swierczewski
 * @email   swierkdeck@gmail.com
 * @website http://home.elka.pw.edu.pl/~bswiercz
 * @version v1.0
 * @ide     CooCox CoIDE
 * @license GNU GPL v3
 * @brief   Inzynierka utilities
 */
#ifndef INZH
#define INZH

#include "stm32f4xx.h"
#include "defines.h"

#include "itoa.h"

#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"
#include "tm_stm32f4_ili9341_ltdc.h"

#include "tm_stm32f4_fonts.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_disco.h"

#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_usb_msc_host.h"

#define BGCOLOR ILI9341_COLOR_LGRAY
#define FGCOLOR ILI9341_COLOR_BLACK

typedef enum {
	RF_OK = 0,
	RF_MOUNT_ERR,
	RF_FOPEN_ERR,
	RF_FREAD_ERR
} Inz_ReadFileResult;

/**
 * @brief  Initializes all peripherals
 * @param  None
 * @retval None
 */
void Inz_InitAll(void);

/**
 * @brief  Displays information about USB mass storage device
 * @param  FATFS handle
 * @retval None
 * @retval Inz_ReadFileResult
 */
void Inz_ShowInfoScreen(FATFS *fs);

/**
 * @brief  Mounts first FS from USB MSC device, opens "rand.dat" and copies its contents to  data buffer in SDRAM.
 * @note   STM32F4x9-Eval board uses inverse logic for leds
 * @param  FATFS handle
 * @param  FIL handle
 * @param  pointer which contents will be filled with number of bytes read
 * @retval Inz_ReadFileResult
 */
Inz_ReadFileResult Inz_ReadData(FATFS *fs, FIL *file, uint32_t *bytes_read);

#endif
