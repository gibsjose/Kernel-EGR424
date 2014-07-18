//	|oled_driver.h|: Header for the OLED Display Driver
//	@author: Joe Gibson
//	@author: Adam Luckenbaugh

#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "drivers/rit128x96x4.h"

//Include standard C libs
#include "utils/ustdlib.h"

#define MAX_BRIGHTNESS 	15	//Maximum brightness
#define MIN_BRIGHTNESS  5   //Minimum brightness
#define OLED_COLS 128
#define OLED_ROWS 96

//Initialize OLED Display Driver
void oled_d_init(void);

//Print at 0,0 with full brightness
void oled_d_print_origin(char *str);

//Print at x,y with full brightness
void oled_d_print_xy(char *str, unsigned long x, unsigned long y);

//Print at x,y with given brightness
void oled_d_print_xyb(char *str, unsigned long x, unsigned long y, unsigned long b);

//Display a vertical bar
void oled_d_display_bar(int aHeight);

//Clear the screen
void oled_d_clear(void);

#endif
