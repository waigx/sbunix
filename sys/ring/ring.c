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
//#include <sys/register.h>
//#include <syscall.h>
#include <sys/sbunix.h>
#include <string.h>
//#include <sys/elf.h>
//#include <sys/tarfs_api.h>
//#include <sys/sched/sched.h>


#include <sys/ring.h>
#include <sys/gdt.h>
#include <sys/sched/sched.h>
//#include <sys/managemem.h>
//#include <sys/mem.h>
#include <sys/register.h>
//#include <sys/debug.h>

//uint64_t check_ELF_format( Elf64_Ehdr *elf);

void set_rsp_tss(uint64_t rsp)
{
	//	uint64_t rsp0 = get_rax_register();
	task_t *task;
	//struct tss_t *tss;
	//rsp = get_rax_register();
	task = (task_t *)get_current_task();
	//tss = (struct tss_t *)&(task->tss);
	setup_tss_rsp((struct tss_t *)task->tsss, (uint64_t)(task->k_stack_base) + 4096 - 8);	


}


