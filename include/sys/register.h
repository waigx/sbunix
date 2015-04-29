#ifndef _REGISTER_H
#define _REGISTER_H

#include <sys/defs.h>


uint64_t get_rax_register(void);
uint64_t get_rdi_register(void);
uint64_t get_rsi_register(void);
uint64_t get_rdx_register(void);
uint64_t get_r10_register(void);

uint64_t get_cr0_register(void);
uint64_t get_cr2_register(void);
uint64_t get_cr3_register(void);

void clearcr0bit(uint64_t bit);

static __inline void load_cr3(cr3e_t cr3)
{
	__asm__ __volatile__(
		"movq %0, %%cr3\n\t"
		: 
		: "r" (cr3));
}

static __inline void load_cr0(cr0e_t cr0)
{
	__asm__ __volatile__(
		"movq %0, %%cr0\n\t"
		: 
		: "r" (cr0));
}

#endif
