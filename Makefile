CC      := arm-none-eabi-gcc
AS      := arm-none-eabi-as
LD      := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy

crc_calc_tool_path := ./tools/crc_calc/crc_calc
uf2conv_path       := ./tools/uf2conv/uf2conf.py
uf2conv_args       := -b 0x10000000 -f 0xe48bff56

ASFLAGS := -mcpu=cortex-m0plus -mthumb
CFLAGS  := -mcpu=cortex-m0plus -mthumb -nostdlib -ffreestanding -Wall -Wextra -O0 -I.
LDFLAGS := -T link.ld -nostdlib

C_SRCS := main.c \
          reset_handler.c \
          hw/hw_resets.c \
          hw/hw_clocks.c \
          hw/hw_gpio.c \
          hw/hw_uart.c \
          hw/hw_irq.c \
          isr/uart0_isr.c \
          cmd/cmd.c \
          cmd/string_helper.c \
          cmd/uart_print.c \
          cmd/cmd_handlers.c
C_OBJS := $(C_SRCS:.c=.o)
OBJS := boot2/boot2.o vector_table.o $(C_OBJS)

all: firmware.uf2

firmware.uf2: firmware.bin
	python $(uf2conv_path) $(uf2conv_args) -c $< -o $@

firmware.bin: firmware.elf
	$(OBJCOPY) -O binary $< $@

firmware.elf: $(OBJS) link.ld
	$(LD) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

boot2/boot2.o: boot2/boot2.S boot2/crc.S
	$(AS) $(ASFLAGS) boot2/boot2.S boot2/crc.S -o $@

crc:
	$(AS) $(ASFLAGS) boot2/boot2.S -o boot2/boot2.o
	$(LD) boot2/boot2.o $(LDFLAGS) -o boot2/boot2.elf
	$(OBJCOPY) -O binary boot2/boot2.elf boot2/boot2.bin
	$(crc_calc_tool_path) boot2/boot2.bin > crc.txt
	rm -f boot2/boot2.o boot2/boot2.elf boot2/boot2.bin

upload:
	sudo mount /dev/sda1 ~/sda1
	sudo cp firmware.uf2 ~/sda1/
	sudo umount ~/sda1

clean:
	rm -f $(OBJS) firmware.elf firmware.bin firmware.uf2
