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

int main(int argc, char* argv[], char* envp[]) 
{
	int x[10];
	int i = 0;
	pid_t pid;
	for (; i<10; i++){
		x[i] = i + 3;
	}
	pid = fork();

	if (pid == 0) {
		for (i = 0; i<10; i++)
			printf("Child: %d\n", x[i]);
		return 0;
	} 
	for (i = 0; i<10; i++)
		printf("parent: %d\n", x[i]);

	return 0;
}
