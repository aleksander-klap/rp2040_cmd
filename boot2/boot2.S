.syntax unified
.cpu cortex-m0plus
.thumb

@ export this section for linker
.global boot2
.section .boot2, "ax"
@ vars
.equ SSI_BASE,                  0x18000000
.equ SSI_CTRL0_OFFSET,          0x00
.equ SSI_ENR_OFFSET,            0x08
.equ SSI_BAUD_OFFSET,           0x14
.equ SSI_DR0_OFFSET,            0x60
.equ SSI_SR_OFFSET,             0x28
.equ SSI_SPI_CTRLR0_OFFSET,     0xf4
.equ M0PLUS_VTOR,               0xe000ed08
.equ SSI_CTRLR0_CFG1,           0x005f0300
.equ SSI_SPI_CTRLR0_CFG1,       0x00002221
.equ SSI_SPI_CTRLR0_CFG2,       0xa0002022
.equ VTOR_ADDR,                 0x10000100

boot2:
    ldr r0, =SSI_BASE
    ldr r5, =SSI_SPI_CTRLR0_OFFSET
    @ disable ssi
    bl disable_ssi
    @ set clock divider
    movs r1, #0x04
    str r1, [r0, #SSI_BAUD_OFFSET]
    @ set DFS_32 - data frame size - set to 8
    @ this is intermidiate - only to set up Flash for qSPI mode
    movs r1, #0x07
    lsls r1, r1, #16
    str r1, [r0, #SSI_CTRL0_OFFSET]
    @ enable_ssi
    bl enable_ssi
    @ ask Flash for its status registers (2 x 8bit)
    movs r1, #0x05
    str r1, [r0, #SSI_DR0_OFFSET]
    movs r1, #0x35
    str r1, [r0, #SSI_DR0_OFFSET]
    @ wait until commands are send
    bl ssi_done_loop
    @ read flash status registers
    ldr r2, [r0, #SSI_DR0_OFFSET]
    ldr r3, [r0, #SSI_DR0_OFFSET]
    @check if QUAD ENABLE bit in Flash statis register 2 is set
    lsls r1, r3, #30
    @ if so, skip setting it up
    bmi flash_qe_set
    @ if not set - needs to be set
    @ first - enable write into Flash (0x06 command) 
    movs r1, 0x06 
    str r1, [r0, #SSI_DR0_OFFSET]
    @ send cmd to write status registers 1 & 2
    movs r1, #0x01
    str r1, [r0, #SSI_DR0_OFFSET]
    @ write sr1 (unchanged)
    str r2, [r0, #SSI_DR0_OFFSET]
    @ set Quad Enable in sr2 and write
    movs r1, #0x02
    orrs r3, r3, r1
    str r3, [r0, #SSI_DR0_OFFSET]
    @ disable write in Flash
    movs r1, #0x04
    str r1, [r0, #SSI_DR0_OFFSET]
    @ wait till all writes done
    bl ssi_done_loop
flash_qe_set:
    @ disable ssi
    bl disable_ssi
    @ write SSI_CTRL0 config
    @ SPI_FRF -> 0x02 = Quad SPI format
    @ DFS_32 -> 0x31 = 32 clock cycles per frame
    @ TMOD = EEPROM_READ
    ldr r1, =SSI_CTRLR0_CFG1
    str r1, [r0, #SSI_CTRL0_OFFSET]
    @ set first SSI SPI config for QuadSPI
    ldr r1, = SSI_SPI_CTRLR0_CFG1
    str r1, [r0, r5]
    @ enable ssi
    bl enable_ssi
    @ send dummy instructions to lock continous mode in Flash for 0xEB read inst
    movs r1, #0xeb
    str r1, [r0, #SSI_DR0_OFFSET]
    movs r1, #0xa0
    str r1, [r0, #SSI_DR0_OFFSET]
    bl ssi_done_loop
    @ set final setting of SSI SPI QuadSPI
    bl disable_ssi
    ldr r1, =SSI_SPI_CTRLR0_CFG2
    str r1, [r0, r5]
    bl enable_ssi
    @ set VTOR (addr of vector table)
    ldr r0, =M0PLUS_VTOR
    ldr r1, =VTOR_ADDR
    str r1, [r0]
    @ copy first VTOR entry to MSP
    ldr r2, [r1]
    msr msp, r2
    @ call second entry in Vector Table
    ldr r2, [r1, #0x04]
    bx r2
@ unreachable
hang:
    b hang
ssi_done_loop:
    ldr r1, [r0, #SSI_SR_OFFSET]
    lsls r2, r1, #31
    bmi ssi_done_loop
    lsls r2, r1, #29
    bpl ssi_done_loop
    bx lr
disable_ssi:
    movs r1, #0x00
    str r1, [r0, #SSI_ENR_OFFSET]
    bx lr
enable_ssi:
    movs r1, #0x01
    str r1, [r0, #SSI_ENR_OFFSET]
    bx lr

