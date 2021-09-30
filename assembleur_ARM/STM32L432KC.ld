/* Point d'entrée */
ENTRY(Reset_Handler)

/* Pile (stack) */
_estack = 0x20010000;    /* fin de la RAM */

/* Mémoire présentes */
MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 64K
FLASH (rx)     : ORIGIN = 0x8000000,  LENGTH = 256K
}

/* Organisation */
SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(8);
    KEEP(*(.isr_vector))
    . = ALIGN(8);
  } >FLASH

  .text :
  {
    . = ALIGN(8);
    *(.text)       /* sections code */
    *(.text*)
    *(.glue_7)     /* glue arm vers thumb code */
    *(.glue_7t)    /* glue thumb vers arm code */
  } >FLASH

  /* Suppression des info des libs standard */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}


