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
#include <sys/debug.h>

#include <sys/managemem.h>
#include <syscall.h>

#include <stdlib.h>
#include <sys/tarfs_api.h>

void task_1(void);
void task_2(void);
void yield(uint64_t user);

uint64_t temp_stack[4096 * 4];
uint64_t temp_stack2[4096 * 4];


void round_robin_scheduler(void)
{
	task_t *task1;
	task_t *task2;

//	task1 = create_task((uint64_t)task_1,"bin/user_1", 
//			(uint64_t *)/*temp_stack*/0xffffffff80300000 , USER_PROCESS);
//	task2 = create_task((uint64_t)task_2,"bin/user_2", 
//			(uint64_t *)/*temp_stack2*/0xffffffff80302000 , USER_PROCESS);
	task1 = newtask("bin/user_1", USER_PROCESS);
	task2 = newtask("bin/user_2", USER_PROCESS);


	//printf("opne_tarfs output = %x\n", open_tarfs("bin/user_1", 0));


	//while(1);
	//
	//kmmap((pml4e_t*)task1->pml4e, task1->pid, ((uint64_t)task2>>12)<<12, (uint64_t)((uint64_t)task2>>12)<<12);
	  //kmmap((pml4e_t*)task2->pml4e, task2->pid, ((uint64_t)task1>>12)<<12, (uint64_t)((uint64_t)task1>>12)<<12);

//	add_task_ready_list(task1);
//	add_task_ready_list(task2);
	task1->status = PROCESS_READY;
	task2->status = PROCESS_READY;
	//close(1);
	//sys_yield();
	yield(0);
}

void yield(uint64_t user)
{
	printf("yield called by %x\n",user);
    syscall_4(SYS_yield, 1, 2, 3, 4);
}



uint64_t global_buf_test[10];
void task_1(void)
{
	uint64_t i = 0;
	uint64_t j = 10000;
	while(1)
	{
		printf("I am task_1, count = %x\n",i++);
		global_buf_test[0] += 1;
		printf("global_buf_test = %x\n",global_buf_test[0]); 
		while(j--);
		j = 10000000;
		//if(i % 5 == 0)
		//	yield();
		
	}
}

void task_2(void)
{
        uint64_t a = 0;
        uint64_t b = 10000;
        while(1)
        {
                printf("I am task_2, count = %x\n",a++);
                global_buf_test[0] += 2;
		printf("global_buf_test = %x\n",global_buf_test[0]);
		while(b--);
                b = 10000000;
		//if(a %5 == 0)
		//	yield();
        }
}
