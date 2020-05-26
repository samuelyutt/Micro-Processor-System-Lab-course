#include "stm32l476xx.h"

#define TIME_SEC 95.87
#define TIM_COUNTERMODE_UP 0
#define TIM_ARR_VAL 99U
#define TIM_PSC_VAL 39999U
//TODO: define your gpio pin

extern void LEDON();
extern void LEDOFF();
int count;
int scan_col;
int key_value;
int time;
int sss = 0;
const unsigned int Table[4][4] = {	{1, 2, 3, 10},
					{4, 5, 6, 11},
					{7, 8, 9, 12},
					{15, 0, 14, 13}};


//These functions inside the asm file
extern void GPIO_init();


void buzzer() {
	sss = 1;
	//TIM2->CR1 |= TIM_CR1_CEN;
	while(GPIOC->IDR&1<<13) {
		LEDON();
		LEDOFF();
	}
	sss = 0;
	//int a = 200000;
	//while(a--);
	//TIM2->CR1 &= ~TIM_CR1_CEN;
}

/* TODO: initial keypad gpio pin, X as output and Y as input */
void Display(int count) {
	//sss = 1;
	int k = count;
	while(count > 0) {
		count--;
		if (k != key_value) {
			count -= key_value;
			//k = key_value;
		}
		int a = 600000;
		while(a--);
		if(count <0)break;
	}
	//sss = 0;
	if (count != -99999)
		buzzer();
	key_value = -99999;
}


void SysTick_Handler(void) // IF INTERRUPT HAPPENS, DO THIS TASK!
{

	if(sss) return;
	//TODO: Show temperature on 7-seg display
	GPIOA->BRR = (0b10111 << 8);
	int st = (((scan_col+1)%4)+8);
	if (st == 11) st++;
	GPIOA->BSRR = (1 << st);
	scan_col = (scan_col+1)%4;

}

void SystemClock_Config(){


	RCC->APB2ENR |= 1;
	SysTick->LOAD  = 4000; //0.1s
	SysTick->VAL   = 0; //Load the SysTick Counter Value
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	return;
}


void keypad_init() {
	// SET keypad gpio OUTPUT //
		RCC->AHB2ENR = RCC->AHB2ENR|0x7;
		//Set PA8,9,10,12 as output mode
		GPIOA->MODER= GPIOA->MODER&0xFDD5FFFF;
		//set PA8,9,10,12 is Pull-up output
		GPIOA->PUPDR=GPIOA->PUPDR|0x1150000;
		//Set PA8,9,10,12 as medium speed mode
		GPIOA->OSPEEDR=GPIOA->OSPEEDR|0x1150000;
		//Set PA8,9,10,12 as high
		GPIOA->ODR=GPIOA->ODR|10111<<8;

	// SET keypad gpio INPUT //
	    //Set PC0,1,2,3 as INPUT mode
		GPIOC->MODER=GPIOC->MODER&0xFFFFFF00;
		//set PC0,1,2,3 is Pull-down input
		GPIOC->PUPDR=GPIOC->PUPDR|0xAA;
		//Set PC0,1,2,3 as medium speed mode
		GPIOC->OSPEEDR=GPIOC->OSPEEDR|0x55;
}


void EXTI_Setup(){
	//select source for EXTI0.1.2.3
	SYSCFG->EXTICR[0] |= (SYSCFG_EXTICR1_EXTI0_PC | SYSCFG_EXTICR1_EXTI1_PC | SYSCFG_EXTICR1_EXTI2_PC | SYSCFG_EXTICR1_EXTI3_PC);
	// setup EXTI
	//enable interrupt 0.1.2.3
	EXTI->IMR1 |= (EXTI_IMR1_IM3 | EXTI_IMR1_IM2 | EXTI_IMR1_IM1 | EXTI_IMR1_IM0);
	//select falling edge 0.1.2.3
	EXTI->FTSR1 |= (EXTI_FTSR1_FT3 | EXTI_FTSR1_FT2 | EXTI_FTSR1_FT1 | EXTI_FTSR1_FT0);
	//clear
	EXTI->PR1 |= (EXTI_PR1_PIF3 | EXTI_PR1_PIF2 | EXTI_PR1_PIF1 | EXTI_PR1_PIF0);

	//enable interrupt mask
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	//clear pending flag
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	//set priority
	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_SetPriority(EXTI1_IRQn,0);
	NVIC_SetPriority(EXTI2_IRQn,0);
	NVIC_SetPriority(EXTI3_IRQn,0);
	//set systick priority level
	//SCB->SHP[(((uint32_t)(int32_t)-1) & 0xFUL)-4UL] = (uint8_t)((1 << (8U - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL);
	SCB->SHP[11] = 0xFF;
}

void EXTI0_IRQHandler(void){
	if(scan_col == 0)
		key_value = 1;
	else if(scan_col == 1)
		key_value = 2;
	else if(scan_col == 2)
		key_value = 3;
	else if(scan_col == 3)
		key_value = 10;
	//clear pending flag
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	//clear exti
	EXTI->PR1 |= (EXTI_PR1_PIF3 | EXTI_PR1_PIF2 | EXTI_PR1_PIF1 | EXTI_PR1_PIF0);
}

void EXTI1_IRQHandler(void){
	if(scan_col == 0)
		key_value = 4;
	else if(scan_col == 1)
		key_value = 5;
	else if(scan_col == 2)
		key_value = 6;
	else if(scan_col == 3)
		key_value = 11;
	//clear pending flag
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	//clear exti
	EXTI->PR1 |= (EXTI_PR1_PIF3 | EXTI_PR1_PIF2 | EXTI_PR1_PIF1 | EXTI_PR1_PIF0);
}

void EXTI2_IRQHandler(void){
	if(scan_col == 0)
		key_value = 7;
	else if(scan_col == 1)
		key_value = 8;
	else if(scan_col == 2)
		key_value = 9;
	else if(scan_col == 3)
		key_value = 12;
	//clear pending flag
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	//clear exti
	EXTI->PR1 |= (EXTI_PR1_PIF3 | EXTI_PR1_PIF2 | EXTI_PR1_PIF1 | EXTI_PR1_PIF0);
}

void EXTI3_IRQHandler(void){
	if(scan_col == 0)
		key_value = 15;
	else if(scan_col == 1)
		key_value = 0;
	else if(scan_col == 2)
		key_value = 14;
	else if(scan_col == 3)
		key_value = 13;
	//clear pending flag
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	//clear exti
	EXTI->PR1 |= (EXTI_PR1_PIF3 | EXTI_PR1_PIF2 | EXTI_PR1_PIF1 | EXTI_PR1_PIF0);
}



void main() {
	GPIO_init();
	SystemClock_Config();
	EXTI_Setup();
	keypad_init();
	scan_col = 0;
	key_value = -99999;
	LEDON();
	while(1) {
		Display(key_value);
	}
}
