#include "inz.h"

void Inz_InitAll(void)
{
	SystemInit(); //Initialize system
	TM_DELAY_Init(); //Initialize delay

	TM_ILI9341_Init(); //Initialize ILI9341 with LTDC. By default layer 1 is used
	TM_ILI9341_Rotate(TM_ILI9341_Orientation_Portrait_2);

	TM_STMPE811_Init(); //Initialize touch controller
	TM_DISCO_LedInit(); //leds init
	TM_USB_MSCHOST_Init(); //usb mass storage init

	TM_ILI9341_SetLayer1();
	TM_ILI9341_Fill(ILI9341_COLOR_LGRAY); /* Fill light gray color on layer 1 */
}

void Inz_ShowInfoScreen(FATFS *fs)
{
	uint32_t free, total;
	uint8_t buffer[10];

	TM_ILI9341_Fill(ILI9341_COLOR_BLUE); /* Fill data on layer 1 */
	TM_ILI9341_Puts(8, 8, "------- Informacje! -------", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
	TM_ILI9341_Puts(8, 28, "Bytes Total:", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
	TM_ILI9341_Puts(8, 48, "Bytes Free:", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
	
	TM_DISCO_LedOn(LED_RED);

	if (f_mount(fs, "1:", 1) == FR_OK)
	{
		TM_FATFS_USBDriveSize(&total, &free);

		//TM_ILI9341_DrawFilledCircle(250, 150, 140, ILI9341_COLOR_MAGENTA);
		//TM_ILI9341_DrawFilledCircle(64, 64, 64, ILI9341_COLOR_GREEN2);
	
		itoa(total, buffer, 10);
		TM_ILI9341_Puts(140, 28, buffer, &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
		itoa(free, buffer, 10);
		TM_ILI9341_Puts(140, 48, buffer, &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
	
		//TM_ILI9341_SetLayer2();
		//TM_ILI9341_Fill(ILI9341_COLOR_GREEN2);/* Fill data on layer 2 */

		//f_mount(0, "1:", 1); //unmount
	}
	else
	{
		TM_ILI9341_Puts(0, 0, "Inz_ShowInfoScreen err!", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
	}

	f_mount(0, "1:", 1);

	TM_DISCO_LedOff(LED_RED);
}

Inz_ReadFileResult Inz_ReadData(FATFS *fs, FIL *file, uint32_t *bytes_read)
{
	Inz_ReadFileResult wynik=RES_OK;
	uint8_t ret;
	
    TM_DISCO_LedOn(LED_RED);

	if (f_mount(fs, "1:", 1) == FR_OK)
	{
		if (f_open(file, "1:rand.bin", FA_READ | FA_OPEN_ALWAYS) == FR_OK) {

			ret=f_read(file, (uint32_t *)SDRAM_BASE, 1048576, bytes_read);

			if(ret != FR_OK) //1048576
			{
				wynik = ret; //RF_FREAD_ERR
			}
			
			/*itoa(result, buffer, 10);
			TM_ILI9341_Puts(150, 92, buffer, &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_TRANSPARENT);

			TM_DISCO_LedOn(LED_GREEN);
			TM_DISCO_LedOff(LED_RED);

			itoa(sn, buffer, 10);
			TM_ILI9341_Puts(150, 148, buffer, &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_TRANSPARENT);
			sobs=abs(sn)/sqrtf(result);
			dtoa(buffer, sobs);
			TM_ILI9341_Puts(150, 162, buffer, &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_TRANSPARENT);
			erro=erfcf(sobs/sqrtf(2));
			dtoa(buffer, erro);
			TM_ILI9341_Puts(150, 176, buffer, &TM_Font_7x10, ILI9341_COLOR_BLACK, ILI9341_TRANSPARENT);*/
			
			f_close(file);
		}
		else
		{
			wynik = RF_FOPEN_ERR;
		}
		
		//f_mount(0, "1:", 1); //unmount
	}
	else
	{
		wynik = RF_MOUNT_ERR;
	}
	
	f_mount(0, "1:", 1);

	TM_DISCO_LedOff(LED_RED);
	
	return wynik;
}
