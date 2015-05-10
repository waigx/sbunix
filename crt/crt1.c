/*
 *
 *  This file is part of sblibc.
 *        sblibc is a simple libc implementation, basically, it is
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


#include <stdlib.h>
#include <sys/defs.h>

void _start(void) {
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	int res;

	__asm__ __volatile__(
			"movq %%rdi, %0;\n\t"
			: "=r" (rdi)
			:
			: "rdi");

	__asm__ __volatile__(
			"movq %%rsi, %0;\n\t"
			: "=r" (rsi)
			:
			: "rsi");

	__asm__ __volatile__(
			"movq %%rdx, %0;\n\t"
			: "=r" (rdx)
			:
			: "rdx");

	res = main(rdi,(char **)rsi, (char**)rdx);
	exit(res);

}
