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
#include <sys/debug.h>


task_t *get_next_task(void)
{
	task_t *next_task;
	uint8_t next_task_runable = FALSE;
	while (next_task_runable == FALSE){
		next_task = g_task_start + g_next_task_index;
		if (next_task->status == PROCESS_READY) {
			next_task_runable = TRUE;
		} else {
			next_task_runable = FALSE;
			g_next_task_index++;
		}
		if (g_next_task_index > g_task_bump)
			g_next_task_index = KERNEL_PID + 1;
	}
	g_next_task_index++;

	return next_task;
}
