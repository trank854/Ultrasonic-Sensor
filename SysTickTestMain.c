// SysTickTestMain.c
// Runs on LM4F120/TM4C123
// Test the SysTick functions by activating the PLL, initializing the
// SysTick timer, and flashing an LED at a constant rate.
// Daniel Valvano
// September 12, 2013
// Modified by Min He and migrate to keil5

/* This example accompanies the books
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   Volume 1, Program 4.7

   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Program 2.11, Section 2.6

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include "SysTick.h"
#include "PLL.h"
#include "tm4c123gh6pm.h"

void PORTF_Init(void);

 int main(void){
	PORTF_Init();							// PF1(Red LED) is an output for debugging
  PLL_Init();               // set system clock to 40 MHz
  SysTick_Init();           // initialize SysTick timer
  while(1){
    GPIO_PORTF_DATA_R = GPIO_PORTF_DATA_R^0x02; // toggle PF1
// 		Test Cases for Different Hertz
//		--------------------------------------------------
//    SysTick_Wait(1);        // approximately 720 ns
//    SysTick_Wait(2);        // approximately 720 ns
//    SysTick_Wait(10000);    // approximately 0.2 ms
//    SysTick_Wait10ms(10);   // approximately 20*10 ms
		SysTick_Wait(10000000); //approximately 0.02 s
  }
}

void PORTF_Init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // activate port F
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)!=SYSCTL_RCGC2_GPIOF){};
	
	GPIO_PORTF_DIR_R |= 0x02; // make PF1 out (built-in blue LED)
  GPIO_PORTF_AFSEL_R &= ~0x02;// disable alt funct on PF1
  GPIO_PORTF_DEN_R |= 0x02; // enable digital I/O on PF1
                            // configure PF1 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFFF0F)+0x00000000;
  GPIO_PORTF_AMSEL_R |= 0x02;   // disable analog functionality on PF  
}
