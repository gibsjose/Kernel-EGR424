#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rit128x96x4.h"
#include "scheduler.h"
#include "oled_driver.h"
#include "timer_driver.h"
#include <string.h>

int main(void)
{
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  // Initialize the OLED display
  oled_d_init();

  // Enable the peripherals used by this example.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

  // Set GPIO A0 and A1 as UART pins.
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Configure the UART for 115,200, 8-N-1 operation.
  UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,
                      (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                       UART_CONFIG_PAR_NONE));

  //Enable the GPIO Port connected to the LED (Port F)
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

  //Enable the GPIO Pin connected to the LED (PF0) and set it to be an output pin
  GPIO_PORTF_DIR_R = 0x01;
  GPIO_PORTF_DEN_R = 0x01;

  //Enable global interrupts
  IntMasterEnable();

  //Initialize Scheduler/Threads
  initThreads();

  //Initialize SysTick Interrupt
  SysTickInit();

  //Yeild() to give control to scheduler
  yield();

  //this should never execute
  exit(0);
}
