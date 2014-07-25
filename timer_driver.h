//	|timer_driver.h|: Header for the Timer Driver
//	@author: Joe Gibson
//	@author: Adam Luckenbaugh

#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"

#include "driverlib/timer.h"
#include "inc/lm3s6965.h"

#include "scheduler.h"

//Initialize SysTick
void SysTickInit();

#endif