OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

SECTIONS {
    . = 0x0;
    
    .text : {
        *(init) 
        *(.text)
        . = ALIGN(4);
    }
    
    .bss : {
        _bss_start = .;     
        *( .bss )              
        . = ALIGN(4);
        _bss_end = .;        
    }

    .data : {
        *(.data)
        . = ALIGN(4);
    }
    .rodata : {
        *(.rodata)
        . = ALIGN(4);
    }
}