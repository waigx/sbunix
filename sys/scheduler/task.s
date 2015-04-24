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

.global	switch_context


// macro for save all registers and segments for switching context//
.macro SAVE_CONTEXT 
	pushq %rbp
	pushq %rax
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

	movq %ds, %rax
	pushq %rax
	movq %es, %rax
	pushq %rax
	movq %fs, %rax
	pushq %rax
	movq %gs, %rax
	pushq %rax
.endm


/* macro for load all registers and segments for switching context*/
.macro LOAD_CONTEXT
	popq %rax
	movq %rax, %gs
	popq %rax
	movq %rax, %fs
	popq %rax
	movq %rax, %es
	popq %rax
	movq %rax, %ds

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
	popq %rax
	popq %rbp
.endm


/* 
 * Switch context
 */
switch_context:
	cli	
	pushq %rbp
	movq %rsp, %rbp

	pushfq
	cmp $0, %rdi
	je .Loadcontext
	popfq 	

	/* store current task to stack */

	pushq %rax

	movq %ss, %rax
	movq %rax, (23 * 8)(%rdi)

	movq %rbp, %rax
	add  $16, %rax
	movq %rax, (22 * 8)(%rdi)

	pushfq
	popq %rax
	movq %rax, (21 * 8)(%rdi)

	movq %cs, %rax
	movq %rax, (20 * 8)(%rdi)

	movq 8(%rbp) , %rax
	movq %rax, (19 * 8)(%rdi)

	popq %rax
	popq %rbp

	/* stored SS, RSP, RFLAGS, CS and RIP into the end of context*/
	add $(19 * 8), %rdi
	movq %rdi, %rsp
	sub $(19 * 8), %rdi

	/* store other registers */
	SAVE_CONTEXT

	/* load new task context */
.Loadcontext:
	movq %rsi, %rsp
	LOAD_CONTEXT
	sti
	iretq 

