#include <sys/color.h>
#include <sys/console.h>
#include <sys/sbunix.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/kio.h>
#include <sys/idt.h>
#include <sys/isr.h>
#include <sys/pic.h>
#include <sys/timer.h>
#include <sys/rtc.h>
#include <sys/keyboard.h>
#include <stdarg.h>
#include <string.h>
#include <const.h>
#include <type.h>


char *g_current_pos = (char *)(CONSOLE_START);
uint8_t g_default_color = CONSOLE_WHITE_DARK;


void start(uint32_t* modulep, void* physbase, void* physfree)
{
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;

	while(modulep[0] != 0x9001) modulep += modulep[1]+2;

	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
		}
	}
	printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	// kernel starts here
	reload_idt();
	init_pic(ENABLE_KEYBOARD_INT);	// only Keyboard intrrupt enable, others are masked by PIC.
					// now, Dongju disable timer interrupt because it makes me to debug difficult.
					// If you want to enable timer interrupt, add ENABLE_TIMER_INT with '|'
	set_timer(10000);		
	__asm volatile("sti");		// enable interupt("asm sti") should be executed after setting all interrupt info.


}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;
struct tss_t tss;

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	register char *s, *v;
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	reload_gdt();
	setup_tss();
	start(
		(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
	s = "!!!!! start() returned !!!!!";
	for(v = (char*)0xb8000; *s; ++s, v += 2) *v = *s;
	while (1);
}
