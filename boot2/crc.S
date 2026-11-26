.syntax unified
.cpu cortex-m0plus
.thumb

.section .crc, "ax"
.align 2
.global crc

crc:
    .word 0xA35A96EB
