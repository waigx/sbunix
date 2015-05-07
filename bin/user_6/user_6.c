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

void read_file_test(void);
void muli_file_test(void);
void std_input_test(void);
void dir_read_test(void);


int main(int argc, char *argv[], char *envp[])
{
	writeline("---Start of read file test---\n", STDOUT_FD);
	read_file_test();
	writeline("---Start of read dir test---\n", STDOUT_FD);
	dir_read_test();
	writeline("---Start of read file test---\n", STDOUT_FD);
	while(1);
	return 0;
}

void read_file_test(void)
{
	char buf[128];
	int fd1 = open("etc/SBUsh.SBUshrc", O_RDONLY);
	int fd3 = open("etc/testfile2.txt", O_RDONLY);
	while ( readline(buf, fd1) != NULL) {
		printf("%s\n", buf);
	}
	close(fd1);
	int fd2 = open("etc/testfile1.txt", O_RDONLY);
	while ( readline(buf, fd2) != NULL) {
		printf("%s\n", buf);
	}
	while ( readline(buf, fd3) != NULL) {
		printf("%s\n", buf);
	}
	close(fd2);
	close(fd3);
	return;
}


void dir_read_test(void) {
	struct dirent *pDirent;
	DIR *pDir;

	pDir = opendir("bin/");
	if (pDir == NULL) {
		return;
	}

	while ((pDirent = readdir(pDir)) != NULL) {
		writeline(pDirent->d_name, STDOUT_FD);
		writeline("  ", STDOUT_FD);
	}
	writeline("\n", STDOUT_FD);
	return;
}
