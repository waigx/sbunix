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


#include <sys/defs.h>
#include <sys/sched.h>
#include <sys/managemem.h>
#include <sys/debug.h>

kpid_t
sys_wait4(int pid, int *status, int options, struct rusage *rusage)
{
	task_t *temptask_ptr;
	uint8_t end_sleeping;

	gp_current_task->status = PROCESS_SLEEPING;
	end_sleeping = FALSE;

	__asm volatile("sti");

	if (pid > KERNEL_PID){
		temptask_ptr = g_task_start + pid;
		while(end_sleeping == FALSE){
			if (temptask_ptr->status == PROCESS_TERMINATED)
				end_sleeping = TRUE;
		}
	} else {
		while(end_sleeping == FALSE){
			end_sleeping = TRUE;
			for (temptask_ptr = g_task_start + KERNEL_PID + 1; temptask_ptr <= g_task_start + g_task_bump; temptask_ptr++){
				if (temptask_ptr->parent == gp_current_task->pid && temptask_ptr->status != PROCESS_TERMINATED)
					end_sleeping = FALSE;
			}
		}
	}

	gp_current_task->status = PROCESS_READY;
	*status = gp_current_task->child_status;

	return gp_current_task->pid;
}
