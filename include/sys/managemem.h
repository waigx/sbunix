/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sbunix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sbunix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sbunix.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _MANAGEMEM_H
#define _MANAGEMEM_H


#include <sys/defs.h>
#include <sys/sched/sched.h>


#define CR3_PWT                                                    0x8
#define CR3_PCD                                                   0x10

#define PTE_PRESENTS                                               0x1
#define PTE_WRITEABLE                                              0x2
#define PTE_SUPER                                                  0x4
#define PTE_PWT                                                    0x8
#define PTE_PCD                                                   0x10

#define PTE_DIRTY                                                 0x40

#define PAGE_SIZE_LOG2                                              12
#define PAGE_SIZE                                (1 << PAGE_SIZE_LOG2)
#define MAX_PAGE_FRAME                                       (1 << 20)
#define PAGE_TABLE_ENTRY_NUM                                  (1 << 9)
#define KERNEL_SPACE_START                          0xffffffff80000000
#define KERNEL_PROC_HEAP_SIZE                                  0xfffff

#define VADDR_SIGN_EXTEND                                           16
#define VADDR_PML4E                                                  9
#define VADDR_PDPE                                                   9
#define VADDR_PDE                                                    9
#define VADDR_PTE                                                    9
#define VADDR_OFFSET                                                12

#define EXTEND_BIT_1                                0xffff000000000000

#define USER_STACK_START              (KERNEL_SPACE_START - PAGE_SIZE)


/* 
 * PML4E self-ref. address;
 * Use index of 510;
 * 
 *                                 |<-- PTE operate start here 
 *                                 |           |<-- PDE operate start here
 *                                 |           |           |<-- PDPE operate start here
 * 1111 1111 1111 1111. 1111 1111 0.111 1111 10.11 1111 110.1 1111 1110. 0000 0000 0000
 * -------------------| -----------|-----------|-----------|-----------| --------------
 *    F    F    F    F|    F    F  |  7    F   | B    F    |D    F    E|    0    0    0
 * -------------------| -----------|-----------|-----------|-----------| --------------
 *         Sign Extend|       PML4E|       PDPE|        PDE|        PTE|         Offset
 * -------------------| -----------|-----------|-----------|-----------| --------------
 *                    |            |           |           |           | 
 */
#define PAGE_SELF_REF                  0xffffff7fbfdfe000


extern uint16_t g_page_frame_pool[MAX_PAGE_FRAME];
extern uint64_t g_next_free_frame_index;
extern void *g_page_frame_start;
extern void *g_physbase;
extern void *g_physfree;


void *allocframe(kpid_t);

uint64_t *newmemtable(kpid_t pid, uint64_t table_size, uint8_t is_self_ref);
cr3e_t newvmem(kpid_t);

void linearmmap(pml4e_t *pml4e_p, kpid_t pid, uint64_t physstart, uint64_t physend, uint64_t offset);

void kmmap(pml4e_t *pml4e_p, kpid_t pid, uint64_t physaddr, uint64_t vaddr);
void mmap(pml4e_t *pml4e_p, kpid_t pid, uint64_t physaddr, uint64_t vaddr);

uint64_t *pe2physaddr(uint64_t pe);
uint64_t physaddr2pebase(uint64_t *physaddr);
uint64_t *getphysaddr(pml4e_t *pml4e_p, void *vaddr);

pml4e_t *getpml4ep();
pdpe_t *getpdpep(uint64_t vaddr);
pde_t *getpdep(uint64_t vaddr);
pte_t *getptep(uint64_t vaddr);


static __inline void load_cr3(cr3e_t cr3)
{
	__asm__ __volatile__(
		"movq %0, %%cr3\n\t"
		: 
		: "r" (cr3));
}

#endif
