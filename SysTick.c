#include "SysTick.h"
#include "tm4c123gh6pm.h"

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = 20000000 - 1;
  NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x1FFFFFFF) | 0x40000000; //Priority 2
	NVIC_ST_CTRL_R = 0x07;
}
