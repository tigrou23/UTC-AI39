.code 16
.syntax unified

.section .vectors, "a", %progbits
.align 4

.long 0x20020000 //top de la RAM
.long _start     //vecteur de reset

.section init, "ax", %progbits
.align 4

.global _start
.thumb_func
_start:
    ldr   sp, = 0x2001FFFC   //registre de la pile (pointeur de pile je crois)
    bl 	  _copy_data         //pour copier le contenu
    bl 	  _init_bss
    bl    main
    b     .  				// boucle infinie

_init_bss:
    ldr   r0, =_bss_start
    ldr   r1, =_bss_end
    mov   r2, #0
.bss_loop:
    cmp   r0, r1
    bge   .done_bss
    str   r2, [r0], #4
    b     .bss_loop
.done_bss:
	bx lr
