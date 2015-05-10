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
#include <sys/sbunix.h>
#include <sys/debug.h>

#include <sys/managemem.h>


void load_test_tasks(void)
{
	task_t *sbu;

//	task_t *task1;
//	task_t *task2;
//	task_t *task3;
//	task_t *task4;
//	task_t *task5;
//	task_t *task6;
//	task_t *task7;


	sbu = newtask("bin/user_1", USER_PROCESS);
//	task1 = newtask("bin/user_1", USER_PROCESS);
//	task2 = newtask("bin/user_2", USER_PROCESS);
//	task3 = newtask("bin/user_3", USER_PROCESS);
//	task4 = newtask("bin/user_4", USER_PROCESS);
//	task5 = newtask("bin/user_5", USER_PROCESS);
//	task6 = newtask("bin/user_6", USER_PROCESS);
//	task7 = newtask("bin/user_7", USER_PROCESS);


	loadtask(sbu);
//
//	loadtask(task1);
//	loadtask(task2);
//	loadtask(task3);
//	loadtask(task4);
//	loadtask(task5);
//	loadtask(task6);
//	loadtask(task7);

}
