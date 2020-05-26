#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"
#define SET_REG(REG, SELECT, VAL) { ((REG)=((REG)&(~(SELECT))) | (VAL)); };


//These functions inside the asm file
extern void GPIO_init();
extern void Delay1sWith4MHz();
extern void DelayDebounce();
extern void LEDON();
extern void LEDOFF();
extern void Detect();

int state;
int pressed;

unsigned int pll_config[5] =
{
    //PLLR[1:0]|PLLREN|23~16|15|PLLN[7:0]|7|PLLM[2:0]|3 2|PLLSRC[1:0]

    //MSI set as clock entry
    //654321098765432109876543210
    0b111000000000000100001110001,
    0b011000000000000110000110001,
    0b011000000000001010000110001,
    0b011000000000010000000110001,
    0b011000000000101000000110001
};
/*
void SystemClock_Config(){
	//SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, 6<<6);
	//RCC->CR |= ~RCC_CR_PLLON;
	//while((RCC->CR | RCC_CR_PLLRDY) == 1);

	//TODO: Change the SYSCLK source and set the corresponding Prescaler value.
	RCC->CR |= RCC_CR_HSION;// turn on HSI16 oscillator
	while((RCC->CR & RCC_CR_HSIRDY) == 0);//check HSI16 ready
	SET_REG(RCC->CFGR, RCC_CFGR_HPRE, 8<<4);//SYSCLK divide by 2. SYSCLK = 16MHz/2 = 8Mhz
	if((RCC->CR & RCC_CR_HSIRDY) == 0) {
		//return;


		//RCC->CR |= RCC_CR_PLLON;
		//while((RCC->CR & RCC_CR_PLLRDY) == 0);//check HSI16 ready

		//SET_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLLN, 6<<6);
		//RCC->PLLCFGR |= RCC_PLLCFGR_PLLPEN;
		//RCC->PLLCFGR |= RCC_PLLCFGR_PLLQEN;
		//RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;
		return;

	}
	// Use HSI16 as system clock //APB1 prescaler not divide //APB2 prescaler not divide
}
*/

void SystemClock_Config(){
	//TODO: Change the SYSCLK source and set the corresponding Prescaler value.
	state %= 5; //state cycle
	    //temporarily use ths hsi clock before turning off the pll clock for configuration since the system still need the clock to work
	    RCC->CR |= RCC_CR_HSION; //turn on the hsi clock before configuraion
	    while((RCC->CR & RCC_CR_HSIRDY) == 0); //wait till the hsi clock has been really turned on

	    RCC->CFGR = 0x00000000; //CFGR reset value
	    RCC->CR  &= 0xFEFFFFFF; //PLL off
	    while (RCC->CR & 0x02000000); //busy waiting till PLL is really halted

	    //after halted, configure the PLLCFGR to set the clock speed
	    RCC->PLLCFGR &= 0x00000001; //off all except the MSI clock source
	    RCC->PLLCFGR |= pll_config[state]; //customization PLLN PLLM PLLR settings

	    RCC->CR |= RCC_CR_PLLON; //turn on the pll clock again
		while((RCC->CR & RCC_CR_PLLRDY) == 0); //busy waiting till PLL is really turned on

		RCC->CFGR |= RCC_CFGR_SW_PLL; //set the clock source as pll clock (customized)
	    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL); //wait till the pll clock is really set
	return;
}

void Detect() {
	int flag = ~GPIOC->IDR&1<<13;
	if (flag) {
		pressed = 1;
		//DelayDebounce();
	}
	return;
}


void main() {
	SystemClock_Config();
	GPIO_init();
	pressed = 0;
	state = 0;
	while (1) {
		LEDON();
		Delay1sWith4MHz();
		LEDOFF();
		Delay1sWith4MHz();
		if (pressed) {
			state++;
			SystemClock_Config();
			pressed = 0;
			DelayDebounce();
			if (state == 4) {
				for (int i=0; i<10; i++)
					DelayDebounce();
			}
		}
	}
}
