OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY {
        FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1M
        RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 512K
}

SECTIONS {

    /* Vecteurs au début de la FLASH */
    .vectors : {
        *(.vectors)
    } > FLASH

    /* Code en FLASH */
    .text : {
        *(.text)
        *(.rodata)
        . = ALIGN(4);
    } > FLASH

    /* Les données initialisées, càd exécution en RAM, chargement depuis FLASH */
    .data : {
        _sdata = .;
        *(.data)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> FLASH 

    /* Adresse en FLASH d'où copier les .data */
    _sidata = LOADADDR(.data);

    /* BSS (données non init en RAM) */
    .bss : {
        _bss_start = .;
        *( .bss )
        . = ALIGN(4);
        _bss_end = .;
    } > RAM
}