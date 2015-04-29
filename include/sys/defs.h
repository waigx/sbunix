#ifndef _DEFS_H
#define _DEFS_H

#define NULL ((void*)0)

#define	TRUE	1
#define	FALSE	0

typedef unsigned long  uint64_t;
typedef          long   int64_t;
typedef unsigned int   uint32_t;
typedef          int    int32_t;
typedef unsigned short uint16_t;
typedef          short  int16_t;


typedef unsigned char   uint8_t;

typedef unsigned long    size_t;
typedef          int   intptr_t;

typedef      uint32_t    kpid_t;
typedef      uint64_t    size_t;

typedef      uint64_t    cr0e_t;
typedef      uint64_t    cr3e_t;
typedef      uint64_t   pml4e_t;
typedef      uint64_t    pdpe_t;
typedef      uint64_t     pde_t;
typedef      uint64_t     pte_t;

#endif
