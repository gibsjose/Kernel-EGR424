//	|timer_driver.c|: Implementation of the Timer Driver
//	@author: Joe Gibson
//	@author: Adam Luckenbaugh

#include "timer_driver.h"

//Initialize Timer Driver
void SysTickInit()
{
   NVIC_ST_CTRL_R = 0;
   NVIC_ST_RELOAD_R = 8000; //0.001s (1ms) period for timer
   NVIC_ST_CURRENT_R = 0;    //current counter value = 0
   NVIC_ST_CTRL_R = 0b111;   //clk source = cpu; enable interrupt; enable SysTick timer
}

void SysTickHandler()
{
   //call the scheduler function in the scheduler file
   scheduler();
}