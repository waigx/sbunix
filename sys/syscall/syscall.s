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

.global	syscall_handler

.global _sys_exit
.global _sys_malloc
.global _sys_free
.global _sys_brk
.global _sys_fork
.global _sys_getpid
.global _sys_getppid
.global _sys_execve
.global _sys_waitpid
.global _sys_sleep
.global _sys_alarm

.global _sys_getcwd
.global _sys_chdir

.global _sys_open
.global _sys_read
.global _sys_write
.global _sys_lseek
.global _sys_close
.global _sys_pipe
.global _sys_dup
.global _sys_dup2



.extern sys_exit
.extern sys_malloc
.extern sys_free
.extern sys_brk
.extern sys_fork
.extern sys_getpid
.extern sys_getppid
.extern sys_execve
.extern sys_waitpid
.extern sys_sleep
.extern sys_alarm

.extern sys_getcwd
.extern sys_chdir

.extern sys_open
.extern sys_read
.extern sys_write
.extern sys_lseek
.extern sys_close
.extern sys_pipe
.extern sys_dup
.extern sys_dup2



// macro for save all registers and segments for switching context//
.macro SAVE_ALL_REG
	pushq %rbp
	//pushq %rax
	pushq %rbx
	pushq %rcx
	pushq %rdx
	pushq %rdi
	pushq %rsi
	pushq %r8
	pushq %r9
	pushq %r10
	pushq %r11
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15

.endm


/* macro for load all registers and segments for switching context*/
.macro LOAD_ALL_REG

	popq %r15
	popq %r14
	popq %r13
	popq %r12
	popq %r11
	popq %r10
	popq %r9
	popq %r8
	popq %rsi
	popq %rdi
	popq %rdx
	popq %rcx
	popq %rbx
	//popq %rax
	popq %rbp
.endm


/* 
 * Switch context
 */
syscall_handler:
	
	SAVE_ALL_REG
	
	//call syscall(%rax, %rdx,%rcx ,%rbx,%rdi ,%rsi,0x8)
	call syscall 	
	
	LOAD_ALL_REG
	iretq 

