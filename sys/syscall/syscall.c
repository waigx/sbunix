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

	switch(syscall_num)
	{

		case SYS_yield:
			//printf("calling sys_yield()\n");
			sys_yield();
			return 0;
			break;

		case SYS_printf: 
			
			printf((const char *)a1, a2, a3, a4, a5);
			return 0;
			break;

/*
		SYS_brk:

			
		SYS_fork:


		SYS_getpid:	


		SYS_getppid:


		SYS_execve:


		SYS_wait4:

		SYS_nanosleep:


		SYS_alarm:     


		SYS_getcwd:    

		SYS_chdir:     
		
		SYS_open:      
		SYS_read:      
		SYS_write:     
		SYS_lseek:     
		SYS_close:     
		SYS_pipe:      
		SYS_dup:       
		SYS_dup2:      
		SYS_getdents:  

 */
		default:
		
			printf("no syscall number\n");
			//printf("a1=%x, a2=%x, a3=%x, a4=%x\n",a1, a2, a3, a4);
			
	} 

	return 0;
}
