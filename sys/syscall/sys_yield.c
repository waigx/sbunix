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
#include <sys/debug.h>
#include <sys/sbunix.h>
#include <sys/register.h>


void
sys_yield(void)
{
	task_t *next_task;
	task_t *current_task;
	// TODO how to get current task pointer and next task point
	next_task = get_next_task();
	current_task = gp_current_task;
//	debug_print("yield", "Current task pid: %d\n", current_task->pid);
//	debug_print("yield", "Next task pid: %d\n", next_task->pid);
	// switch context
	if (current_task->status != PROCESS_TERMINATED)
		current_task->status = PROCESS_READY;
	next_task->status = PROCESS_RUNNING;

	gp_current_task = next_task;
	//if(current_task != NULL)

	//load_cr3(cr3e_t cr3)
	load_cr3(next_task->cr3);
	if(current_task->pid == 1) {
		switch_context((struct regs_struct*)NULL, (struct regs_struct*)next_task->context.regs); 
	} else {
		switch_context((struct regs_struct*)current_task->context.regs, (struct regs_struct*)next_task->context.regs);
	}
}

