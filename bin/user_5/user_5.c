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
#include <stdlib.h>
#include <const.h>
#include <libstr.h>
#include <libio.h>

void read_file_test(void)
{
	char buf[128];
	int fd1 = open("etc/SBUsh.SBUshrc", O_RDONLY);
	int fd3 = open("etc/testfile2.txt", O_RDONLY);
	printf("[U5]fd1 %d\n", fd1);
	while ( readline(buf, fd1) != NULL) {
		printf("[U5]%s\n", buf);
	}
	close(fd1);
	int fd2 = open("etc/testfile1.txt", O_RDONLY);
	printf("[U5]fd2 %d\n", fd2);
	while ( readline(buf, fd2) != NULL) {
		printf("[U5]%s\n", buf);
	}
	printf("[U5]fd3 %d\n", fd3);
	while ( readline(buf, fd3) != NULL) {
		printf("[U5]%s\n", buf);
	}
	close(fd2);
	close(fd3);
	return;
}


int main(int argc, char* argv[], char* envp[]) 
{
	//	uint64_t i = 0;
	//    uint64_t j = 10000;

	while(1)
	{
	read_file_test();
		//		printf("I'm user_5 and count = %x\n",i++);
		//		  __asm volatile("sti");		
		//		printfat(10, 10,"I'm user 222 \n");
		//                while(j--);
		//              j = 1000000;
		//          if(i % 5 == 0)
		//                        yield(13);
	}
	return 0;
}
