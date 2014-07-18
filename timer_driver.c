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

void Timer0AInit()
{
   //Enable Timer 0 Peripheral
   SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

   //Configure and Set Timer 0A
   TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
   TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / 1);   //1Hz?

   //Enable the Timer 0A interrupt
   IntEnable(INT_TIMER0A);
   TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

   //Enable Timer 0A
   TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer0IntHandler(void)
{
   //Clear Timer 0 interrupt flag
   TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

   //tell the threads that the timer interrupt fired
   //deal with this later
}