# rp2040_cmd

A simple bare-metal command line for the RP2040 (Raspberry Pi Pico), over UART - no SDK, no libc.
I wrote is on the way of learning MCU's and bare metal programming - mostly for debugging.
It is a lot of fun not only learning about hardware architecture - but also writing own
libc - like `strlen` or `streq`.

## Credits
Datasheet is the groundtruth for this code - [RP2040 datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf).   
Also would not be able to learn without those guys:
- [vxj9800 RP2040 BareMetal Step-by-step guide](https://github.com/vxj9800/bareMetalRP2040) 
- [Life with David Baremetal Adventures](https://www.youtube.com/playlist?list=PLiRALtgGsxmYTF6ZqDSg4DRWDLGVer-Kb)

## This is Early development
For now, most of settings are hardcoded - and functions are doing exactly one thing that 
is aimed for this use case. For example, `hw\hw_uart0_init()` takes not parameters and
sets it up with hardcoded UART settings: 115200 baudrate, UART fifos diasbled etc. I want 
to change this in future.   
Also - the `commands handlers` are directly calling UART functionality - will be split in future
for some more elegant solution - like stdin & stdout.
There is read command (read memory value) - and the write is coming (also the backspace) :)

## Commands

| Command | Description |
|---|---|
| `led on` / `led off` / `led toggle` | Controls the on-board LED (GP25). |
| `read 0xXXXXXXXX` | Reads the 32-bit word at that address and prints it in hex. |

`read` needs the address as `0x` plus exactly 8 hex digits, for example:

```
read 0x40024000
Hex: 0x00fabaa0
```

This reads `XOSC_CTRL`. The value shows the crystal oscillator is enabled, with range 1–15 MHz.

> Reading an address that doesn't exist (an unmapped region, or a peripheral that is
> held in reset) raises a HardFault. The firmware then hangs in `default_handler`
> until you reset the board.

## Connecting

The command line runs on **UART0**, not on the Pico's USB port. The firmware has no
USB stack. You need a USB-to-UART adapter with **3.3 V** logic, e.g. a CP2102, CH340
or FT232.

| Pico | Adapter |
|---|---|
| GP0 – UART0 TX (pin 1) | RX |
| GP1 – UART0 RX (pin 2) | TX |
| GND (pin 3) | GND |

Then open a terminal at **115200 8N1**, no flow control. For example:

```
picocom -b 115200 /dev/ttyUSB0
```

You exit picocom with `Ctrl-A` then `Ctrl-X`. The firmware echoes each character you
type, so leave local echo off. Commands end with Enter (`\r`).

Input limits:

- Max 18 characters per line, not counting Enter. A longer line is dropped.
- No backspace. A typo makes the command unknown, so just press Enter and type it again.
- Arguments are separated by spaces, max `ARGV_MAX` (5) tokens.

## Building and flashing

You need `arm-none-eabi-gcc` / `binutils` and `python`.
The RP2040 needs an .uf2 format that is formed out of binary - I used the [Microsoft uf2conv](https://github.com/honorx/uf2conv) (thats what you need python for)
```
make            # builds firmware.elf / .bin / .uf2
make clean
```

To flash, hold BOOTSEL while you plug in the Pico, then copy `firmware.uf2` to the
`RPI-RP2` drive. `make upload` does the same, but it assumes the drive is at
`/dev/sda1` and mounts it at `~/sda1`. Check those values for your machine.

`make crc` recalculates the boot2 CRC with `tools/crc_calc` (built with `make` in
`tools/crc_calc/`). You only need it after changing `boot2/boot2.S`.

## Adding your own command

A command is a handler function plus one entry in the `commands[]` table in
`cmd/cmd_handlers.c`. The parser splits the line on spaces and calls the handler whose
name matches the first word. `argv[0]` is the command name.

1. Write the handler in `cmd/cmd_handlers.c`:

   ```c
   __attribute__((section(".text.cmd.cmd_handlers")))
   void cmd_hello_handler(uint8 argc, char *argv[]){
       if (argc != 2) {
           uart_puts("Usage: hello <name>", 1);
           return;
       }
       uart_puts("Hello ", 0);
       uart_puts(argv[1], 1);
   }
   ```

2. Declare it in `cmd/cmd_handlers.h`:

   ```c
   __attribute__((section(".text.cmd.cmd_handlers")))
   void cmd_hello_handler(uint8 argc, char *argv[]);
   ```

3. Add it to the table **and increase `num_commands`**:

   ```c
   const uint8 num_commands = 3;
   const command_t commands[] = {
       {"led", cmd_led_handler},
       {"read", cmd_read_handler},
       {"hello", cmd_hello_handler}
   };
   ```

   `num_commands` is not calculated from the table. If you forget to change it, the new
   command is never found.

For output, use the helpers in `cmd/uart_print.h` (`uart_putc`, `uart_puts`,
`uart_putnl`). For parsing, use the ones in `cmd/string_helper.h` (`string_eq`, `strlen`,
`str_addr_to_hex`, `uint32_to_str_hex`).

## How it works

1. `reset_handler` sets up the clocks (XOSC → PLL → 125 MHz `clk_sys`, ROSC off),
   copies `.data` to RAM, zeroes `.bss` and calls `main`.
2. `setup()` takes IO_BANK0 and UART0 out of reset, sets up the LED and UART0 pins,
   starts `clk_peri` and UART0, and enables the UART0 interrupt in the NVIC.
3. `uart0_isr` runs for every received byte. It echoes the byte and stores it in
   `uart_buff`. On `\r` it sets `command_ready`.
4. The main loop sees `command_ready` and calls `parse_command()`. That function
   tokenizes the line, looks up the command in `commands[]` and calls its handler.

## Hardcoded settings

| What | Value |
|---|---|
| Crystal (XOSC) | 12 MHz, startup delay ~1 ms |
| `clk_ref` | XOSC / 1 = 12 MHz |
| `clk_sys` | PLL_SYS: 12 MHz × 125 / 6 / 2 = **125 MHz** |
| `clk_peri` | XOSC = 12 MHz |
| UART0 | 115200 baud (IBRD 6, FBRD 33), 8N1, FIFOs off, RX interrupt per byte |
| UART pins | GP0 TX, GP1 RX |
| LED | GP25 (SIO output) |
| Line buffer | `UART_BUFF_SIZE` 20 bytes, `ARGV_MAX` 5 |

## Project layout

```
├── main.c            setup() + main loop
├── reset_handler.c   clocks, .data/.bss init, jump to main
├── vector_table.S    Cortex-M0+ and RP2040 interrupt vectors
├── link.ld           flash / SRAM layout
├── globals.h         uint8/16/32, REG32 and atomic alias macros (SET/CLR/XOR)
├── boot2/            second-stage bootloader (QSPI flash setup) + CRC
├── hw/               register-level code: clocks, resets, gpio, uart, irq
├── isr/              uart0_isr
├── cmd/              parser, command handlers, string + UART print helpers
└── tools/            crc_calc and uf2conv
```
