#ifndef _REGISTER_H
#define _REGISTER_H

#include <sys/defs.h>


uint64_t get_rax_register(void);
uint64_t get_rdi_register(void);
uint64_t get_rsi_register(void);
uint64_t get_rdx_register(void);
uint64_t get_r10_register(void);

uint64_t get_cr3_register(void);

#endif
