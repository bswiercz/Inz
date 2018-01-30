#include "inz.h"

int main(void)
{
    FATFS USB_Fs;
    FIL USB_Fil;

    uint8_t usb_write = 1;
    uint32_t bytes_read;
    uint32_t touch_lock = 0;
    uint32_t visibleMenu = 0;
    uint32_t needRedraw = 1;
    uint8_t retval;

    SystemInit(); //Initialize system

	TM_ILI9341_Init(); //Initialize ILI9341 with LTDC. By default layer 1 is used
	TM_ILI9341_Fill(BGCOLOR); /* Fill light gray color on layer 1 */
	TM_ILI9341_Rotate(TM_STMPE811_Orientation_Portrait_1);

	retval = TM_SDRAM_Init();
	if (!retval)
	{
		TM_ILI9341_Puts(8, 8, "SDRAM Init failed!", &TM_Font_11x18, ILI9341_COLOR_RED, BGCOLOR);
	}

	TM_DISCO_LedInit(); //leds init
	TM_USB_MSCHOST_Init(); //usb mass storage init

    TM_STMPE811_TouchData hTouchData;
    TM_STMPE811_State_t hTouchState;
    hTouchData.orientation = TM_STMPE811_Orientation_Portrait_1;
	TM_STMPE811_Init(); //Initialize touch controller

    int8_t iButtonPressed, iButtonStart, iButtonBack;
    TM_ILI9341_Button_t hButtonStart;
    hButtonStart.x = 8;
	hButtonStart.y = 264;
	hButtonStart.width = 108;
	hButtonStart.height = 48;
	hButtonStart.background = ILI9341_COLOR_GRAY;
	hButtonStart.borderColor = ILI9341_COLOR_BLACK;
	hButtonStart.label = "Start";
	hButtonStart.font = &TM_Font_11x18;
	hButtonStart.color = ILI9341_COLOR_GREEN;
	hButtonStart.flags = TM_BUTTON_FLAG_ENABLED;
    iButtonStart = TM_ILI9341_Button_Add(&hButtonStart);

    TM_ILI9341_Button_t hButtonBack;
    hButtonBack.x = 124;
    hButtonBack.y = 264;
    hButtonBack.width = 108;
    hButtonBack.height = 48;
    hButtonBack.background = ILI9341_COLOR_GRAY;
    hButtonBack.borderColor = ILI9341_COLOR_BLACK;
    hButtonBack.label = "Back";
    hButtonBack.font = &TM_Font_11x18;
    hButtonBack.color = ILI9341_COLOR_BLACK;
    hButtonBack.flags = TM_BUTTON_FLAG_ENABLED;
    iButtonBack = TM_ILI9341_Button_Add(&hButtonBack);

    //TM_ILI9341_Button_DrawAll();

    while(1)
    {

		//TM_ILI9341_Fill(ILI9341_COLOR_GRAY); /* Fill data on layer 1 */

    	/*
    	 * Draw actual menu
    	 */
    	if (visibleMenu == 0 && needRedraw)
    	{
    			TM_ILI9341_Puts(8, 8, "TRNG tester", &TM_Font_7x10, FGCOLOR, BGCOLOR);
    			TM_ILI9341_Button_Draw(iButtonStart);
    			TM_ILI9341_Button_Draw(iButtonBack);
    	}
    	else if (visibleMenu == 1 && needRedraw)
    	{
    		//
    	}
    	else if (visibleMenu == 2 && needRedraw)
		{
    		//
    	}

    	needRedraw = 0;

    	/*
    	 * Draw debug info
    	 */


    	/*
    	 * Process touch event
    	 */
		iButtonPressed = TM_ILI9341_Button_Touch(&hTouchData);
		hTouchState = TM_STMPE811_ReadTouch(&hTouchData);
    	if (hTouchState == TM_STMPE811_State_Pressed && touch_lock == 1)
    	{
        	TM_ILI9341_Fill(BGCOLOR);
        	TM_ILI9341_Button_DrawAll();

        	int8_t buffer[10];
        	sprintf(buffer, "%03d:%03d", hTouchData.x, hTouchData.y);
        	TM_ILI9341_Puts(191, 0, buffer, &TM_Font_7x10, FGCOLOR, BGCOLOR);

        	TM_ILI9341_DrawLine(hTouchData.x, 0, hTouchData.x, 319, ILI9341_COLOR_RED);
        	TM_ILI9341_DrawLine(0, hTouchData.y, 239, hTouchData.y, ILI9341_COLOR_RED);
    	}

    	/*
    	 * Constraint continous clicking
    	 */
    	if (hTouchData.pressed == TM_STMPE811_State_Released && hTouchData.last_pressed == TM_STMPE811_State_Pressed)
    	{
    		touch_lock = 0;
    		TM_DISCO_LedOff(LED_GREEN);
    	}
    	if (hTouchData.pressed == TM_STMPE811_State_Pressed && hTouchData.last_pressed == TM_STMPE811_State_Released)
    	{
    		touch_lock = 1;
    		TM_DISCO_LedOn(LED_GREEN);
    	}

    	/*
    	 * Detect button touch
    	 */
    	if (iButtonPressed == iButtonStart)
    	{
    		TM_ILI9341_Puts(0, 0, "Button Start Touched", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
    		TM_DISCO_LedToggle(LED_RED);
    	}

    	if (iButtonPressed == iButtonBack)
    	{
    		TM_ILI9341_Puts(0, 0, "Button Back Touched", &TM_Font_7x10, ILI9341_COLOR_WHITE, ILI9341_TRANSPARENT);
    	}

        /* Host Task handler */
        /* This have to be called periodically as fast as possible */
        //TM_USB_MSCHOST_Process();

        /* Device is connected and ready to use */
        if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_Connected) {

    		TM_ILI9341_DrawFilledCircle(250, 150, 140, ILI9341_COLOR_MAGENTA);
    		TM_ILI9341_DrawFilledCircle(64, 64, 64, ILI9341_COLOR_GREEN2);

            /* If we didn't write data already */
            if (usb_write) {
            	usb_write=0;

            	Inz_ShowInfoScreen(&USB_Fs);
            	Inz_ReadData(&USB_Fs, &USB_Fil, &bytes_read);
            	while(!TM_DISCO_ButtonPressed());

            	Frequency(bytes_read);
            	while(!TM_DISCO_ButtonPressed());
            	Runs(bytes_read);
            }
            else if (!usb_write && TM_DISCO_ButtonPressed())
            {
            	usb_write=1;
            }
        }
    }
}
