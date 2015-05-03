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


#include <sys/sched/sched.h>
#include <sys/managemem.h>
#include <sys/register.h>
#include <sys/debug.h>
#include <sys/sbunix.h>



uint64_t _clean_bit(uint64_t entry, uint64_t bit)
{
	uint64_t util_bits = -1;
	util_bits -= bit;
	return entry & util_bits;
}


cr3e_t _copy_mem_table(cr3e_t cr3)
{
	pml4e_t *p_pml4e_p = pe2physaddr(cr3);
	pdpe_t *p_pdpe_p;
	pde_t *p_pde_p;
	pte_t *p_pte_p;

	uint64_t pml4e_offset;
	uint64_t pdpe_offset;
	uint64_t pde_offset;
	uint64_t pte_offset;

	pml4e_t *c_pml4e_p = newmemtable(1 << VADDR_PML4E, TRUE);
	pdpe_t *c_pdpe_p;
	pde_t *c_pde_p;
	pte_t *c_pte_p;
	pml4e_t pml4e;
	pdpe_t pdpe;
	pde_t pde;
	pte_t pte;


	for (pml4e_offset = 0; pml4e_offset < (1 << VADDR_PDPE); pml4e_offset++) {
		if (pml4e_offset == PAGE_SELF_REF_PML4E_INDEX)
			continue;
		pml4e = *(p_pml4e_p + pml4e_offset);
		if (pml4e == 0)
			continue;
		p_pdpe_p = pe2physaddr(pml4e);
		c_pdpe_p = newmemtable(1 << VADDR_PDPE, FALSE);
		pml4e = (pml4e & PERMITE_BITS) | physaddr2pebase(c_pdpe_p);
		*(c_pml4e_p + pml4e_offset) = pml4e;

		for (pdpe_offset = 0;  pdpe_offset < (1 << VADDR_PDPE); pdpe_offset++) {
			pdpe = *(p_pdpe_p + pdpe_offset);
			if ( pdpe == 0)
				continue;
			p_pde_p = pe2physaddr(pdpe);
			c_pde_p = newmemtable(1 << VADDR_PDE, FALSE);
			pdpe = (pdpe & PERMITE_BITS) | physaddr2pebase(c_pde_p);
			*(c_pdpe_p + pdpe_offset) = pdpe;

			for (pde_offset = 0; pde_offset < (1 << VADDR_PDE); pde_offset++) {
				pde = *(p_pde_p + pde_offset);
				if ( pde == 0)
					continue;
				p_pte_p = pe2physaddr(pde);
				c_pte_p = newmemtable(1 << VADDR_PTE, FALSE);
				pde = (pde & PERMITE_BITS) | physaddr2pebase(c_pte_p);
				*(c_pde_p + pde_offset) = pde;

				for (pte_offset = 0;  pte_offset < (1 << VADDR_PTE); pte_offset++) {

					pte = *(p_pte_p + pte_offset);
					if ((pte == 0) || (pte && PTE_PRESENTS))
						continue;
					pte = _clean_bit(pte, PTE_WRITEABLE);
					*(p_pte_p + pte_offset) = pte;
					*(c_pte_p + pte_offset) = pte;
				}
			}
		}
	}


	/*
	 * Allocate new pid
	 */
	while ((g_task_start + g_next_task_free_index)->pid != 0) {
		g_next_task_free_index += 1;
		if (g_next_task_free_index > g_task_bump)
			g_task_bump++;
		if (g_next_task_free_index >= MAX_PROC_NUM);
		// max process num exceed error here;
	}


	/*TEMP*/return 0;
}


uint64_t
sys_fork()
{
	task_t *kernel_task = gettask(KERNEL_PID);
	load_cr3(kernel_task->cr3);


	load_cr3(gp_current_task->cr3);
	/*TEMP*/return 0;
}
