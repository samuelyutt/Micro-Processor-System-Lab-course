.syntax unified
.cpu cortex-m4
.thumb
.data
    arr1: .byte 0x19, 0x34, 0x14, 0x32, 0x52, 0x23, 0x61, 0x29
    arr2: .byte 0x18, 0x17, 0x33, 0x16, 0xFA, 0x20, 0x55, 0xAC
.text
.global main

do_sort: //TODO
    adds r1, r1, #1          // i++
    movs r2, -1              // r2 = j
    cmp r1, #7               // if i >= 7: return
    blt next
    bx lr
next:
    // if j >= 7-i: do_sort
    adds r2, r2, #1          // j++
    movs r7, #7
    subs r6, r7, r1          // r6 = length - 1 - i = 7 - i
    cmp r2, r6
    bge do_sort
    ldrb r3, [r0, r2]        // r3 = arr[j]
    adds r12, r2, #1         // r12 = j+1
    ldrb r4, [r0, r12]       // r4 = arr[j+1]
    cmp r3, r4
    //blt next				// small to big
    bgt next				// big to small
    // if arr[j] > arr[j+1]: swap
    movs r5, r4              // r5 = temp = r4
    strb r3, [r0, r12]
    strb r4, [r0, r2]
    b next
main:
    ldr r0, =arr1
    movs r1, #-1             //r1 = i
    bl do_sort
    movs r1, #-1             //r1 = i
    ldr r0, =arr2
    bl do_sort
L:
    b L
