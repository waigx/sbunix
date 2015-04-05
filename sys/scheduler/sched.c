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
#include <sys/sbunix.h>

void task_1(void);
void task_2(void);


uint64_t temp_stack[4096 * 4];
uint64_t temp_stack2[4096 * 4];


void round_robin_scheduler(void)
{
	struct task_t *task1;
	struct task_t *task2;

	task1 = create_task((uint64_t)task_1, NULL, (uint64_t *)temp_stack/*0x81000000*/ , KERNEL_PROCESS);
	task2 = create_task((uint64_t)task_2, NULL, (uint64_t *)temp_stack2/*0x82000000*/ , KERNEL_PROCESS);
	
	add_task_ready_list(task1);
	add_task_ready_list(task2);

	sys_yield();
}

void task_1(void)
{
	uint64_t i = 0;
	uint64_t j = 1000;
	while(1)
	{
		printf("I am task_1, count = %x\n",i++);
		while(j--);
		j = 1000;
		if(i % 10 == 0)
			sys_yield();
		
	}
}

void task_2(void)
{
        uint64_t a = 0;
        uint64_t b = 100;
        while(1)
        {
                printf("I am task_2, count = %x\n",a++);
                while(b--);
                b = 100;
		if(a %10 == 0)
			sys_yield();
        }
}
