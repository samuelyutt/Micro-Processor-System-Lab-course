#include "stm32l476xx.h"
#include "cmsis_gcc.h"
#include "core_cm4.h"
#include "core_cmFunc.h"
#include "core_cmInstr.h"
#include "core_cmSimd.h"
#include "system_stm32l4xx.h"

#ifndef GPIO_PIN_0
#define GPIO_PIN_0   ((uint16_t) 0x0001)
#define GPIO_PIN_1   ((uint16_t) 0x0002)
#define GPIO_PIN_2   ((uint16_t) 0x0004)
#define GPIO_PIN_3   ((uint16_t) 0x0008)
#define GPIO_PIN_4   ((uint16_t) 0x0010)
#define GPIO_PIN_5   ((uint16_t) 0x0020)
#define GPIO_PIN_6   ((uint16_t) 0x0040)
#define GPIO_PIN_7   ((uint16_t) 0x0080)
#define GPIO_PIN_8   ((uint16_t) 0x0100)
#define GPIO_PIN_9   ((uint16_t) 0x0200)
#define GPIO_PIN_10  ((uint16_t) 0x0400)
#define GPIO_PIN_11  ((uint16_t) 0x0800)
#define GPIO_PIN_12  ((uint16_t) 0x1000)
#define GPIO_PIN_13  ((uint16_t) 0x2000)
#define GPIO_PIN_14  ((uint16_t) 0x4000)
#define GPIO_PIN_15  ((uint16_t) 0x8000)
#define GPIO_PIN_ALL ((uint16_t) 0xFFFF)
#endif

#ifndef SET_BIT
#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT) ((REG) & (BIT))
#define CLEAR_REG(REG) ((REG) = (0x0))
#define WRITE_REG(REG, VAL) ((REG) = (VAL))
#define READ_REG(REG) ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))
#endif

void GPIO_SetPinLow(GPIO_TypeDef *GPIOX, uint16_t GPIO_PIN_Y) {
	GPIOX->BRR = GPIO_PIN_Y;
}

void GPIO_SetPinHigh(GPIO_TypeDef *GPIOX, uint16_t GPIO_PIN_Y) {
	GPIOX->BSRR = GPIO_PIN_Y;
}

int GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOX, uint16_t GPIO_PIN_Y) {
	return GPIOX->IDR & GPIO_PIN_Y;
}


void GPIO_Init(void) {
	// AHB2
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;
	// USART1_TX
	//TM_GPIO_Init(GPIOB, 6, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	// USART1_RX
	//TM_GPIO_Init(GPIOB, 7, TM_GPIO_Mode_AF, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Low);
	GPIOA->MODER   &= 0b11111111110000111111111111111111;
	GPIOA->MODER   |= 0b00000000001010000000000000000000;
	GPIOA->PUPDR   &= 0b11111111110000111111111111111111;
	GPIOA->OSPEEDR &= 0b11111111110000111111111111111111;
	GPIOA->OTYPER  &= 0b11111111111111111111100111111111;
	// AF7
	GPIOA->AFR[1] = (GPIOA->AFR[1] & 0xFFFFF00F) | 0x00000770;
	// B1_USER
	//TM_GPIO_Init(GPIOC, 13, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);
	GPIOC->MODER   &= 0b11110011111111111111111111111111;
	GPIOC->PUPDR   &= 0b11110011111111111111111111111111;
	GPIOC->OSPEEDR &= 0b11110011111111111111111111111111;
	GPIOC->OSPEEDR |= 0b00000100000000000000000000000000;
	GPIOC->OTYPER  &= 0b11111111111111111101111111111111;
}

void USART1_Init(void) {
	// APB2
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// Word length, Parity selection, Parity control enable, Transmitter enable, Receiver enable, Oversampling mode
	MODIFY_REG(USART1->CR1, USART_CR1_M | USART_CR1_PS | USART_CR1_PCE | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8, USART_CR1_TE | USART_CR1_RE);
	// STOP bits, LIN mode enable, Clock enable
	MODIFY_REG(USART1->CR2, USART_CR2_STOP | USART_CR2_LINEN | USART_CR2_CLKEN, 0x0);
	// RTS enable, CTS enable, One sample bit method enable, Smartcard mode enable, Half-duplex selection, IrDA mode enable
	MODIFY_REG(USART1->CR3, USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT | USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN, 0x0);
	// USARTDIV
	MODIFY_REG(USART1->BRR, 0xFFFF, 4000000L/9600L);
	// USART enable
	USART1->CR1 |= USART_CR1_UE;
}

int UART_Transmit(uint8_t *arr, uint32_t size) {
	int sent = 0;
	for (int i = 0; i < size; ++i) {
		while (!READ_BIT(USART1->ISR, USART_ISR_TC));
		USART1->TDR = *arr;
		sent++;
		arr++;
	}
	while (!READ_BIT(USART1->ISR, USART_ISR_TC));
	return sent;
}

int main(void) {
	int prev_btn = 1, curr_btn = 1;
	GPIO_Init();
	USART1_Init();

	while (1) {
		if (!prev_btn && curr_btn)
			UART_Transmit((uint8_t *) "Hello World!", 12);
		prev_btn = curr_btn;
		curr_btn = GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_13);
	}
	return 0;
}
