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

#include <sys/sbunix.h>
#include <sys/managemem.h>
#include <sys/mem.h>
#include <sys/console.h>
#include <sys/debug.h>


uint64_t *pe2physaddr(uint64_t pe)
{
//	uint64_t baseaddr = pe << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_OFFSET) << VADDR_OFFSET;
	uint64_t baseaddr = pe << 12 >> 24 << 12;
	if ( (baseaddr >> (VADDR_PML4E + VADDR_PDPE + VADDR_PDE + VADDR_PTE + VADDR_OFFSET - 1)) & 1 )
		baseaddr |= EXTEND_BIT_1;
	return (uint64_t *)baseaddr;
}


uint64_t physaddr2pebase(uint64_t *physaddr)
{
//	uint64_t pebase = (uint64_t)physaddr << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_OFFSET ) << VADDR_OFFSET;
	uint64_t pebase = (uint64_t)physaddr << 12 >> 24 << 12;
	return pebase;
}


void newvaddr(kpid_t pid, uint64_t vaddr)
{
	uint64_t permission_bits = PTE_PRESENTS | PTE_USER | PTE_WRITEABLE;
	uint64_t *temp_ptr;
	int i;
	pml4e_t pml4e;
	pdpe_t pdpe;
	pde_t pde;
	pte_t pte;
	pml4e_t *pml4e_p = getpml4ep();
	pdpe_t *pdpe_p = getpdpep(vaddr);
	pde_t *pde_p = getpdep(vaddr);
	pte_t *pte_p = getptep(vaddr);

	
	uint64_t pml4e_offset = vaddr << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_PDPE + VADDR_PDE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pdpe_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pde_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pte_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE + VADDR_OFFSET);

	pml4e = *(pml4e_p + pml4e_offset);
	if ((pml4e & PTE_PRESENTS) != PTE_PRESENTS) {
		temp_ptr = allocframe(pid);
		*(pml4e_p + pml4e_offset) = physaddr2pebase(temp_ptr) | permission_bits;
		for (i=0; i<512; i++)
		{
			*(pdpe_p + i) = 0;
		}
	}

	pdpe = *(pdpe_p + pdpe_offset);
	if ((pdpe & PTE_PRESENTS) != PTE_PRESENTS) {
		temp_ptr = allocframe(pid);
		*(pdpe_p + pdpe_offset) = physaddr2pebase(temp_ptr) | permission_bits;
		for (i=0; i<512; i++)
		{
			*(pde_p + i) = 0;
		}
	}

	pde = *(pde_p + pde_offset);
	if ((pde & PTE_PRESENTS) != PTE_PRESENTS) {
		temp_ptr = allocframe(pid);
		*(pde_p + pde_offset) = physaddr2pebase(temp_ptr) | permission_bits;
		for (i=0; i<512; i++)
		{
			*(pte_p + i) = 0;
		}
	}

	pte = *(pte_p + pte_offset);
	if ((pte & PTE_PRESENTS) != PTE_PRESENTS) {
		temp_ptr = allocframe(pid);
		*(pte_p + pte_offset) = physaddr2pebase(temp_ptr) | permission_bits;
		printf("[page phy]: %p\n", temp_ptr);
	}

	return;
}

void mmap(pml4e_t *pml4e_p, kpid_t pid, uint64_t physaddr, uint64_t vaddr)
{
}


void kmmap(pml4e_t *pml4e_p, kpid_t pid, uint64_t physaddr, uint64_t vaddr, uint8_t is_user, uint8_t is_writable)
{
	uint64_t permission_bits = PTE_PRESENTS;

	uint64_t pml4e_offset = vaddr << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_PDPE + VADDR_PDE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pdpe_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pde_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PTE + VADDR_OFFSET);
	uint64_t pte_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE + VADDR_OFFSET);

	pdpe_t *pdpe_p;
	pde_t *pde_p;
	pte_t *pte_p;
	pml4e_t pml4e;
	pdpe_t pdpe;
	pde_t pde;
	pte_t pte;

	if (is_user)
		permission_bits |= PTE_USER | PTE_WRITEABLE;

	pml4e = *(pml4e_p + pml4e_offset);
	if ((pml4e & PTE_PRESENTS) == PTE_PRESENTS) {
		pdpe_p = (pdpe_t *)pe2physaddr(pml4e);
	} else {
		pdpe_p = newmemtable(pid, 1 << VADDR_PDPE, FALSE);
		*(pml4e_p + pml4e_offset) = physaddr2pebase(pdpe_p) | permission_bits;
	}

	pdpe = *(pdpe_p + pdpe_offset);
	if ((pdpe & PTE_PRESENTS) == PTE_PRESENTS) {
		pde_p = (pde_t *)pe2physaddr(pdpe);
	} else {
		pde_p = newmemtable(pid, 1 << VADDR_PDE, FALSE);
		*(pdpe_p + pdpe_offset) = physaddr2pebase(pde_p) | permission_bits;
	}

	pde = *(pde_p + pde_offset);
	if ((pde & PTE_PRESENTS) == PTE_PRESENTS) {
		pte_p = (pte_t *)pe2physaddr(pde);
	} else {
		pte_p = newmemtable(pid, 1 << VADDR_PTE, FALSE);
		*(pde_p + pde_offset) = physaddr2pebase(pte_p) | permission_bits;
	}

	pte = *(pte_p + pte_offset);

	if ((pte & PTE_PRESENTS) == PTE_PRESENTS) {
		debug_print("Mem", "Error, duplicated page table entry mapping.\n");
		debug_pause();
		return;
	} else {
		permission_bits = PTE_PRESENTS;
		if (is_writable)
			permission_bits |= PTE_WRITEABLE;
		if (is_user)
			permission_bits |= PTE_USER;
		*(pte_p + pte_offset) = physaddr2pebase((void *)physaddr) | permission_bits;
	}
	return;
}


void *allocframe(kpid_t pid)
{
	void *physadd = g_page_frame_start;
	physadd += g_next_free_frame_index * (PAGE_SIZE);

//	printf("%d ", g_next_free_frame_index);
	
	g_page_frame_pool[g_next_free_frame_index] = (uint16_t)pid;

	while (g_page_frame_pool[g_next_free_frame_index] != 0) {
		g_next_free_frame_index++;
		if (g_next_free_frame_index > g_frame_bump)
			g_frame_bump++;
		if (g_next_free_frame_index >= MAX_PAGE_FRAME);
			/* Place information here*/
	}
	return physadd;
}


uint64_t *newmemtable(kpid_t pid, uint64_t table_size, uint8_t is_self_ref)
{
	int i;
	uint64_t *start_p = (uint64_t *)allocframe(pid);

	for (i = 0; i < table_size; i++)
		*(start_p + i) = 0;

	if (is_self_ref)
		*(start_p + 510) = physaddr2pebase(start_p) | PTE_PRESENTS | PTE_USER | PTE_WRITEABLE;

	return start_p;
}


cr3e_t newvmem(kpid_t pid)
{
	/*Begin to create a new 4 level table */
	uint64_t i;
	uint64_t page_frame_start = (uint64_t)g_page_frame_start;
	pml4e_t *pml4e_p = (pml4e_t *)newmemtable(pid, 1 << VADDR_PML4E, TRUE);

	for (i = (uint64_t)0; i < page_frame_start; i += (PAGE_SIZE))
		kmmap(pml4e_p, pid, i, i + KERNEL_SPACE_START, FALSE, TRUE);

	for (i = (uint64_t)0; i < (uint64_t)g_physbase; i += (PAGE_SIZE))
		kmmap(pml4e_p, pid, i, i, FALSE, TRUE);
	
	return (cr3e_t)physaddr2pebase(pml4e_p);
}


void freevmem(kpid_t pid)
{
	uint64_t i = g_frame_bump;
	uint8_t is_first_time = TRUE;

	while (i != 0) {
//		debug_print("FreMem", "Examing:%d\n", i);

		if (g_page_frame_pool[i] == pid) {
//			debug_print("FreMem", "Hit:%d\n", i);
			g_page_frame_pool[i] = 0;
			g_next_free_frame_index = i;
			if (is_first_time)
				g_frame_bump -= 1;
		} else {
			is_first_time = FALSE;
		}

		i -= 1;
	}

	return;
}





pml4e_t *getpml4ep()
{
	return (pml4e_t *)PAGE_SELF_REF;
} 


pdpe_t *getpdpep(uint64_t vaddr)
{
	return (pdpe_t *)((vaddr << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_PDPE + VADDR_PDE + VADDR_PTE + VADDR_OFFSET) << VADDR_OFFSET) | (PAGE_SELF_REF >> (VADDR_OFFSET + VADDR_PTE) << (VADDR_OFFSET + VADDR_PTE)));
}


pde_t *getpdep(uint64_t vaddr)
{
	return (pde_t *)((vaddr << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_PDE + VADDR_PTE + VADDR_OFFSET) << VADDR_OFFSET) | (PAGE_SELF_REF >> (VADDR_OFFSET + VADDR_PTE + VADDR_PDE) << (VADDR_OFFSET + VADDR_PTE + VADDR_PDE)));
}


pte_t *getptep(uint64_t vaddr)
{
	return (pte_t *)((vaddr << VADDR_SIGN_EXTEND >> (VADDR_SIGN_EXTEND + VADDR_PTE + VADDR_OFFSET) << VADDR_OFFSET) | (PAGE_SELF_REF >> (VADDR_OFFSET + VADDR_PTE + VADDR_PDE + VADDR_PDPE) << (VADDR_OFFSET + VADDR_PTE + VADDR_PDE + VADDR_PDPE)));
}
