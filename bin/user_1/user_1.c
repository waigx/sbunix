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


#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
#include <libsys.h>

void testfun(int i)
{
	volatile uint64_t x[100];
	volatile uint64_t j;
	for (j = 3; j<100; j++)
		*(x + j) = j;

	printf("stack %x\n", i);
}



int main(int argc, char* argv[], char* envp[]) 
{
	uint64_t i = 0;
//	volatile uint64_t *test1_ptr = (uint64_t *)0xffffffff7ffec748;
//	volatile uint64_t *test2_ptr = (uint64_t *)0xffffffff7ffec648;
	//uint64_t *test_ptr = (uint64_t *)0xadeadbeef;


	//printf("1");
	//while(1);

	printf("I'm user_1 and count = %x\n",i++);
	printf("break: %p\n",sbrk(0));
//		*test1_ptr = 123;
//		*test2_ptr = 124;
//		*test3_ptr = 125;
//		printf("test1_ptr-> %d\n",*test1_ptr);
//		printf("test2_ptr-> %d\n",*test2_ptr);

	testfun(i);
//	while(1);
	return 0;
}

