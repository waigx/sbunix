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
//#include <sys/sbunix.h>
#include <syscall.h>
//#include <sys/kio.h>

int main(int argc, char* argv[], char* envp[]) 
{
	uint64_t i = 0;
	//    uint64_t j = 10000;

	while(1)
	{
		printf("I'm user_4 and count = %x\n",i++);
		//		  __asm volatile("sti");		
		//		printfat(10, 10,"I'm user 222 \n");
		//                while(j--);
		//              j = 1000000;
		//          if(i % 5 == 0)
		//                        yield(13);
	}
	return 0;
}
