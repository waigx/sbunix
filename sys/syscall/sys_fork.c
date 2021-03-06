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


#include <sys/sched.h>
#include <sys/managemem.h>
#include <sys/register.h>
#include <sys/debug.h>
#include <sys/mem.h>
#include <sys/sbunix.h>
#include <sys/register.h>


uint64_t _clean_bit(uint64_t entry, uint64_t bit)
{
	uint64_t util_bits = -1;
	util_bits -= bit;
	return entry & util_bits;
}


cr3e_t _copy_mem_table(cr3e_t cr3)
{
	uint64_t frame_index;

	cr3e_t new_cr3;
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
					if ((pte & PTE_PRESENTS) == 0)
						continue;
					pte = _clean_bit(pte, PTE_WRITEABLE);
					*(p_pte_p + pte_offset) = pte;
					*(c_pte_p + pte_offset) = pte;
					frame_index = physaddr2frameindex(pe2physaddr(pte));
					if (frame_index < MAX_PAGE_FRAME)
						g_page_frame_pool[frame_index] += 1;
				}
			}
		}
	}

	new_cr3 = physaddr2pebase(c_pml4e_p);

	return new_cr3;
}


/* temp code */
extern struct tss_t tss_buf[16];
#define RSP_OFFSET        (0x50)

uint64_t
sys_fork()
{
	uint64_t pte_offset = KERNEL_STACK_START << (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE) >> (VADDR_SIGN_EXTEND + VADDR_PML4E + VADDR_PDPE + VADDR_PDE + VADDR_OFFSET);
	pte_t *pte_p = getptep(KERNEL_STACK_START); 
	void *kernelphyaddr = pe2physaddr(*(pte_p + pte_offset));
	freeframe(kernelphyaddr);

	/* If you add/subtract local variable, you should change RSP_OFFSET */
	uint64_t rsp = get_rsp_register();
	rsp = rsp + RSP_OFFSET;

	copymem(g_page_frame_buf, (void *)KERNEL_STACK_START, PAGE_SIZE);
	kpid_t childpid;
	task_t *kernel_task = gettask(KERNEL_PID);
	load_cr3(kernel_task->cr3);

	void *newkernelstackphysaddr = allocframe();

	kpid_t newpid = g_next_task_free_index;
	cr3e_t newcr3 = _copy_mem_table(gp_current_task->cr3);
	kmmap(pe2physaddr(newcr3), (uint64_t)newkernelstackphysaddr, KERNEL_STACK_START, FALSE, TRUE);
	copymem(newkernelstackphysaddr, g_page_frame_buf, PAGE_SIZE);


	copymem((g_task_start + newpid), gp_current_task, sizeof(task_t));
	(g_task_start + newpid)->pid = newpid;
	(g_task_start + newpid)->parent = gp_current_task->pid;
	(g_task_start + newpid)->cr3 = newcr3;
	(g_task_start + newpid)->status = PROCESS_READY;

	load_cr3(gp_current_task->cr3);
	childpid = g_next_task_free_index;
	/*
	 * Allocate new pid
	 */
	while ((g_task_start + g_next_task_free_index)->pid != 0 && (g_task_start + g_next_task_free_index)->status != PROCESS_TERMINATED) {
		g_next_task_free_index += 1;
		if (g_next_task_free_index > g_task_bump)
			g_task_bump++;
		if (g_next_task_free_index >= MAX_PROC_NUM);
		// max process num exceed error here;
	}

	init_task_context((g_task_start + childpid), (uint64_t)*((uint64_t *)rsp), (uint64_t *)rsp);
	(g_task_start + childpid)->context.regs[CONTEXT_RSP_OFFSET] = rsp +8;
	(g_task_start + childpid)->context.regs[CONTEXT_RIP_OFFSET] = (uint64_t)*(uint64_t *)rsp;
	
	(g_task_start + childpid)->context.regs[CONTEXT_CS_OFFSET] = GDT_KERNEL_CODE_SEG;
	(g_task_start + childpid)->context.regs[CONTEXT_DS_OFFSET] = GDT_KERNEL_DATA_SEG;
	(g_task_start + childpid)->context.regs[CONTEXT_ES_OFFSET] = GDT_KERNEL_DATA_SEG;
	(g_task_start + childpid)->context.regs[CONTEXT_FS_OFFSET] = GDT_KERNEL_DATA_SEG;
	(g_task_start + childpid)->context.regs[CONTEXT_GS_OFFSET] = GDT_KERNEL_DATA_SEG;
	(g_task_start + childpid)->context.regs[CONTEXT_SS_OFFSET] = GDT_KERNEL_DATA_SEG;

	(g_task_start + childpid)->tsss = (struct tss_t *)&(tss_buf[childpid]);

	return childpid;
}
