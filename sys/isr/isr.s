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


.extern divide_handler
.extern timer_handler
.extern keyboard_handler
.extern pagefault_handler

.extern debug

.global isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7, isr8, isr9
.global isr10, isr11, isr12, isr13, isr14, isr15, isr16, isr17, isr18, isr19
.global isr20, isr21, isr22, isr23, isr24, isr25, isr26, isr27, isr28, isr29
.global isr30, isr31, isr32, isr33, isr34, isr35, isr36, isr37


// macro for save all registers and segments for switching context//
.macro SAVE_ALL_REG 
	pushq %rbp
	movq %rsp, %rbp
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
	movq %gs, %rax
	pushq %rax
	movq %fs, %rax
	pushq %rax

	movq $0x10, %rax
	movq %rax , %ds
	movq %rax, %es
	movq %rax, %gs
	movq %rax, %fs
.endm


/* macro for load all registers and segments for switching context*/
.macro LOAD_ALL_REG
	popq %rax
	movq %rax, %ds
	popq %rax
	movq %rax, %es
	popq %rax
	movq %rax, %gs
	popq %rax
	movq %rax, %fs

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
 * Exception handlers
 */

/* ISR 0,
 * 
 * Division by Zero Exception 
 */
isr0:
	cli
	pushq $0
	pushq $4

	SAVE_ALL_REG
	movq $0, %rdi
	call divide_handler

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq

isr1:
	cli
	pushq $0
	pushq $1

	SAVE_ALL_REG
	movq $1, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr2:
	cli
	pushq $0
	pushq $2

	SAVE_ALL_REG
	movq $2, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr3:
	cli
	pushq $0
	pushq $3

	SAVE_ALL_REG
	movq $3, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr4:
	cli
	pushq $0
	pushq $4

	SAVE_ALL_REG
	movq $4, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq

isr5:
	cli
	pushq $0
	pushq $5

	SAVE_ALL_REG
	movq $5, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr6:
	cli
	pushq $0
	pushq $6

	SAVE_ALL_REG
	movq $6, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr7:
	cli
	pushq $0
	pushq $7

	SAVE_ALL_REG
	movq $7, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr8:
	cli
	pushq $0
	pushq $8

	SAVE_ALL_REG
	movq $8, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr9:
	cli
	pushq $0
	pushq $9

	SAVE_ALL_REG
	movq $9, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr10:
	cli
	pushq $0
	pushq $10

	SAVE_ALL_REG
	movq $10, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr11:
	cli
	pushq $0
	pushq $11

	SAVE_ALL_REG
	movq $11, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr12:
	cli
	pushq $0
	pushq $12

	SAVE_ALL_REG
	movq $12, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr13:
	cli
	pushq $0
	pushq $13

	SAVE_ALL_REG
	movq $13, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


/* ISR 0x0E(14),
 * 
 * 0x0E(14), Page Fault
 */
isr14:
	cli
	pushq $0
	pushq $14

	SAVE_ALL_REG
	movq $14, %rdi
	call pagefault_handler

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr15:
	cli
	pushq $0
	pushq $15

	SAVE_ALL_REG
	movq $15, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr16:
	cli
	pushq $0
	pushq $16

	SAVE_ALL_REG
	movq $16, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr17:
	cli
	pushq $0
	pushq $17

	SAVE_ALL_REG
	movq $17, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr18:
	cli
	pushq $0
	pushq $18

	SAVE_ALL_REG
	movq $18, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr19:
	cli
	pushq $0
	pushq $19

	SAVE_ALL_REG
	movq $19, %rdi
	call debug

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq


isr20:
isr21:
isr22:
isr23:
isr24:
isr25:
isr26:
isr27:
isr28:
isr29:
isr30:
isr31:

/* ISR 32,
 * 
 * 32, Time handler 
 */
isr32:
	cli
	pushq $0
	pushq $32
	
	SAVE_ALL_REG
	movq $0, %rdi
	call timer_handler
	
	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq

/* ISR 33,
 * 
 * 33, Keyboard handler 
 */
isr33:
	cli
	pushq $0
	pushq $33

	SAVE_ALL_REG
	movq $0, %rdi
	call keyboard_handler

	LOAD_ALL_REG
	add $0x10, %rsp
	sti
	iretq

isr34:
isr35:
isr36:
isr37:
