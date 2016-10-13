SECTIONS {
  . = 0x8000;

  .text : {
    *(.text*)
  }
  . = ALIGN(0x1000);

  _etext = .;
  . = ALIGN(0x1000);

  .rodata : {
    *(.rodata*)
  }
  . = ALIGN(0x1000);

  .data : {
    *(.data*)
  }
  . = ALIGN(0x1000);

  __bss_start = .;
  . = ALIGN(0x1000);

  .bss : {
    *(.bss*)
  }
  . = ALIGN(0x1000);

  _end = .;
}
