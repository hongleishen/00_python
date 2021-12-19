OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(__start)
SECTIONS
{
	. = 0x00000000;
	. = ALIGN(4);
	.text :
	{
		_text = .;
		*(.vectors*)
		__vectors_end = .;
		CPUDIR/start.o	(.text*)
		*(._INIT_START);
		*(._INIT_EARLY);
		*(._INIT_CORE);
		*(._INIT_POST_CORE);
		*(._INIT_ARCH);
		*(._INIT_SUBSYS);
		*(._INIT_FS);
		*(._INIT_DEV);
		*(._INIT_LATE);
		*(._INIT_END);
		*(.text*)
	}

	. = ALIGN(4);
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }
	_etext = .;
	_text_size = . - _text;
	.data : {
		_data = .;
		*(.data*)
	}

	. = ALIGN(4);

	.image_copy_end :
	{
		*(.__image_copy_end)
	}

	.rel_dyn_start : { *(.__rel_dyn_start) }
	.rel.dyn :
	{
		*(.rel*)
	}
	.rel_dyn_end : { *(.__rel_dyn_end) }

	.end :
	{
		*(.__end)
	}

	_image_binary_end = .;

	. = ALIGN(4096);
	.mmutable : {
		__mmu_table = .;
		*(.mmutable)
	}

/*
 * Compiler-generated __bss_start and __bss_end, see arch/arm/lib/bss.c
 * __bss_base and __bss_limit are for linker only (overlay ordering)
 */

	.bss_start : {
		KEEP(*(.__bss_start));
		__bss_base = .;
	}

	.bss __bss_base (OVERLAY) : {
		*(.bss*)
		 . = ALIGN(4);
		 __bss_limit = .;
	}

	.bss_end __bss_limit (OVERLAY) : {
		KEEP(*(.__bss_end));
	}

	/DISCARD/ : {
		*(.rel.reloc)
		*(.eh_frame)
		*(.note.GNU-stack)
		*(.dynsym)
		*(.dynstr)
		*(.note.gnu.build-id)
		*(.comment)
	}
}
