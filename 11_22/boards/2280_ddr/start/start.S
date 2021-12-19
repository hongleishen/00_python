#include "linkage.h"

	/* Stack Sizes */
	.set  UND_STACK_SIZE, 0x00000010
	.set  ABT_STACK_SIZE, 0x00000010
	.set  FIQ_STACK_SIZE, 0x00000010
	.set  IRQ_STACK_SIZE, 0x00000800
	.set  SVC_STACK_SIZE, 0x00000800

	/* Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs */
	.set  MODE_USR, 0x10        /* User Mode */
	.set  MODE_FIQ, 0x11        /* FIQ Mode */
	.set  MODE_IRQ, 0x12        /* IRQ Mode */
	.set  MODE_SVC, 0x13        /* Supervisor Mode */
	.set  MODE_ABT, 0x17        /* Abort Mode */
	.set  MODE_UND, 0x1B        /* Undefined Mode */
	.set  MODE_SYS, 0x1F        /* System Mode */
	.equ  I_BIT, 0x80           /* when I bit is set, IRQ is disabled */
	.equ  F_BIT, 0x40           /* when F bit is set, FIQ is disabled */

	.globl  reset
reset:
	ldr r0, =(CONFIG_SP_START_ADDR)
	bic r0, r0, #7  /* 8-byte alignment for ABI compliance */

	msr     CPSR_c, #MODE_IRQ|I_BIT|F_BIT /* IRQ Mode */
	mov     sp, r0
	sub     r0, r0, #IRQ_STACK_SIZE
	msr     CPSR_c, #MODE_UND|I_BIT|F_BIT /* Undefined Instruction Mode */
	mov     sp, r0
	sub     r0, r0, #UND_STACK_SIZE
	msr     CPSR_c, #MODE_ABT|I_BIT|F_BIT /* Abort Mode */
	mov     sp, r0
	sub     r0, r0, #ABT_STACK_SIZE
	msr     CPSR_c, #MODE_FIQ|I_BIT|F_BIT /* FIQ Mode */
	mov     sp, r0
	sub     r0, r0, #FIQ_STACK_SIZE
	msr     CPSR_c, #MODE_SVC|I_BIT|F_BIT /* Supervisor Mode */
	mov     sp, r0
	sub     r0, r0, #SVC_STACK_SIZE
	msr     CPSR_c, #MODE_SYS|I_BIT|F_BIT /* System Mode */
	mov     sp, r0
	nop

	ldr r0, =__bss_start    /* this is auto-relocated! */
	ldr r1, =__bss_end      /* this is auto-relocated! */
	mov r2, #0x00000000     /* prepare zero to clear BSS */

clbss_l:
	cmp r0, r1              /* while not at end of BSS */
	strlo   r2, [r0]        /* clear 32-bit BSS word */
	addlo   r0, r0, #4      /* move to next */
	blo clbss_l
	nop

	/* enter_svc_and_close_fiq*/
	ldr     r0, =0xd3
	msr     cpsr_c, r0

	bl  main
	nop

loop:
	nop
	b loop
	nop

exit:
	b exit
	.size exit, . - exit

	.globl __start
	.section .vectors, "ax"

__start:
	b       reset
	ldr     pc, =do_undefined_instruction
	ldr     pc, =vPortYieldProcessor
	ldr     pc, =do_prefetch_abort
	ldr     pc, =do_data_abort
	ldr     pc, =do_not_used
	ldr     pc, =do_irq
	ldr     pc, =do_fiq