	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	user_stack_bottom: .zero 128
	expr_result: .word 0

.text
	.global main
	//postfix_expr: .asciz "-100 10 20 + - 10 +"
	.macro padded_string string, max
1:
    .ascii "\string"
2:
    .ifgt \max - (2b - 1b)
    .zero \max - (2b - 1b)
    .endif
    .endm

	postfix_expr:
		padded_string "70  0   -1 + 50 + -", 32
main:
	LDR r0, =postfix_expr
	ldr r9, =user_stack_bottom
	add r9, r9,  #128
	msr MSP, r9
	movs r5 , #0			// r5 = integer
	movs r1 , #0			// byte offset
	movs r11, #0			// negative
parse:
	mov r8, r3

	ldr r2 ,[r0, r1]		//r2 = [r0 + r1]
	add r1, #1
	and r3 , r2 , 0xFF		// 48-57

	cmp r3 , 0x20			//r3 == " "
	beq blank
	cmp r3, #45				//r3 == "-"
	beq negative
	cmp r3, # 43
	beq positive 			//r3 == "+"
	cmp r3, #0
	beq blank
	bl atoi
	b parse


	//TODO: Setup stack pointer to end of user_stack and calculate the
	//expression using PUSH, POP operators, and store the result into expr_result

// set r5 = 0
blank:
	cmp r8, #32
	beq parse
	cmp r11, #1
	beq nread
	cmp r11, #2
	beq onlypossign
	b back
	cmp r3, #0
	beq save

save:
	ldr r7, = expr_result
	pop {r8}
	str r8, [r7]
	b program_end

back:
	nop
	////
	//cmp r3, 0x20
	//beq parse
	////
	PUSH {r5}
	nop
	movs r5, #0
	b parse

onlynegsign:
	pop {r6}
	pop {r7}
	sub r8, r7, r6
	push {r8}
	cmp r3, #0
	beq save
 	b parse

onlypossign:
	mov r11, #0
	pop {r6}
	pop {r7}
	add r8, r6, r7
	push {r8}
	cmp r3, #0
	beq save
	b parse


nread:
	mov r11, #0
	cmp r5 , #0			// negative sign read
	beq onlynegsign
	mov r12 , #(-1)
	mul r5, r5, r12
	b back

program_end:
	nop
	B program_end

atoi:
	//TODO: implement a “convert string to integer” function
	subs r3 , r3 , #48
	movs r4 , #10
	mul r5, r5, r4
	adds r5, r5, r3
	BX LR

negative:
	mov r11, #1
	b parse

positive:
	mov r11, #2
	b parse
