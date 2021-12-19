/******************************************************************************
  All rights reserved.

  @brief        irq handler
  @creator      Qianyu Liu

  @History
      When          Who             What, where, why
      ----------    ------------    ----------------------------------------
      2019/10/29    Qianyu Liu      the initial version
******************************************************************************/
#include "vic_int.h"
#include "portmacro.h"

int interrupt_flg = EXIT_INTERRUPT;

void do_undefined_instruction(void)
{
	imi_printf("undefined_instruction\n");
	while(1) ;
}

void do_software_interrupt(void)
{
	imi_printf("software_interrupt\n");
	while(1) ;
}

void do_prefetch_abort(void)
{
	imi_printf("prefetch_abort");
	while(1) ;
}

void do_data_abort(void)
{
	imi_printf("data_abort\n");
	while(1) ;
}

void do_not_used(void)
{
	imi_printf("not_used\n");
	while(1) ;
}

void do_irq(void) __attribute__((naked));
void do_irq(void)
{
	portSAVE_CONTEXT();
	interrupt_flg = ENTER_INTERRUPT;
	vic_handle_irq();
	interrupt_flg = EXIT_INTERRUPT;
	portRESTORE_CONTEXT();
}

void do_fiq(void)
{
	imi_printf("fiq\n");
	while(1) ;
}