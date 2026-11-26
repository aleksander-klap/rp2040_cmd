.syntax unified
.cpu cortex-m0plus
.thumb

.global _vector_table
.global _stack_start
.global reset_handler
.global uart0_isr
.align 2

.section .vector_table, "a"
_vector_table:
    
    @ first 16 ints (cortex-m0plus specific)
    .word _stack_start
    .word reset_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler
    .word default_handler

    @ 31 RP2040 ints
    .word default_handler   @ interrupt 0
    .word default_handler   @ interrupt 1
    .word default_handler   @ interrupt 2
    .word default_handler   @ interrupt 3
    .word default_handler   @ interrupt 4
    .word default_handler   @ interrupt 5
    .word default_handler   @ interrupt 6
    .word default_handler   @ interrupt 7
    .word default_handler   @ interrupt 8
    .word default_handler   @ interrupt 9
    .word default_handler   @ interrupt 10
    .word default_handler   @ interrupt 11
    .word default_handler   @ interrupt 12
    .word default_handler   @ interrupt 13
    .word default_handler   @ interrupt 14
    .word default_handler   @ interrupt 15
    .word default_handler   @ interrupt 16
    .word default_handler   @ interrupt 17
    .word default_handler   @ interrupt 18
    .word default_handler   @ interrupt 19
    .word uart0_isr         @ interrupt 20 
    .word default_handler   @ interrupt 21
    .word default_handler   @ interrupt 22
    .word default_handler   @ interrupt 23
    .word default_handler   @ interrupt 24
    .word default_handler   @ interrupt 25
    .word default_handler   @ interrupt 26 - never triggers
    .word default_handler   @ interrupt 27 - never triggers
    .word default_handler   @ interrupt 28 - never triggers
    .word default_handler   @ interrupt 29 - never triggers
    .word default_handler   @ interrupt 30 - never triggers
    .word default_handler   @ interrupt 31 - never triggers


.section .text, "ax"
.thumb_func
default_handler:
    b default_handler
