.syntax unified
.cpu cortex-m4
.thumb
.text
	.global GPIO_init
	.global max7219_init
	.global max7219_send
	.global max7219_init2
	.global max7219_send2
	//.global main
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIO_BSRR_OFFSET, 0x18
	.equ GPIO_BRR_OFFSET, 0x28

	.equ GPIOA_BASE, 0x48000000
	.equ GPIOA_MODER, 0x48000000
	.equ GPIOA_OTYPER, 0x48000004
	.equ GPIOA_PUDER, 0x4800000C
	.equ GPIOA_ODR, 0x48000014

	.equ GPIOB_BASE, 0x48000400
	.equ GPIOB_MODER, 0x48000400
	.equ GPIOB_OTYPER, 0x48000404
	.equ GPIOB_PUDER, 0x4800040C
	.equ GPIOB_ODR, 0x48000414

	.equ DECODE_MODE, 0x09
	.equ SHUTDOWN, 0x0C
	.equ INTENSITY, 0x0A
	.equ SCAN_LIMIT, 0x0B
	.equ DISPLAY_TEST, 0x0F

	.equ DATA, 0x20 //PA5
	.equ LOAD, 0x40 //PA6
	.equ CLOCK, 0x80 //PA7



GPIO_init:
	//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK
	movs  r0, #0x7
	ldr  r1, =RCC_AHB2ENR
	str  r0,[r1]

	ldr r0, =0x5400
	ldr  r1, = GPIOA_MODER
	ldr  r2, [r1]
	ldr r3, =0xFFFF03FF
	and  r2, r3
	orrs r2, r2, r0
	str  r2, [r1]

	ldr r0, =0x5400
	ldr  r1, = GPIOB_MODER
	ldr  r2, [r1]
	ldr r3, =0xFFFF03FF
	and  r2, r3
	orrs r2, r2, r0
	str  r2, [r1]

	//ldr  r1, =GPIOA_ODR

	BX LR

max7219_init:
	//TODO: Initialize max7219 registers
	push {r0, r1, r2, lr}

	ldr r0, =#DECODE_MODE
	ldr r1, =#0xFF
	BL max7219_send
	BL max7219_send2

	ldr r0, =#DISPLAY_TEST
	ldr r1, =#0x0
	BL max7219_send
	BL max7219_send2

	ldr r0, =#SCAN_LIMIT
	ldr r1, =0x7
	BL max7219_send
	BL max7219_send2

	ldr r0, =#INTENSITY
	ldr r1, =#0xA
	BL max7219_send
	BL max7219_send2

	ldr r0, =#SHUTDOWN
	ldr r1, =#0x1
	BL max7219_send
	BL max7219_send2

	pop {r0, r1, r2, pc}
	BX LR

max7219_init2:
	//TODO: Initialize max7219 registers
	push {r0, r1, r2, lr}

	ldr r0, =#DECODE_MODE
	ldr r1, =#0xFF
	BL max7219_send2

	ldr r0, =#DISPLAY_TEST
	ldr r1, =#0x0
	BL max7219_send2

	ldr r0, =#SCAN_LIMIT
	ldr r1, =0x7
	BL max7219_send2

	ldr r0, =#INTENSITY
	ldr r1, =#0xA
	BL max7219_send2

	ldr r0, =#SHUTDOWN
	ldr r1, =#0x1
	BL max7219_send2

	pop {r0, r1, r2, pc}
	BX LR


max7219_send:
	//input parameter: r0 is ADDRESS , r1 is DATA
	//TODO: Use this function to send a message to max7219
	push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
	lsl r0, r0, #8
	add r0, r0, r1
	ldr r1, =#GPIOA_BASE
	ldr r2, =#LOAD
	ldr r3, =#DATA
	ldr r4, =#CLOCK
	ldr r5, =#GPIO_BSRR_OFFSET
	ldr r6, =#GPIO_BRR_OFFSET
	mov r7, #16//r7 = i
.max7219send_loop:
	mov r8, #1
	sub r9, r7, #1
	lsl r8, r8, r9 // r8 = mask
	str r4, [r1, r6]//HAL_GPIO_WritePin(GPIOA, CLOCK, 0);
	tst r0, r8
	beq .bit_not_set//bit not set
	str r3, [r1, r5]
	b .if_done
.bit_not_set:
	str r3, [r1, r6]
.if_done:
	str r4, [r1, r5]
	subs r7, r7, #1
	bgt .max7219send_loop
	str r2, [r1, r6]
	str r2, [r1, r5]

	pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
	BX LR

max7219_send2:
	//input parameter: r0 is ADDRESS , r1 is DATA
	//TODO: Use this function to send a message to max7219
	push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
	lsl r0, r0, #8
	add r0, r0, r1
	ldr r1, =#GPIOB_BASE
	ldr r2, =#LOAD
	ldr r3, =#DATA
	ldr r4, =#CLOCK
	ldr r5, =#GPIO_BSRR_OFFSET
	ldr r6, =#GPIO_BRR_OFFSET
	mov r7, #16//r7 = i
.max7219send_loop2:
	mov r8, #1
	sub r9, r7, #1
	lsl r8, r8, r9 // r8 = mask
	str r4, [r1, r6]//HAL_GPIO_WritePin(GPIOA, CLOCK, 0);
	tst r0, r8
	beq .bit_not_set2//bit not set
	str r3, [r1, r5]
	b .if_done
.bit_not_set2:
	str r3, [r1, r6]
.if_done2:
	str r4, [r1, r5]
	subs r7, r7, #1
	bgt .max7219send_loop2
	str r2, [r1, r6]
	str r2, [r1, r5]

	pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9}
	BX LR
