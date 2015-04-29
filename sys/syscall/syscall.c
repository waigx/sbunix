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

#include <sys/register.h>
#include <syscall.h>
#include <sys/sbunix.h>
#include <sys/sched/sched.h>
#include <sys/debug.h>


uint64_t syscall(void)
{
	uint64_t ret = 0;
	uint64_t syscall_num = 0;
	uint64_t a1 = 0;
	uint64_t a2 = 0;
	uint64_t a3 = 0;
	uint64_t a4 = 0;
	uint64_t a5 = 0;


	__asm__ __volatile__("movq %%rax, %0"
			:"=r"(ret)
			:
			);
	syscall_num =  ret;


	syscall_num = get_rax_register();
	a1 = get_rdi_register();
	a2 = get_rsi_register();
	a3 = get_rdx_register();
	a4 = get_r10_register();
	a5 = get_rsi_register();

//	printf("a1=%x, a2=%x, a3=%x, a4=%x\n",a1, a2, a3, a4);

	switch(syscall_num) {

		case SYS_exit:
			sys_exit(a1);
			return 0;
			break;
	
//#define SYS_brk        12

		case SYS_fork:
			ret = sys_fork();
			return ret;
			break;

		case SYS_getpid:
			ret = sys_getpid();
			return ret;
			break;

		case SYS_getppid:
			ret = sys_getppid();
			return ret;
			break;

		case SYS_execve:
			ret = sys_execve((char *)a1, (char **)a2, (char **)a3);
			return ret;
			break;


//#define SYS_wait4      61
//#define SYS_nanosleep  35
//#define SYS_alarm      37
//#define SYS_getcwd     79
//#define SYS_chdir      80
//#define SYS_open        2
//#define SYS_read        0
//#define SYS_write       1
//#define SYS_lseek       8
//#define SYS_close       3
//#define SYS_pipe       22
//#define SYS_dup        32
//#define SYS_dup2       33
//#define SYS_getdents   78

		case SYS_yield:
			sys_yield();
			return 0;
			break;

		case SYS_printf: 
			printf((const char *)a1, a2, a3, a4, a5);
			return 0;
			break;

		default:
			printf("Invalid system call number: %d\n", syscall_num);
			//printf("a1=%x, a2=%x, a3=%x, a4=%x\n",a1, a2, a3, a4);
	} 

	return 0;
}
