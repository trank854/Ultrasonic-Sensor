//Requirements
//PLL - 40Mhz
//PB5 - Trigger Pin - Output
//PB4 - Echo Pin - Input
//GPTimer1 - Count-Down - Makes Trigger Pulses (10us) - Measure ECHO High Period
//SysTick - Enable Interrupt - Changes PWM
//	Period Handles Blinking, Duty Cycle Handles Power Brightness
//Edge Interrupt (Probably mentioning GPIOPortB_Handler) to catch High and Falling Edge
//Interrupt Priority: Timer - 3 -> PortB - 2 -> SysTick - 1

#include "PLL.h"
#include "SysTick.h"
#include "SysTick.h"
#include "Timer1A.h"
#include "tm4c123gh6pm.h"

//Function Prototypes
void PortF_Init(void);
void PortB_Init(void);

//Variables
volatile unsigned long delay;
unsigned long distance;
unsigned int flag;

#define PERIOD 4000
#define BLINKING 10000000

int main(void){
	PortB_Init();
	PortF_Init();
	PLL_Init();
	SysTick_Init();
	Timer1A_Init();
	GPIO_PORTB_DATA_R &= ~0x20;
	distance = 400;
	while(1){
		flag = 0;
		GPIO_PORTB_DATA_R &= ~0x20; 
		GPIO_PORTB_DATA_R |= 0x20;
		Timer1A_Wait();
		GPIO_PORTB_DATA_R &= ~0x20;
		while(!flag);
	}
}

void PortB_Init(void){
	//Port Initialization
	SYSCTL_RCGC2_R |= 0x02;																		//Enable PortB Clock
	delay = SYSCTL_RCGC2_R;																		//Delay
	GPIO_PORTB_DIR_R |= 0x20;																	//PB4 -> Input : PB5 -> Output
	GPIO_PORTB_DEN_R |= 0x30;																	//PB4 - PB5 Enable
	
	//Interrupt Initialization
	GPIO_PORTB_IS_R &= ~0x10;																	//Edge-Triggered
	GPIO_PORTB_IBE_R |= 0x10;																	//Both Edges
	GPIO_PORTB_ICR_R = 0x10;																	//Reset flag 4
	GPIO_PORTB_IM_R |= 0x10;																	//Arm PB4
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF) | 0x00002000;			//Priority 1 - Bits 13-15
	NVIC_EN0_R |= 0x02;																				//Enable IRQ 1 (GPIOPortB_Handler)
}

void PortF_Init(void){
	//Port Initialization
	SYSCTL_RCGC2_R |= 0x20;																		//Enable PortF Clock
	delay = SYSCTL_RCGC2_R;																		//Delay
	GPIO_PORTF_DIR_R |= 0x0E;																	//PF1 - 3 -> Output
	GPIO_PORTF_DEN_R |= 0x0E;																	//PF1 - 3 Enable
}

void SysTick_Handler(){
	NVIC_ST_CTRL_R &= ~0x01;
	if (distance > 70){
		GPIO_PORTF_DATA_R = 0x00;
	}
	else if (distance <= 70 && distance >= 10){
		GPIO_PORTF_DATA_R = 0x0E;
	}
	else if (distance < 10){
		GPIO_PORTF_DATA_R = 0x02;
	}
	NVIC_ST_RELOAD_R = 4000 - 1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R |= 0x01;
}

void GPIOPortB_Handler(){
	if((GPIO_PORTB_DATA_R&0x10) == 0x10){
		Timer1A_Start();
	}
	else{
		Timer1A_Stop();
		distance = ((4294967295 - TIMER1_TAR_R)*0.025*0.0343)/2;
		flag = 1;
	}
	GPIO_PORTB_ICR_R = 0x10;
}
