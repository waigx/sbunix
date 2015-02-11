/*
 *
 *  This file is part of sblibc.
 *        sblibc is a simple shell implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sblibc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sblibc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sblibc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _SYSCALL_H
#define _SYSCALL_H

#include <sys/defs.h>
#include <sys/syscall.h>

static __inline uint64_t syscall_0(uint64_t n)
{
	uint64_t returned_value;

	__asm__ __volatile__(
		"movq %1, %%rax\n\t"
		"syscall;"
		"movq %%rax, %0\n\t"
		: "=r" (returned_value)
		: "r" (n)
		: "%rax");

	return returned_value;
}

static __inline uint64_t syscall_1(uint64_t n, uint64_t a1)
{
	uint64_t returned_value;

	__asm__ __volatile__(
		"movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"syscall;"
		"movq %%rax, %0\n\t"
		: "=r" (returned_value)
		: "r" (n), "r" (a1)
		: "%rax", "%rdi");

	return returned_value;
}

static __inline uint64_t syscall_2(uint64_t n, uint64_t a1, uint64_t a2) 
{
	uint64_t returned_value;

	__asm__ __volatile__(
		"movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
		"syscall;"
		"movq %%rax, %0\n\t"
		: "=r" (returned_value)
		: "r" (n), "r" (a1), "r" (a2)
		: "%rax", "%rdi", "%rsi");

	return returned_value;
}

static __inline uint64_t syscall_3(uint64_t n, uint64_t a1, uint64_t a2, uint64_t a3)
{
	uint64_t returned_value;

	__asm__ __volatile__(
		"movq %1, %%rax\n\t"
		"movq %2, %%rdi\n\t"
		"movq %3, %%rsi\n\t"
		"movq %4, %%rdx\n\t"
		"syscall;"
		"movq %%rax, %0\n\t"
		: "=r" (returned_value)
		: "r" (n), "r" (a1), "r" (a2), "r" (a3)
		: "%rax", "%rdi", "rsi", "rdx");

	return returned_value;
}

#endif
