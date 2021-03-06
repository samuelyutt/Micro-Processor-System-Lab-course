#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"

#define SET_REG(REG, SELECT, VAL) { ((REG)=((REG)&(~(SELECT))) | (VAL)); };
#define GPIO_PIN_0  ((uint16_t) 0x0001)
#define GPIO_PIN_1  ((uint16_t) 0x0002)
#define GPIO_PIN_2  ((uint16_t) 0x0004)
#define GPIO_PIN_3  ((uint16_t) 0x0008)

int duty_cycle;
int nowf, pref;
int pressed;

const unsigned int Table[4][4] = { {75, 80, 90, 500},
         {101, 114, 120, 600},
         {135, 152, -1, -1},
         {-1, -1, -1, -1}};


void init_gpio(){
 RCC->AHB2ENR = RCC->AHB2ENR|0x7;
   GPIOC->MODER=GPIOC->MODER&0xFFFFFF00;
  //set PC0,1,2,3 is Pull-down input
 GPIOC->PUPDR=GPIOC->PUPDR|0xAA;
 //Set PC0,1,2,3 as medium speed mode
 GPIOC->OSPEEDR=GPIOC->OSPEEDR|0xFF;

}
void timer_init()
{

 /* GPIO: set PB4 as alternate function */
 RCC->AHB2ENR |= 0x1 << 1;
 GPIOB->MODER |= GPIO_MODER_MODE3_1;
 RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
 GPIOB->AFR[0] |= GPIO_AFRL_AFSEL3_0;
 TIM2->CR1 |= TIM_CR1_DIR;
 TIM2->CR1 |= TIM_CR1_ARPE;
 TIM2->ARR = (uint32_t) 100;
 TIM2->CCMR1 &= 0xFFFFFCFF;
 TIM2->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
 TIM2->CCMR1 |= TIM_CCMR1_OC2PE;
 TIM2->CCER |= TIM_CCER_CC2E;
 TIM2->EGR = TIM_EGR_UG;
}

int ReadGPIO(GPIO_TypeDef *GPIOX, uint16_t GPIO_PIN_Y) {
 return GPIOX->IDR & GPIO_PIN_Y;
}

void timer_config()
{
 TIM2->PSC = (uint32_t) (4000000 / nowf / 100);
 TIM2->CCR2 = duty_cycle;
}

void buzzer(int tempf, int duration, int pause) {
 if (tempf == -1) {
  while(duration--);
  return;
 }
 nowf = tempf;
 timer_config();
 TIM2->CR1 |= TIM_CR1_CEN;
 while(duration--);
 TIM2->CR1 &= ~TIM_CR1_CEN;
 while(pause--);

}

void start_game() {
 buzzer(600,60000, 90000);
 buzzer(600,240000, 0);
}

void goal() {
 buzzer(300,30000, 40000);
 buzzer(400,30000, 40000);
 buzzer(500,60000, 40000);
 buzzer(400,30000, 40000);
 buzzer(500,30000, 40000);
 buzzer(600,160000, 0);
}

void end_game() {
 buzzer(500,60000, 90000);
 buzzer(400,60000, 90000);
 buzzer(300,160000, 0);
}

void main() {
 timer_init();
 nowf = -1;
 pref = -1;
 pressed =0;
 duty_cycle = 90;
 init_gpio();
 int start = 1;

 while(1) {
  if(start && ReadGPIO(GPIOC, GPIO_PIN_0)) {
   start_game();
   start = 0;
  }
  if (ReadGPIO(GPIOC, GPIO_PIN_1)) {
   goal();
  }
  if (!start && ReadGPIO(GPIOC, GPIO_PIN_0)) {
   end_game();
   start = 1;
  }
 }
}
