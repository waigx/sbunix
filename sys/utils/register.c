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

uint64_t get_rax_register(void)
{
	uint64_t ret = 0;
	__asm__ __volatile__("movq %%rax, %0"
			:"=r"(ret)
			:
			);
	return ret;
}

uint64_t get_rdi_register(void)
{
	uint64_t ret = 0;
	__asm__ __volatile__("movq %%rdi, %0"
			:"=r"(ret)
			:
			);
	return ret;
}

uint64_t get_rsi_register(void)
{
	uint64_t ret = 0;
	__asm__ __volatile__("movq %%rsi, %0"
			:"=r"(ret)
			:
			);
	return ret;
}
uint64_t get_rdx_register(void)
{
	uint64_t ret = 0;
	__asm__ __volatile__("movq %%rdx, %0"
			:"=r"(ret)
			:
			);
	return ret;
}

uint64_t get_r10_register(void)
{
	uint64_t ret = 0;
	__asm__ __volatile__("movq %%r10, %0"
			:"=r"(ret)
			:
			);
	return ret;
}

uint64_t get_cr2_register(void)
{
	uint64_t ret = 0;
	__asm__ __volatile__("movq %%cr2, %0"
			:"=r"(ret)
			:
			);
	return ret;
}

uint64_t get_cr3_register(void)
{
	uint64_t ret = 0;
	__asm__ __volatile__("movq %%cr3, %%rax;"
			"movq %%rax, %0;"
			:"=r"(ret)
			:
			);
	return ret;
}

