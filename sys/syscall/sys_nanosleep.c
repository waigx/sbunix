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
#include <sys/timer.h>
#include <sys/rtc.h>
#include <sys/debug.h>


int
sys_nanosleep(const struct timespec *req, struct timespec *rem)
{
	volatile uint64_t tv_sec_offset;
	volatile uint64_t tv_nsec_offset;
	volatile uint32_t timecount_snapshot = g_timer_count;

	gp_current_task->status = PROCESS_SLEEPING;
	rem->tv_sec = req->tv_sec; 
	rem->tv_nsec = req->tv_nsec; 

	while (1){
		__asm volatile("sti");
		tv_sec_offset = (g_timer_count - timecount_snapshot)/TIME_FREQ;
		tv_nsec_offset = ((1000000000/TIME_FREQ) * (g_timer_count - timecount_snapshot))%1000000000;
		if ((rem->tv_nsec - tv_nsec_offset) < 0) {
			rem->tv_nsec -= tv_nsec_offset + 1000000000;
			rem->tv_sec -= 1;
		}
		rem->tv_sec -= tv_sec_offset;
		if (rem->tv_sec <= 0) {
			break;
		}
		if (timecount_snapshot != g_timer_count)
			timecount_snapshot = g_timer_count;
	}

	gp_current_task->status = PROCESS_READY;
	return 0;
}
