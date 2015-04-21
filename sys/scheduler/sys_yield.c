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


task_t *get_next_task(void)
{
	task_t *next_task;
	uint8_t next_task_runable = (FALSE);
	while (next_task_runable == FALSE){
		next_task = g_task_start + g_next_task_index;
		if ((next_task->pid > 1) && (next_task->status == PROCESS_READY)) {
			next_task_runable = TRUE;
		} else {
			next_task_runable = FALSE;
			g_task_start++;
		}
		if (g_next_task_index == g_task_bump)
			g_next_task_index = 1;
	}
	g_next_task_index++;

	return next_task;
}

void
sys_yield(void)
{
	task_t *next_task;
	task_t *current_task;
	// TODO how to get current task pointer and next task point
	next_task = get_next_task();
	current_task = gp_current_task;

	// switch context
	current_task->status = PROCESS_READY;
	next_task->status = PROCESS_RUNNING;

	gp_current_task = next_task;
	//if(current_task != NULL)
	//add_task_ready_list(next_task);


	//load_cr3(cr3e_t cr3)
	load_cr3(next_task->cr3);

	if(current_task == NULL)
	{
		switch_context((struct regs_struct*)NULL, (struct regs_struct*)next_task->context.regs); 
	}
	else
		switch_context((struct regs_struct*)current_task->context.regs, (struct regs_struct*)next_task->context.regs);
}

