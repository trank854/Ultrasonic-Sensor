#include "Timer1A.h"
#include "tm4c123gh6pm.h"

void Timer1A_Init(){
	SYSCTL_RCGCTIMER_R |= 0x02;																//Activate 16/32-bit Timer1
	TIMER1_CTL_R = 0x00;																			//Disable Timer1
	TIMER1_CFG_R = 0x00;																			//32-bit Configuration
	TIMER1_TAMR_R |= 0x02;																		//Periodic Timer Mode/Count-Down
	//TIMER1_TAILR_R = 400 - 1;																	//Set Start Number for Count-Down
	TIMER1_TAPR_R = 0;																				//Check Every Cycle
}

void Timer1A_Wait(){
	TIMER1_CTL_R = 0x00;
  TIMER1_TAILR_R = 400 - 1; // number of counts to wait
	TIMER1_CTL_R |= 0x01;
  while(TIMER1_TAR_R != 0); // wait for count flag
  TIMER1_CTL_R = 0x00;
}

void Timer1A_Start(){
	TIMER1_CTL_R = 0x00;
	TIMER1_TAILR_R = 4294967296 - 1;
	TIMER1_CTL_R = 0x01;
}

void Timer1A_Stop(){
	TIMER1_CTL_R = 0x00;
}