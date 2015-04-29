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


#include <stdlib.h>

#include <sys/register.h>
#include <syscall.h>
#include <sys/sbunix.h>
#include <sys/sched/sched.h>
#include <sys/tarfs_api.h>
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
	
		case SYS_brk:
			sys_brk(a1);
			ret = get_rax_register();
			return ret;
			break;

		case SYS_fork:
			sys_fork();
			ret = get_rax_register();
			return ret;
			break;

		case SYS_getpid:
			sys_getpid();
			ret = get_rax_register();
			return ret;
			break;

		case SYS_getppid:
			sys_getppid();
			ret = get_rax_register();
			return ret;
			break;

		case SYS_execve:
			sys_execve((char *)a1, (char **)a2, (char **)a3);
			ret = get_rax_register();
			return ret;
			break;

//#define SYS_wait4      61
//#define SYS_nanosleep  35
//#define SYS_alarm      37
//#define SYS_getcwd     79
//#define SYS_chdir      80

		case SYS_open:
			open_tarfs((const char *)a1, a2);
			return get_rax_register();
			break;

		case SYS_read:
			read_tarfs((int)a1, (void *)a2, (size_t)a3);
			return get_rax_register();
			break;

		case SYS_write:
			sys_write((int)a1, (char *)a2, (size_t)a3);
			return get_rax_register();
			break;

		case SYS_lseek:
			lseek_tarfs((int)a1, (off_t)a2, (int)a3);
			return get_rax_register();
			break;

		case SYS_close:
			close_tarfs((int)a1);
			return get_rax_register();
			break;

//#define SYS_pipe       22
//#define SYS_dup        32
//#define SYS_dup2       33

		case  SYS_getdents:
			sys_getdentry((uint64_t)a1, (uint64_t *)a2, (uint64_t)a3);
			return get_rax_register();
			break;

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
