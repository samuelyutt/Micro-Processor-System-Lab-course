.syntax unified
.cpu cortex-m4
.thumb
.text
	.global GPIO_init
	.global LEDON
	.global LEDOFF
	//.global Delay1sWith4MHz
	.global DelayDebounce
.text
	.global main
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIO_BSRR_OFFSET, 0x18
	.equ GPIO_BRR_OFFSET, 0x28

	.equ GPIOB_BASE, 0x48000400
	.equ GPIOB_MODER, 0x48000400
	.equ GPIOB_OTYPER, 0x48000404
	.equ GPIOB_PUDER, 0x4800040C
	.equ GPIOB_ODR, 0x48000414



LEDON:
	//TODO: Display LED by leds
	push {r0, r1}
	ldr  r1, =GPIOB_ODR
	ldr r0, =0xFF
	str r0, [r1]
	pop {r0, r1}
	bx lr

LEDOFF:
	//TODO: Display LED by leds
	push {r0, r1}
	ldr  r1, =GPIOB_ODR
	ldr r0, =0x0
	str r0, [r1]
	pop {r0, r1}
	bx lr



GPIO_init:
	//TODO: Initialize three GPIO pins as output for PA5
	movs  r0, #0x7
	ldr  r1, =RCC_AHB2ENR
	str  r0,[r1]

	ldr r0, =0x5400
	ldr  r1, = GPIOB_MODER
	ldr  r2, [r1]
	ldr r3, =0xFFFF03FF
	and  r2, r3
	orrs r2, r2, r0
	str  r2, [r1]



	BX LR
