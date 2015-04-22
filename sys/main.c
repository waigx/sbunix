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
#include <sys/managemem.h>
#include <sys/general.h>
#include <sys/debug.h>
#include <stdarg.h>
#include <string.h>
#include <const.h>
#include <type.h>
#include <sys/sched/sched.h>

char g_screenshot[CONSOLE_ROW * CONSOLE_COL * 2];
uint16_t g_page_frame_pool[MAX_PAGE_FRAME];
uint64_t g_next_free_frame_index = 0;
uint16_t g_next_task_free_index = 1;
uint16_t g_next_task_index = 1;
uint16_t g_task_bump;
void *g_physbase;
void *g_physfree;
void *g_page_frame_start;
task_t *g_task_start;
task_t *gp_current_task;
uint32_t g_current_pos = 2 * 21 * CONSOLE_COL;
uint8_t g_default_color = CONSOLE_WHITE_DARK;
uint8_t is_shifted = 0;
uint8_t is_ctrled = 0;
uint8_t g_timer_count = 0;
uint8_t g_debug_mode = 0;
struct rtc_t g_time_boot = TIMEZONE_UTC;


void start(uint32_t* modulep, void* physbase, void* physfree)
{
	void *phystop = 0;
	void *physbottom = 0;
	
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;

	while(modulep[0] != 0x9001) modulep += modulep[1]+2;

	// Init screen
	screenshot();
	rollscreen(4);

	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
			physbottom = (void *)(smap->base);
			phystop = (void*)(smap->base + smap->length);
		}
	}
	printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	printf("physbase: %p, physfree: %p\n", physbase, physfree);

	// kernel starts here
	// Initial kernel
	printf("[Kernel]: Initializing kernel memory ...\n");
	init_kernel(physbase, physfree, physbottom, phystop);
	printf("[Kernel]: Finished.\n");

//	// scheduler
//	reload_idt();
//	__asm volatile("sti");
//	round_robin_scheduler();
//	while(1);
//	///////////////////////

	reload_idt();
	// only Keyboard intrrupt enable, others are masked by PIC.
	init_pic(ENABLE_KEYBOARD_INT | ENABLE_TIMER_INT);
	// now, Dongju disable timer interrupt because it makes me to debug difficult.
	// If you want to enable timer interrupt, add ENABLE_TIMER_INT with '|'
	set_timer(100);
	__asm volatile("sti");// enable interupt("asm sti") should be executed after setting all interrupt info.

	while (1);

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
