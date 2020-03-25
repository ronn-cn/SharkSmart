
/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20030000;    /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x4000;      /* required amount of heap  */
_Min_Stack_Size = 0xA000; /* required amount of stack */

/*
RAM 	鍐呴儴RAM
FlASH 	鍐呴儴FLASH
EXTSRAM	澶栭儴SRAM
CCMSRAM	鍐呴儴CCM
EXTABLE	malloc.c涓唴瀛樼鐞嗙敤
CCMTABLE	malloc.c涓唴瀛樼鐞嗙敤
LCDMEM0	鏄惧�?
*/


/* Specify the memory areas */
MEMORY
{
RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 192K
FLASH (rx)      : ORIGIN = 0x08040000, LENGTH = 768K
EXTSRAM(xrw)    : ORIGIN = 0XC0600000, LENGTH = 20480K 
CCMSRAM(xrw)    : ORIGIN = 0X10000000, LENGTH = 60K
EXTABLE(xrw)    : ORIGIN = 0xC1A00000, LENGTH = 1807K
CCMTABLE(xrw)   : ORIGIN = 0x1000f000, LENGTH = 4K
LCDMEM0(xrw)     : ORIGIN = 0xC0000000, LENGTH = 750K
LCDMEM1(xrw)     : ORIGIN = 0xC0BB8000, LENGTH = 750K
}

/* Define output sections */
SECTIONS
{
  /* The startup code goes first into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } >FLASH

  /* The program code and other data goes into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >FLASH

  
  
  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

   
  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >RAM AT> FLASH


  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >RAM

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM


  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
/* 鍙敤澶栭儴鍐呭�? */
  .extsram 0XC01F4000 (NOLOAD):
  {
    . = ALIGN(4);
	*(.exsram)
	*(.exsram*)
	. = ALIGN(4);

  } AT > EXTSRAM
/* 鍙敤CCM鍐呭�? */
  .ccmsram 0x10000000 (NOLOAD):
  {
    . = ALIGN(4);
	*(.ccmsram)
	*(.ccmsram*)
	. = ALIGN(4);

  } AT > CCMSRAM
/* 澶栭儴鍐呭瓨绠＄�? */  
  .exttable 0xC1E30000 (NOLOAD):
  {
   . = ALIGN(4);
	*(.extable)
	*(.extable*)
   . = ALIGN(4);
  } AT > EXTABLE
/* 鍐呴儴CCM鍐呭瓨绠＄悊 */
  .ccmtable 0x1000f000 (NOLOAD):
  {
   . = ALIGN(4);
	*(.ccmtable)
	*(.ccmtable*)
   . = ALIGN(4);
  } AT > CCMTABLE
/* LCD鏄惧�? */
  .lcdmem0 0xC0000000 (NOLOAD):
  {
   . = ALIGN(4);
	*(.lcdmem0)
	*(.lcdmem0*)
   . = ALIGN(4);
  } AT > LCDMEM0

  .lcdmem1 0xC0BB8000 (NOLOAD):
  {
   . = ALIGN(4);
	*(.lcdmem1)
	*(.lcdmem1*)
   . = ALIGN(4);
  } AT > LCDMEM1
}


