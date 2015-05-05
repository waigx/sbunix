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
#include <string.h>
#include <sys/managemem.h>
#include <sys/mem.h>
#include <sys/console.h>
#include <sys/debug.h>


/* PART 1: These functions do not care current cr3*/

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


void cow_pagefault_handler(uint64_t vaddr)
{
	uint64_t pte_offset = vaddr << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE + VADDR_OFFSET);
	uint64_t *pte_p = getptep(vaddr) + pte_offset;
	void *new_frame_p;
	pte_t pte = *pte_p;

	if (g_page_frame_pool[physaddr2frameindex(pe2physaddr(pte))] == 1) {
		*pte_p = pte | PTE_WRITEABLE;
		return;
	}

	copymem(g_page_frame_buf, (void *)(vaddr >> VADDR_OFFSET << VADDR_OFFSET), PAGE_SIZE);
	new_frame_p = allocframe();
	*pte_p = (pte & PERMITE_BITS) | physaddr2pebase(new_frame_p) | PTE_WRITEABLE;
	copymem((void *)(vaddr >> VADDR_OFFSET << VADDR_OFFSET), g_page_frame_buf, PAGE_SIZE);
	g_page_frame_pool[physaddr2frameindex(pe2physaddr(pte))] -= 1;

	return;
}


void freevmem(kpid_t pid)
{
	pml4e_t *pml4e_p = getpml4ep();
	pdpe_t *pdpe_p;
	pde_t *pde_p;
	pte_t *pte_p;

	void *page_frame;

	uint64_t pml4e_offset;
	uint64_t pdpe_offset;
	uint64_t pde_offset;
	uint64_t pte_offset;

	uint64_t self_ref_temp_addr;

	pml4e_t pml4e;
	pdpe_t pdpe;
	pde_t pde;
	pte_t pte;

	for (pml4e_offset = 0; pml4e_offset < (1 << VADDR_PDPE); pml4e_offset++) {
		if (pml4e_offset == PAGE_SELF_REF_PML4E_INDEX)
			continue;
		pml4e = *(pml4e_p + pml4e_offset);
		if (pml4e == 0)
			continue;
		self_ref_temp_addr = pml4e_offset << (VADDR_OFFSET + VADDR_PTE + VADDR_PDE + VADDR_PDPE);
		pdpe_p = getpdpep(self_ref_temp_addr);

		for (pdpe_offset = 0;  pdpe_offset < (1 << VADDR_PDPE); pdpe_offset++) {
			pdpe = *(pdpe_p + pdpe_offset);
			if ( pdpe == 0)
				continue;
			self_ref_temp_addr = ((pml4e_offset << VADDR_PDPE) + pdpe_offset) << (VADDR_OFFSET + VADDR_PTE + VADDR_PDE);
			pde_p = getpdep(self_ref_temp_addr);

			for (pde_offset = 0; pde_offset < (1 << VADDR_PDE); pde_offset++) {
				pde = *(pde_p + pde_offset);
				if ( pde == 0)
					continue;
				self_ref_temp_addr = ((((pml4e_offset << VADDR_PDPE) + pdpe_offset) << VADDR_PDE) + pde_offset) << (VADDR_OFFSET + VADDR_PTE);
				pte_p = getptep(self_ref_temp_addr);

				for (pte_offset = 0;  pte_offset < (1 << VADDR_PTE); pte_offset++) {
					pte = *(pte_p + pte_offset);
					if ((pte & PTE_PRESENTS) == 0)
						continue;
					page_frame = pe2physaddr(pte);
					freeframe(page_frame);
				}
				freeframe(pe2physaddr(pde));
			}
			freeframe(pe2physaddr(pdpe));
		}
		freeframe(pe2physaddr(pml4e));
	}

	for (g_next_free_frame_index = 0; g_next_free_frame_index <= g_frame_bump + 1; g_next_free_frame_index ++) {
		if (g_page_frame_pool[g_next_free_frame_index] == 0)
			break;
	}

	return;
}


uint64_t physaddr2frameindex(void *physaddr)
{
	uint64_t index;
	index = ((uint64_t)(physaddr - g_page_frame_start))/(PAGE_SIZE);
	return index;
}


void freeframe(void *physaddr)
{
	uint64_t index;
	index = physaddr2frameindex(physaddr);
	if (index < MAX_PAGE_FRAME)
		g_page_frame_pool[index] -= 1;

	return;
}


void *allocframe()
{
	void *physadd = g_page_frame_start;
	physadd += g_next_free_frame_index * (PAGE_SIZE);

	
	g_page_frame_pool[g_next_free_frame_index] += 1;

	while (g_page_frame_pool[g_next_free_frame_index] != 0) {
		g_next_free_frame_index++;
		if (g_next_free_frame_index > g_frame_bump)
			g_frame_bump++;
		if (g_next_free_frame_index >= MAX_PAGE_FRAME);
			/* Place information here*/
	}
	return physadd;
}
/* The end of PART 1*/


/* PART 2: These functions should be called under kernel cr3*/
void mmap(pml4e_t *pml4e_p, uint64_t physaddr, uint64_t vaddr)
{
}


void kmmap(pml4e_t *pml4e_p, uint64_t physaddr, uint64_t vaddr, uint8_t is_user, uint8_t is_writable)
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
		pdpe_p = newmemtable(1 << VADDR_PDPE, FALSE);
		*(pml4e_p + pml4e_offset) = physaddr2pebase(pdpe_p) | permission_bits;
	}

	pdpe = *(pdpe_p + pdpe_offset);
	if ((pdpe & PTE_PRESENTS) == PTE_PRESENTS) {
		pde_p = (pde_t *)pe2physaddr(pdpe);
	} else {
		pde_p = newmemtable(1 << VADDR_PDE, FALSE);
		*(pdpe_p + pdpe_offset) = physaddr2pebase(pde_p) | permission_bits;
	}

	pde = *(pde_p + pde_offset);
	if ((pde & PTE_PRESENTS) == PTE_PRESENTS) {
		pte_p = (pte_t *)pe2physaddr(pde);
	} else {
		pte_p = newmemtable(1 << VADDR_PTE, FALSE);
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


uint64_t *newmemtable(uint64_t table_size, uint8_t is_self_ref)
{
	int i;
	uint64_t *start_p = (uint64_t *)allocframe();

	for (i = 0; i < table_size; i++)
		*(start_p + i) = 0;

	if (is_self_ref)
		*(start_p + PAGE_SELF_REF_PML4E_INDEX) = physaddr2pebase(start_p) | PTE_PRESENTS | PTE_USER | PTE_WRITEABLE;

	return start_p;
}


cr3e_t newvmem()
{
	/*Begin to create a new 4 level table */
	uint64_t i;
	uint64_t page_frame_start = (uint64_t)g_page_frame_start;
	pml4e_t *pml4e_p = (pml4e_t *)newmemtable(1 << VADDR_PML4E, TRUE);

	for (i = (uint64_t)0; i < page_frame_start; i += (PAGE_SIZE))
		kmmap(pml4e_p, i, i + KERNEL_SPACE_START, FALSE, TRUE);

	for (i = (uint64_t)0; i < (uint64_t)g_physbase; i += (PAGE_SIZE))
		kmmap(pml4e_p, i, i, FALSE, TRUE);
	
	return (cr3e_t)physaddr2pebase(pml4e_p);
}
/* The end of PART 2 */


/* PART 3: These functions should be called under user cr3*/

void removevma(vma_t *vma)
{
	long long shift_bytes;
	shift_bytes = sizeof(vma_t);

	shiftmem(vma, g_vma_end, shift_bytes);
	return;
}


void insertvma(vma_t vma)
{
	void *newvma_start = vma.vaddr_start;
	void *newvma_end = vma.vaddr_end;
	uint64_t newvma_permission = vma.permission;
	long long shift_bytes;
	vma_t *currentvma = g_vma_start;
	vma_t *tempvma;

	while ( currentvma->vaddr_end != NULL ) {
		if (newvma_start < currentvma->vaddr_end) {
			if (newvma_start < currentvma->vaddr_start) {
				tempvma = currentvma;
				while (tempvma->vaddr_end != NULL){
					if (newvma_end < tempvma->vaddr_end){
						if (newvma_end < tempvma->vaddr_start) {
							/*
							 * Case 0
							 *    New vma --------
							 *    Old vma  ---     ---
							 */
							shift_bytes = tempvma - currentvma - 1;
							shift_bytes *= sizeof(vma_t);
							shiftmem(currentvma, g_vma_end, shift_bytes);
							copymem(currentvma, &vma, sizeof(vma_t));
							return;
						} else {
							/*
							 * Case 1
							 *    New vma ----------
							 *    Old vma  ---     ---
							 */
							if ((tempvma->permission == newvma_permission) && (strcmp(tempvma->name, vma.name) == 0)) {
								shift_bytes = tempvma - currentvma;
								shift_bytes *= sizeof(vma_t);
								shiftmem(currentvma, g_vma_end, shift_bytes);
								currentvma->vaddr_start = newvma_start;
							} else {
								shift_bytes = tempvma - currentvma - 1;
								shift_bytes *= sizeof(vma_t);
								shiftmem(currentvma, g_vma_end, shift_bytes);
								(currentvma + 1)->vaddr_start = newvma_end;
								copymem(currentvma, &vma, sizeof(vma_t));
							}
							return;
						}
					}
					tempvma += 1;
				}
				/*
				 * Case 2
				 *    New vma -------------
				 *    Old vma  ---     ---
				 */
				shift_bytes = tempvma - currentvma;
				shift_bytes *= sizeof(vma_t);
				shiftmem(currentvma, g_vma_end, shift_bytes);
				copymem(currentvma, &vma, sizeof(vma_t));
				return;
			} else {
				tempvma = currentvma;
				while (tempvma->vaddr_end != NULL){
					if (newvma_end < tempvma->vaddr_end){
						if (newvma_end < tempvma->vaddr_start) {
							/*
							 * Case 3
							 *    New vma   ------
							 *    Old vma  ---     ---
							 */
							if ((currentvma->permission == newvma_permission) && (strcmp(currentvma->name, vma.name) == 0)) {
								shift_bytes = tempvma - currentvma - 1;
								shift_bytes *= sizeof(vma_t);
								shiftmem(currentvma + 1, g_vma_end, shift_bytes);
								currentvma->vaddr_end = vma.vaddr_end;
							} else {
								shift_bytes = tempvma - currentvma - 2;
								shift_bytes *= sizeof(vma_t);
								shiftmem(currentvma + 1, g_vma_end, shift_bytes);
								copymem(currentvma + 1, &vma, sizeof(vma_t));
								currentvma->vaddr_end = vma.vaddr_start;
							}
							return;
						} else {
							/*
							 * Case 4
							 *    New vma   --------
							 *    Old vma  ---     ---
							 */
								debug_print("vma", "Not supported new vma case.\n");
								debug_pause();
							}
							return;
						}
					}
					tempvma += 1;
				}
				/*
				 * Case 5
				 *    New vma   -----------
				 *    Old vma  ---     ---
				 */
				if ((currentvma->permission == newvma_permission) && (strcmp(currentvma->name, vma.name) == 0)) {
					shift_bytes = tempvma - currentvma;
					shift_bytes *= sizeof(vma_t);
					shiftmem(currentvma + 1, g_vma_end, shift_bytes);
					currentvma->vaddr_end = vma.vaddr_end;
				} else {
					shift_bytes = tempvma - currentvma - 1;
					shift_bytes *= sizeof(vma_t);
					shiftmem(currentvma + 1, g_vma_end, shift_bytes);
					copymem(currentvma + 1, &vma, sizeof(vma_t));
					currentvma->vaddr_end = vma.vaddr_start;
				}
				return;
			}
		currentvma += 1;
	}
	copymem(currentvma, &vma, sizeof(vma_t));
	return;
}


void removeaddrange(void *vaddr_start, void *vaddr_end)
{
	long long shift_bytes;
	vma_t *currentvma = g_vma_start;
	vma_t *tempvma;

	while ( currentvma->vaddr_end != NULL ) {
		if (vaddr_start < currentvma->vaddr_end) {
			if (vaddr_start <= currentvma->vaddr_start) {
				tempvma = currentvma;
				while (tempvma->vaddr_end != NULL){
					if (vaddr_end < tempvma->vaddr_end){
						if (vaddr_end <= tempvma->vaddr_start) {
							/*
							 * Case 0
							 *    New vma --------
							 *    Old vma  ---     ---
							 */
							shift_bytes = tempvma - currentvma;
							shift_bytes *= sizeof(vma_t);
							shiftmem(currentvma, g_vma_end, shift_bytes);
							return;
						} else {
							/*
							 * Case 1
							 *    New vma ----------
							 *    Old vma  ---     ---
							 */
							shift_bytes = tempvma - currentvma;
							shift_bytes *= sizeof(vma_t);
							shiftmem(currentvma, g_vma_end, shift_bytes);
							currentvma->vaddr_start = vaddr_end;
							return;
						}
					}
					tempvma += 1;
				}
				/*
				 * Case 2
				 *    New vma -------------
				 *    Old vma  ---     ---
				 */
				shift_bytes = tempvma - currentvma + 1;
				shift_bytes *= sizeof(vma_t);
				shiftmem(currentvma, g_vma_end, shift_bytes);
				return;
			} else {
				tempvma = currentvma;
				while (tempvma->vaddr_end != NULL){
					if (vaddr_end < tempvma->vaddr_end){
						if (vaddr_end < tempvma->vaddr_start) {
							/*
							 * Case 3
							 *    New vma   ------
							 *    Old vma  ---     ---
							 */
							shift_bytes = tempvma - currentvma - 1;
							shift_bytes *= sizeof(vma_t);
							shiftmem(currentvma + 1, g_vma_end, shift_bytes);
							currentvma->vaddr_end = vaddr_start;
						} else {
							/*
							 * Case 4
							 *    New vma   --------
							 *    Old vma  ---     ---
							 */
							shift_bytes = tempvma - currentvma - 1;
							shift_bytes *= sizeof(vma_t);
							shiftmem(currentvma + 1, g_vma_end, shift_bytes);
							currentvma->vaddr_end = vaddr_start;
							(currentvma + 1)->vaddr_start = vaddr_end;
							}
							return;
						}
					}
					tempvma += 1;
				}
			/*
			 * Case 5
			 *    New vma   -----------
			 *    Old vma  ---     ---
			 */
			shift_bytes = tempvma - currentvma;
			shift_bytes *= sizeof(vma_t);
			shiftmem(currentvma + 1, g_vma_end, shift_bytes);
			currentvma->vaddr_end = vaddr_start;
			return;
			}
		currentvma += 1;
	}
	return;

}


vma_t *lookupvmabyvaddr(void *vaddr)
{
	vma_t *currentvma = g_vma_start;
	while (currentvma->vaddr_end != NULL) {
		if ((currentvma->vaddr_start <= vaddr) && (vaddr < currentvma->vaddr_end))
			return currentvma;
		currentvma += 1;
	}
	return NULL;
}


vma_t *lookupvmabyname(const char *name)
{
	vma_t *currentvma = g_vma_start;
	while ( currentvma->vaddr_end != NULL ) {
		if (strcmp(currentvma->name, name) == 0)
			return currentvma;
		currentvma += 1;
	}
	return NULL;
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

void newvaddr(uint64_t vaddr)
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
		temp_ptr = allocframe();
		*(pml4e_p + pml4e_offset) = physaddr2pebase(temp_ptr) | permission_bits;
		for (i=0; i<512; i++)
			*(pdpe_p + i) = 0;
	} else {
		*(pml4e_p + pml4e_offset) = pml4e | permission_bits;
	}

	pdpe = *(pdpe_p + pdpe_offset);
	if ((pdpe & PTE_PRESENTS) != PTE_PRESENTS) {
		temp_ptr = allocframe();
		*(pdpe_p + pdpe_offset) = physaddr2pebase(temp_ptr) | permission_bits;
		for (i=0; i<512; i++)
			*(pde_p + i) = 0;
	} else {
		*(pdpe_p + pdpe_offset) = pdpe | permission_bits;
	}


	pde = *(pde_p + pde_offset);
	if ((pde & PTE_PRESENTS) != PTE_PRESENTS) {
		temp_ptr = allocframe();
		*(pde_p + pde_offset) = physaddr2pebase(temp_ptr) | permission_bits;
		for (i=0; i<512; i++)
			*(pte_p + i) = 0;
	} else {
		*(pde_p + pde_offset) = pde | permission_bits;
	}
 

	pte = *(pte_p + pte_offset);
	if ((pte & PTE_PRESENTS) != PTE_PRESENTS) {
		temp_ptr = allocframe();
		*(pte_p + pte_offset) = physaddr2pebase(temp_ptr) | permission_bits;
	} else {
		*(pte_p + pte_offset) = pte | permission_bits;
	}
 
	return;
}


/* The end of PART 3 */
