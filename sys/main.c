#include <sys/color.h>
#include <sys/console.h>
#include <sys/sbunix.h>
#include <sys/gdt.h>
#include <sys/tarfs.h>
#include <sys/defs.h>


char *g_current_pos = (char *)(CONSOLE_START);
uint8_t g_default_color = CONSOLE_WHITE_DARK;

void setmem(void *start, void *end, char value);
void shiftmem(void *start, void *end, uint8_t offset);
void writenewline(int n);
void writechar(char c);
void writecharcolor(char c, uint8_t color);
void _writecharpos(char *pos, char c, uint8_t color);
void rollscreen(int n);

void start(uint32_t* modulep, void* physbase, void* physfree)
{
	char *s;
	char *s_ptr;
	s = "This is a test, test! aaa ";

	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;

	while(modulep[0] != 0x9001) modulep += modulep[1]+2;

	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
//			printf("Available Physical Memory [%x-%x]\n", smap->base, smap->base + smap->length);
		}
	}
//	printf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);
	// kernel starts here
	for (int i=0; i<501; i++) {
		for (s_ptr = s; *s_ptr; s_ptr ++) {
			 writechar(*s_ptr);
		}
		writenewline(1);
	}
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





void setmem(void *start, void *end, char value)
{
	while (start < end) {
		*(char *)start = value;
		start += 1;
	}
}


void shiftmem(void *start, void *end, uint8_t offset)
{
	while (start + offset < end) {
		*(char *)start = *((char *)(start) + offset);
		start += 1;
	}
}


void writechar(char c)
{
	writecharcolor(c, g_default_color);
}


void writecharcolor(char c, uint8_t color)
{
	_writecharpos(g_current_pos, c, color);
	g_current_pos += 2;
	if (g_current_pos >= (char *)CONSOLE_START + 2 * (CONSOLE_ROW - 1) * CONSOLE_COL) {
		rollscreen(1);
	}
	return;

}


void writenewline(int n)
{
	int i;
	uint64_t blank = n * CONSOLE_COL - (((uint64_t)g_current_pos - CONSOLE_START) / 2) % CONSOLE_COL;

	for (i = 0; i < blank; i++) {
		writecharcolor('\0', CONSOLE_BLACK_DARK);
	}
}


void rollscreen(int n)
{
	shiftmem((char *)CONSOLE_START, (char *)CONSOLE_START + 2 * CONSOLE_ROW * CONSOLE_COL, 2 * n * CONSOLE_COL);
	setmem((char *)CONSOLE_START + 2 * CONSOLE_ROW * CONSOLE_COL - 2 * n * CONSOLE_COL, (char *)CONSOLE_START + 2 * CONSOLE_ROW * CONSOLE_COL, 0);
	g_current_pos -= 2 * n * CONSOLE_COL;
}


void _writecharpos(char *pos, char c, uint8_t color)
{
	*pos = c;
	*(pos + 1) = color;
}
