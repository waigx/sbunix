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
#include <sys/sched.h>
#include <sys/keyboard.h>
#include <sys/managemem.h>
#include <sys/general.h>
#include <sys/debug.h>
#include <sys/file_op.h>


char g_screenshot[CONSOLE_ROW * CONSOLE_COL * 2];
char g_cwd[MAX_CWD_LEN];
uint16_t g_page_frame_pool[MAX_PAGE_FRAME];
uint8_t g_page_frame_buf[PAGE_SIZE * 4];
uint64_t g_next_free_frame_index = 0;
uint64_t g_page_frame_buf_ptr = 0;
uint16_t g_next_task_free_index = 1;
uint16_t g_next_task_index = 1;
uint16_t g_task_bump;
uint64_t g_frame_bump;
void *g_physbase;
void *g_physfree;
void *g_page_frame_start;
task_t *g_task_start;
task_t *gp_current_task;
vma_t *g_vma_start = (vma_t *)VMA_START;
vma_t *g_vma_end = ((vma_t *)VMA_START) + MAX_VMA_NUM;
vma_t *g_vma_phy_start;
struct file_descript *g_fdt_start = (struct file_descript *)(((vma_t *)VMA_START) + MAX_VMA_NUM); 
struct file_descript *g_fdt_end = (struct file_descript *)(((vma_t *)VMA_START) + MAX_VMA_NUM) + MAX_OPEN_FILE_DESCRIPT;
uint32_t g_current_pos = 2 * 21 * CONSOLE_COL;
uint8_t g_default_color = CONSOLE_WHITE_DARK;
uint8_t is_shifted = 0;
uint8_t is_ctrled = 0;
uint16_t g_timer_count = 0;
uint16_t g_switch_timer = 0;
uint8_t g_debug_mode = 0;
struct rtc_t g_time_boot = TIMEZONE_UTC;
char kb_queue[KEYBOARD_BUF_SIZE];
uint64_t start_queue = 0;
uint64_t end_queue = 0;



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
	debug_print("info", "Initializing kernel memory ...\n");
	init_kernel(physbase, physfree, physbottom, phystop);
	debug_print("info", "Finished ...\n");


	reload_idt();
	// only Keyboard intrrupt enable, others are masked by PIC.
	init_pic(ENABLE_KEYBOARD_INT | ENABLE_TIMER_INT);
	// now, Dongju disable timer interrupt because it makes me to debug difficult.
	// If you want to enable timer interrupt, add ENABLE_TIMER_INT with '|'

	load_test_tasks();
	set_timer(1000);
	__asm volatile("sti");// enable interupt("asm sti") should be executed after setting all interrupt info.
//	yield(1);
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
