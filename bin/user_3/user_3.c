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
	pid_t pid;
	pid_t this_pid;
	pid_t p_pid;
	int x=0;

	pid = fork();
	if (pid == 0)
		x = 95;
	this_pid = getpid();
	p_pid = getppid();

	while(1)
	{
		printf("Returned Pid is %d, Parent Pid is %d, This Pid is %d\n", pid, p_pid, this_pid);
		printf("x value: %d\n", x++);
	}
	return 0;
}
