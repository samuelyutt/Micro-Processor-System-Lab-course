.syntax unified
.cpu cortex-m4
.thumb
.text
	.global main
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIO_BSRR_OFFSET, 0x18
	.equ GPIO_BRR_OFFSET, 0x28

	.equ GPIOA_BASE, 0x48000000
	.equ GPIOA_MODER, 0x48000000
	.equ GPIOA_OTYPER, 0x48000004
	.equ GPIOA_PUDER, 0x4800000C
	.equ GPIOA_ODR, 0x48000014

	.equ GPIOC_MODER, 0x48000800
	.equ GPIOC_IDR, 0x48000810

	.equ DECODE_MODE, 0x9
	.equ SHUTDOWN, 0xC
	.equ INTENSITY, 0xA
	.equ SCAN_LIMIT, 0xB
	.equ DISPLAY_TEST, 0xF

	.equ DATA, 0x20 //PA5
	.equ LOAD, 0x40 //PA6
	.equ CLOCK, 0x80 //PA7


main:
	BL GPIO_init
	BL max7219_init
	mov r7, #0
	mov r10, #0
	mov r12, #0

	mov r8, #0
	mov r9, #1

	mov r0, #1
	mov r1, #0
	BL MAX7219Send
	b Delay
	B Loop

Loop:
	cmp r7, #1
	beq Pressed
	ldr r5, =GPIOC_IDR
	ldr r3, [r5]
	mov r4, #1
	lsl r4, #13
	ands r3, r4
	bne Loop
	mov r7, #1
	b Loop


Pressed:
	add r8, r8, r9
	mov r2, r8

	mov r0, #9	// r0 = digit = address
	mov r1, #0	// r1 = count = data
	mov r4, #0	// r4 = start print
	ldr r7 , =#100000000
	cmp r2, r7
	bge ge1E8
	b ge1E7


ge1E8:
	ldr r0, =#DECODE_MODE
	ldr r1, =#0x1
	BL MAX7219Send
	mov r0, #2
	bl ScanLimit

	mov r0, #1
	mov r1, #1
	BL MAX7219Send
	mov r0, #2
	mov r1, #0x1
	BL MAX7219Send

Delay888:
	push {r0}
	ldr r0, =#150000
.Delay888_loop:
	sub r0, r0, 1
	cmp r0, #0
	bne .Delay_loop
	pop {r0}

	mov r7, #0
	b Loop


ge1E7:
	ldr r3, =#10000000
	cmp r2, r3
	blt Display7
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E7
Display7:
	sub r0, r0, 1
	cmp r1, #0
	beq Zero7

	cmp r4, #1
	beq NoLimit7
	bl ScanLimit
NoLimit7:
	BL MAX7219Send
	mov r4, #1
	mov r1, #0
	b ge1E6
Zero7:
	cmp r4, #0
	beq ge1E6
	BL MAX7219Send
	mov r4, #1
////////////////////////////
ge1E6:
	ldr r3, =#1000000
	cmp r2, r3
	blt Display6
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E6
Display6:
	sub r0, r0, 1
	cmp r1, #0
	beq Zero6

	cmp r4, #1
	beq NoLimit6
	bl ScanLimit
NoLimit6:
	BL MAX7219Send
	mov r4, #1
	mov r1, #0
	b ge1E5
Zero6:
	cmp r4, #0
	beq ge1E5
	BL MAX7219Send
	mov r4, #1
////////////////////////////
ge1E5:
	ldr r3, =#100000
	cmp r2, r3
	blt Display5
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E5
Display5:
	sub r0, r0, 1
	cmp r1, #0
	beq Zero5

	cmp r4, #1
	beq NoLimit5
	bl ScanLimit
NoLimit5:
	BL MAX7219Send
	mov r4, #1
	mov r1, #0
	b ge1E4
Zero5:
	cmp r4, #0
	beq ge1E4
	BL MAX7219Send
	mov r4, #1
////////////////////////////
ge1E4:
	ldr r3, =#10000
	cmp r2, r3
	blt Display4
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E4
Display4:
	sub r0, r0, 1
	cmp r1, #0
	beq Zero4

	cmp r4, #1
	beq NoLimit4
	bl ScanLimit
NoLimit4:
	BL MAX7219Send
	mov r4, #1
	mov r1, #0
	b ge1E3
Zero4:
	cmp r4, #0
	beq ge1E3
	BL MAX7219Send
	mov r4, #1
////////////////////////////
ge1E3:
	ldr r3, =#1000
	cmp r2, r3
	blt Display3
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E3
Display3:
	sub r0, r0, 1
	cmp r1, #0
	beq Zero3

	cmp r4, #1
	beq NoLimit3
	bl ScanLimit
NoLimit3:
	BL MAX7219Send
	mov r4, #1
	mov r1, #0
	b ge1E2
Zero3:
	cmp r4, #0
	beq ge1E2
	BL MAX7219Send
	mov r4, #1
////////////////////////////
ge1E2:
	ldr r3, =#100
	cmp r2, r3
	blt Display2
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E2
Display2:
	sub r0, r0, 1
	cmp r1, #0
	beq Zero2

	cmp r4, #1
	beq NoLimit2
	bl ScanLimit
NoLimit2:
	BL MAX7219Send
	mov r4, #1
	mov r1, #0
	b ge1E1
Zero2:
	cmp r4, #0
	beq ge1E1
	BL MAX7219Send
	mov r4, #1
////////////////////////////
ge1E1:
	ldr r3, =#10
	cmp r2, r3
	blt Display1
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E1
Display1:
	sub r0, r0, 1
	cmp r1, #0
	beq Zero1

	cmp r4, #1
	beq NoLimit1
	bl ScanLimit
NoLimit1:
	BL MAX7219Send
	mov r4, #1
	mov r1, #0
	b ge1E0
Zero1:
	cmp r4, #0
	beq ge1E0
	BL MAX7219Send
	mov r4, #1
////////////////////////////
ge1E0:
	ldr r3, =#1
	cmp r2, r3
	blt Display0
	add r1, r1, #1
	subs r2, r2, r3
	b ge1E0
Display0:
	sub r0, r0, 1

	BL MAX7219Send

	mov r7, #0
	mov r0, #9

	mov r2, r9
	mov r9, r8
	mov r8, r2
Delay:
	//TODO: Write a delay 1sec function
	push {r0}
	ldr r0, =#150000
.Delay_loop:
	sub r0, r0, 1
	cmp r0, #0
	bne .Delay_loop
	pop {r0}

Detect1s:
	ldr r5, =GPIOC_IDR
	ldr r3, [r5]
	mov r4, #1
	lsl r4, #13
	ands r3, r4
	bne Loop

Delay1s:
	//TODO: Write a delay 1sec function
	push {r0}
	ldr r0, =#500000
.Delay1s_loop:
	sub r0, r0, 1
	cmp r0, #0
	bne .Delay1s_loop
	pop {r0}

	ldr r5, =GPIOC_IDR
	ldr r3, [r5]
	mov r4, #1
	lsl r4, #13
	ands r3, r4
	bne Loop
	b main


ScanLimit:
	push {r0, r1, lr}
	sub r1, r0, #1
	ldr r0, =#SCAN_LIMIT
	BL MAX7219Send
	pop {r0, r1, pc}

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

	// Set GPIOC Pin 13 as input mode: Press button
	ldr r8, =GPIOC_MODER
	ldr r0, [r8]
	ldr r2, =#0xF3FFFFFF
	and r0, r2
	str r0, [r8]

	//ldr  r1, =GPIOA_ODR

	BX LR

MAX7219Send:
	//input parameter: r0 is ADDRESS , r1 is DATA
	//TODO: Use this function to send a message to max7219
	push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, lr}
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

	pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, pc}
	BX LR

max7219_init:
	//TODO: Initialize max7219 registers
	push {r0, r1, r2, lr}

	ldr r0, =#DECODE_MODE
	ldr r1, =#0xFF
	BL MAX7219Send

	ldr r0, =#DISPLAY_TEST
	ldr r1, =#0x0
	BL MAX7219Send

	ldr r0, =#SCAN_LIMIT
	ldr r1, =0x0
	BL MAX7219Send

	ldr r0, =#INTENSITY
	ldr r1, =#0xA
	BL MAX7219Send

	ldr r0, =#SHUTDOWN
	ldr r1, =#0x1
	BL MAX7219Send

	pop {r0, r1, r2, pc}
	BX LR

