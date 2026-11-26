ENTRY(boot2)

MEMORY
{
    flash(rx) : ORIGIN = 0x10000000, LENGTH = 2048k
    sram(rwx) : ORIGIN = 0x20000000, LENGTH = 256k
}

SECTIONS
{
    .boot2 :
    {
        _start_boot2 = .;
        *(.boot2*)
        _end_boot2 = .;
        . = . + (252 - (_end_boot2 - _start_boot2));
        *(.crc*)
    } > flash

    .text :
    {
        *(.vector_table*)
        *(.text.reset_handler*)
        *(.text*)
    } > flash
    
    .data :
    {
        _start_data = .;
        *(.data*)
        _end_data = .;
    } > sram AT > flash

    .bss (NOLOAD) :
    {
        _start_bss = .;
        *(.bss*)
        _end_bss = .;
    } > sram

    .stack (NOLOAD):
    {
        . = ORIGIN(sram) + LENGTH(sram);
        _stack_start = .;
    } > sram

    _start_data_flash = LOADADDR(.data);
}

