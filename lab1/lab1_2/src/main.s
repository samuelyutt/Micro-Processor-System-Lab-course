.syntax unified
	.cpu cortex-m4
	.thumb
.text
.global main
.equ N, -3

// r0 = N
// r1, r2
// r3 = temp
// r4 = ans
// r5 = remaining count

fib: //TODO
	adds r3, r1, r2
	bvs Overflow
	movs r1, r2
	movs r2, r3
	subs r5, r5, 1
	cmp r5, #0
	bne fib
	bx lr
Overflow:
	movs r3, #-2
	bx lr

Nsmall:
	movs r4, #1
	B L

outrange:
	movs r4, #-1
	B L

main:
	movs r0, #N
	cmp r0, #1
	blt outrange
	cmp r0, #100
	bgt outrange

	cmp r0, #3
	blt Nsmall

	movs r1, #1
	movs r2, #1
	sub r5, r0, 2
	bl fib
	movs r4, r3
	nop
L: b L
