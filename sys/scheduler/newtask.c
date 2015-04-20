/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
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
#include <sys/mem.h>
#include <sys/managemem.h>
#include <string.h>

void init_task(task_t *task, uint64_t entry_point, uint64_t *stack_base);
void init_task_context(task_t *task, uint64_t entry_point, uint64_t *stack_base);
struct task_t* alloc_task(uint64_t pid);
uint64_t get_newpid(void);



task_t *newtask(const char *task_name, process_type_t type)
{
	uint64_t entry_point;
	uint64_t *phy_stack_base;
	task_t *task = g_task_start + g_next_task_free_index;
	kpid_t new_pid = g_next_task_free_index;
	cr3e_t new_cr3;

	/*
	 * Allocate new pid
	 */
	while ((g_task_start + g_next_task_free_index)->pid != 0) {
		g_next_task_free_index += 1;
		if (g_next_task_free_index >= MAX_PROC_NUM);
		// max process num exceed error here;
	}

	new_cr3 = newvmem(new_pid);

	task->cr3 = new_cr3;
	task->pid = new_pid;
	task->type = type;
	task->status = PROCESS_NEW;
	strcpy(task->name, task_name);
	phy_stack_base = allocframe(new_pid);
	kmmap(pe2physaddr(new_cr3), new_pid, (uint64_t)phy_stack_base, USER_STACK_START);

	if(type == KERNEL_PROCESS) {
//		init_task(task, instruction_addr, virtual_memory_addr);
//		page = (uint64_t)allocframe(pid);
	} else if (type == USER_PROCESS) {
		entry_point = load_elf(task, task_name);
		init_task(task, entry_point, phy_stack_base);
	}

	task->status = PROCESS_NEW;

	// add task list

	return task;
}


void init_task(task_t *task, uint64_t entry_point, uint64_t *stack_base)
{
	task->stack_base = stack_base;
	init_task_context(task, entry_point, stack_base);
}


void init_task_context(task_t *task, uint64_t entry_point, uint64_t *stack_base )
{
	//setmem(task->context, (uint64_t*)((uint64_t)task->context + sizeof(struct regs_struct)), 0x00);
	setmem((uint64_t *)task->context.regs, (uint64_t *)task->context.regs + sizeof(struct regs_struct), 0x00);

	// TODO stack_base will be allocated size or memory menagement by dongju
	task->context.regs[CONTEXT_RSP_OFFSET] = (uint64_t)stack_base + 4096 -8;
	task->context.regs[CONTEXT_RBP_OFFSET] = (uint64_t)stack_base + 4096 -8;

	task->context.regs[CONTEXT_CS_OFFSET] = GDT_KERNEL_CODE_SEG;
	task->context.regs[CONTEXT_DS_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_ES_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_FS_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_GS_OFFSET] = GDT_KERNEL_DATA_SEG;
	task->context.regs[CONTEXT_SS_OFFSET] = GDT_KERNEL_DATA_SEG;
	
	task->context.regs[CONTEXT_RIP_OFFSET] = entry_point;
	// TODO needed to enable interrupt by dongju
	//task->context.regs[CONTEXT_RFLAG_OFFSET] |= (0x1 << 9);	// IF(bit 9) enable	
}

void
sys_yield(void)
{
	struct task_t *next_task;
	struct task_t *current_task;
	// TODO how to get current task pointer and next task point
	next_task = (task_t*)get_next_task();
	current_task = gp_current_task;

	// switch context
	gp_current_task = next_task;
	//if(current_task != NULL)
	add_task_ready_list(next_task);


	//load_cr3(cr3e_t cr3)
	load_cr3((cr3e_t)next_task->cr3);

	if(current_task == NULL)
	{
		switch_context((struct regs_struct*)NULL, (struct regs_struct*)next_task->context.regs); 
	}
	else
		switch_context((struct regs_struct*)current_task->context.regs, (struct regs_struct*)next_task->context.regs);
}

