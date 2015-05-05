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
#include <stdlib.h>
//#include <sys/mem.h>
#include <const.h>
#include <libstr.h>
#include <string.h>
#include <libio.h>

void file_read_test(void);
void file_opendir_test(void);
void write_test(void);


int main(int argc, char* argv[], char* envp[]) 
{
	uint64_t i = 0;

	char buf[128];

	for(i=0; i<128; i++)
		*(buf+i) = 0;

//	while(1);
//	readline(buf, STDIN_FD);
//	printf("buf = %s\n", buf);

	//write_test();
	//file_read_test();
	//file_opendir_test();

	//while(1);

        while(1)
        {
                printf("I'm user_2 and count = %x\n",i++);
//		  __asm volatile("cli");		
//		printfat(10, 10,"I'm user 222 \n");
//                while(j--);
  //              j = 1000000;
      //          if(i % 5 == 0)
 //                       yield(13);
        }
        return 0;
}

void write_test(void)
{
	char* buf = "testing sys write";

        write(STDOUT_FD, buf, 17);
        write(STDOUT_FD, buf, 10);
        write(STDOUT_FD, buf, 7);

}


void file_read_test(void)
{
	uint64_t i = 0;
        int fd = 0;
        char buf[128];
        uint64_t size = 0;


	 fd = open("bin/user_2", 0);

        printf("open fd = %x \n", fd);

        printf("buf = %x\n", buf);
        size = read(fd, buf, 8);

        printf("read size = %x\n", size);

        for(i = 0; i < 8; i++)
        {
                printf("buf[%x]", i);
                printf("=%x\n", buf[i]);
        }

        size = read(fd, buf, 8);
        printf("read size = %x\n", size);

        for(i = 0; i < 8; i++)
        {
                printf("buf[%x]", i);
                printf("=%x\n", buf[i]);
        }
        size = read(fd, buf, 8);
        printf("read size = %x\n", size);

        for(i = 0; i < 8; i++)
        {
                printf("buf[%x]", i);
                printf("=%x\n", buf[i]);
        }
}

void file_opendir_test(void)
{
//	uint64_t i = 0;
        DIR *pDir;
        struct dirent *pDirent;

	pDir = opendir("bin/"   /*"bin/"*/);
        printf("pDir ->fd = %x\n", pDir->dir_fd);

         while ((pDirent = readdir(pDir)) != NULL){
                printf("ls:%s\n", pDirent->d_name);
        }



}
