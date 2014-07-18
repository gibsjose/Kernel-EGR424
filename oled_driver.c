//	|oled_driver.c|: Implementation of the OLED Display Drivers
//	@author: Joe Gibson
//	@author: Adam Luckenbaugh

#include "oled_driver.h"

//Initialize OLED Display Driver
void oled_d_init(void)
{
	//Initialize OLED Display
	RIT128x96x4Init(1000000);
}

//Print at 0,0 with full brightness
void oled_d_print_origin(char *str)
{
	RIT128x96x4StringDraw(str, 0, 0, MAX_BRIGHTNESS);
}

//Print at x,y with full brightness
void oled_d_print_xy(char *str, unsigned long x, unsigned long y)
{
	RIT128x96x4StringDraw(str, x, y, MAX_BRIGHTNESS);
}

//Print at x,y with given brightness
void oled_d_print_xyb(char *str, unsigned long x, unsigned long y, unsigned long b)
{
	RIT128x96x4StringDraw(str, x, y, b);
}


void oled_d_display_bar(int aHeight)
{
	char * cDarkStr = "#####################";
	char * cClearStr = "                     ";

	unsigned charHeight = 8;

	unsigned rowInc = OLED_ROWS / charHeight;
	
	//Calculate brightness as a percentage of MAX_BRIGHTNESS, 
	// but keep between MAX_BRIGHTNESS and MIN_BRIGHTNESS
	unsigned brightness = ((unsigned)((aHeight / 1023.0) * (double)MAX_BRIGHTNESS) % (MAX_BRIGHTNESS - MIN_BRIGHTNESS)) + MIN_BRIGHTNESS;
	
	//Display a vertical bar with a variable height and variable brightness
	for(unsigned rows = 0; rows < OLED_ROWS / charHeight; rows++)
	{
		if( rowInc - rows < (rowInc * (aHeight / 1023.0) ))
		{
			oled_d_print_xyb(cDarkStr, 0, rows * charHeight, brightness);
		}
		else
		{
			oled_d_print_xyb(cClearStr, 0, rows * charHeight, brightness);
		}
	}
}

//Clear the screen
void oled_d_clear(void)
{
	RIT128x96x4Clear();
}
