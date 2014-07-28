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
#include "timer_driver.h"
#include <string.h>

int main(void)
{
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  // Initialize the OLED display and write status.
  RIT128x96x4Init(1000000);
  RIT128x96x4StringDraw("Project 3 - LG", 20,  0, 15);

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

  RIT128x96x4StringDraw("Project 3 asdf", 20,  20, 15);

  //Enable global interrupts
  IntMasterEnable();

  //Initialize Scheduler/Threads
  initThreads();

  //THREADS SHOULD BE RUNNING!

  SysTickInit();

  yield();

  iprintf("After yield()\r\n");

  while(1);

  exit(0);
}
