	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	result: .byte 0
.text
.global main
.equ X, 0x5555
.equ Y, 0xAA55
hamm:
	//TODO

	and r5, r4, #1		//r5 = val & 1
	movs r4, r4, lsr#1	//r4 /= 2
	cmp r5, #1			//r5 == 1?
	beq add				//Jump to add
	cmp r4, #0
	bne hamm
	bx lr

add:
	add r3, r3, #1		//r3 = result + 1
	b hamm

main:
	ldr r0, =X //This code will cause assemble error. Why? And how to fix.
	ldr r1, =Y
	ldr r2, =result
	ldr r3, [r2]
	eor r4, r0, r1		//r4 = val = X ^ Y
	bl hamm
	str r3, [r2]
	nop
L:
	b L
