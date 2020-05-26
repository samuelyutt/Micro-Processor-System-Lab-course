#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"

//These functions inside the asm file
extern void GPIO_init();
extern void LEDON();
extern void LEDOFF();


int on;

void SysTick_Handler(void) // IF INTERRUPT HAPPENS, DO THIS TASK!
{
    //TODO: Show temperature on 7-seg display
	on ^= 1;
	if (on) LEDON();
	else LEDOFF();
}

void SystemClock_Config(){
	// Turn HSI on
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);

	//unsigned int 32 bit counter 8000000 (3s interrupt once)
	SysTick->LOAD = (uint32_t)11999999;
	//system interrupt happens for every 8000000 cpu cycles, that is the peroid of 3 second
	SysTick->CTRL |= 0x00000007; //processor clock, turn on all

	return;
}


void main() {
	SystemClock_Config();
	GPIO_init();
	on = 0;
	while(1);
}
