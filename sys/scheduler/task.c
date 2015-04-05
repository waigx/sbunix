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
#include <sys/sched/list.h>
#include <sys/mem.h>

void init_task(struct task_t *task, uint64_t entry_point, uint64_t *stack_base);
void init_task_context(struct task_t *task, uint64_t entry_point, uint64_t *stack_base);
struct	task_t*	alloc_task(void);
uint64_t get_newpid(void);


static uint64_t	next_pid = 0;
static struct task_t *gp_current_task;
uint8_t task_buf[5][4096];
struct list_manager_struct g_ready_task_list;


struct task_t*
create_task(uint64_t instruction_addr, 
		uint8_t *binary , 
		void* virtual_memory_addr, 
		enum process_type type )
{
	struct	task_t	*task;
	uint64_t	pid;
	// allocate new task
	task = alloc_task();

	// get new pid
	pid = get_newpid();
	task->pid = pid;

	//TODO using kmalloc
	//task->name = binary;

	if(type == KERNEL_PROCESS)
	{
		task->type = KERNEL_PROCESS;
		init_task(task, instruction_addr, virtual_memory_addr);

	}
	else if(type == USER_PROCESS)
	{
		//set_cr3();
		//task->stack_base = virtual_memory_addr;
	}

	// setting pml4e map 

	// allocate_virtual_addr()
	//task->stack_base = virtual_memory_addr;

	task->status = PROCESS_RUNNABLE;
	//task->process_type = USER_PROCESS;

	// TODO. dealing with elf format

	// add task list
	return (struct task_t*)task;
}


struct task_t*
get_next_task(void)
{
	struct task_t *new_task = NULL;
	//uint64_t i = 0;
	new_task = (struct task_t*)remove_list_from_header(&g_ready_task_list);
	return (struct task_t*)new_task;
}

void add_task_ready_list(struct task_t *task)
{
	add_list_to_tail(&g_ready_task_list, task);

}


void init_task(struct task_t *task, uint64_t entry_point, uint64_t *stack_base)
{

	task->stack_base = stack_base;
	init_task_context(task, entry_point, stack_base);


}

void init_task_context(struct task_t *task, uint64_t entry_point, uint64_t *stack_base )
{
	//setmem(task->context, (uint64_t*)((uint64_t)task->context + sizeof(struct regs_struct)), 0x00);
	setmem((uint64_t *)task->context.regs, (uint64_t *)task->context.regs, 0x00);

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
	next_task = (struct task_t*)get_next_task();
	current_task = gp_current_task;

	// switch context
	gp_current_task = next_task;
	//if(current_task != NULL)
	add_task_ready_list(next_task);

	if(current_task == NULL)
	{
		switch_context((struct regs_struct*)NULL, (struct regs_struct*)next_task->context.regs); 
	}
	else
		switch_context((struct regs_struct*)current_task->context.regs, (struct regs_struct*)next_task->context.regs);
}

void
free_task()
{
	;

}

static uint64_t task_num = 0;

struct task_t*
alloc_task(void)
{
	uint64_t	addr;
	// addr = kmalloc
	//addr = allocmem();

	addr = (uint64_t)task_buf[task_num];
	task_num ++; 

	return (struct task_t*)addr;
}

void
add_task_list(void)
{
	;
}

void
free_task_list(void)
{
	;
}


uint64_t get_newpid(void)
{
	//TODO how to deal with pid=0 because pid=0 means child process.
	next_pid += 1;
	return next_pid;
}
