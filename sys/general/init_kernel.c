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


#include <sys/general.h>
#include <sys/managemem.h>
#include <sys/kio.h>
#include <sys/sbunix.h>
#include <sys/debug.h>


void _init_mem_pools()
{
	return;
}


void _kmmap(pml4e_t *pml4e_p, kpid_t pid, uint64_t physaddr, uint64_t vaddr)
{
	pdpe_t *pdpe_p;
	pde_t *pde_p;
	pte_t *pte_p;

	uint64_t pml4e_offset = vaddr << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_PDPE + VADDR_PDE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pdpe_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pde_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pte_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE + VADDR_OFFSET);

	pml4e_t pml4e;
	pdpe_t pdpe;
	pde_t pde;
	pte_t pte;


	pml4e = *(pml4e_p + pml4e_offset);
	if ((pml4e & 1) == PTE_PRESENTS) {
		pdpe_p = (pdpe_t *)pe2physaddr(pml4e);
	} else {
		pdpe_p = newmemtable(pid, 1 << VADDR_PDPE, FALSE);
		*(pml4e_p + pml4e_offset) = physaddr2pebase(pdpe_p) | PTE_PRESENTS | PTE_SUPER | PTE_WRITEABLE;
	}

	pdpe = *(pdpe_p + pdpe_offset);
	if ((pdpe & 1) == PTE_PRESENTS) {
		pde_p = (pde_t *)pe2physaddr(pdpe);
	} else {
		pde_p = newmemtable(pid, 1 << VADDR_PDE, FALSE);
		*(pdpe_p + pdpe_offset) = physaddr2pebase(pde_p) | PTE_PRESENTS | PTE_SUPER | PTE_WRITEABLE;
	}

	pde = *(pde_p + pde_offset);
	if ((pde & 1) == PTE_PRESENTS) {
		pte_p = (pte_t *)pe2physaddr(pde);
	} else {
		pte_p = newmemtable(pid, 1 << VADDR_PTE, FALSE);
		*(pde_p + pde_offset) = physaddr2pebase(pte_p) | PTE_PRESENTS | PTE_SUPER | PTE_WRITEABLE;
	}

	pte = *(pte_p + pte_offset);
	if ((pte & 1) == PTE_PRESENTS) {
		return;
	} else {
		*(pte_p + pte_offset) = physaddr2pebase((void *)physaddr) | PTE_PRESENTS | PTE_WRITEABLE;
	}
	return;
}


cr3e_t _init_kernel_mem(kpid_t pid, uint64_t physbase, uint64_t physfree, uint64_t physbottom, uint64_t phystop)
{
	uint64_t i;
	uint64_t page_frame_start = (uint64_t)g_page_frame_start;
	pml4e_t *pml4e_p = (pml4e_t *)newmemtable(pid, 1 << VADDR_PML4E, TRUE);

	for (i = (uint64_t)0; i <= phystop; i+= (PAGE_SIZE))
		_kmmap(pml4e_p, pid, i, i);

	for (i = (uint64_t)0; i < page_frame_start; i += (PAGE_SIZE))
		_kmmap(pml4e_p, pid, i, i + KERNEL_SPACE_START);

	_kmmap(pml4e_p, pid, CONSOLE_START, CONSOLE_START);
	
	printf("[Kernel Init Mem]: Kernel memory tables take %d page frames.\n", g_next_free_frame_index);

	return (cr3e_t)physaddr2pebase(pml4e_p);
}


void _init_kernel_process(void *physbase, void *physfree, void *physbottom, void *phystop)
{
	proc_ent *kproc = g_proc_ent_start + g_next_proc_free_index;
	kpid_t kpid = g_next_proc_free_index;
	cr3e_t kcr3;

	kcr3 = _init_kernel_mem(kpid, (uint64_t)physbase, (uint64_t)physfree, (uint64_t)physbottom, (uint64_t)phystop);

	kproc->cr3 = kcr3;
	kproc->pid = kpid;

	g_next_proc_free_index += 1;
	load_cr3(kcr3);
	return;
}


void init_kernel(void *physbase, void *physfree, void *physbottom, void *phystop)
{

	_init_mem_pools();

	g_physfree = physfree;
	g_physbase = physbase;
	g_page_frame_start = physfree + KERNEL_PROC_HEAP_SIZE + PAGE_SIZE;
	// align page table to PAGE_SIZE
	g_page_frame_start = (void *)(((uint64_t)g_page_frame_start) >> PAGE_SIZE_LOG2 << PAGE_SIZE_LOG2);
	g_proc_ent_start = (proc_ent *)g_physfree;

	_init_kernel_process(physbase, physfree, physbottom, phystop);
	return;
}
