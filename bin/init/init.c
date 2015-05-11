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
#include <libio.h>
#include <libcommon.h>
#include <const.h>
#include <string.h>
#include <libio.h>
#include <libstr.h>


int main(int argc, char *argv[], char *envp[]) {
	int envp_index;
	int fd_rc;
	int child_pid;
	char buf[MAXLINE];
	char *buf_ptr;
	char *argv_rc[1] = {NULL};
	char *envp_rc[MAX_ENVP];

	envp_index = 0;

	//read init parameters;
	writeline("Reading configrations ...\n", STDOUT_FD);
	fd_rc = open(INIT_CONFIG, O_RDONLY);
	while (readline(buf, fd_rc) != NULL) {
		buf_ptr = malloc(strlen(buf) + 1);
		strcpy(buf_ptr, buf);
		envp_rc[envp_index] = buf_ptr;
		envp_index += 1;
	}
	envp_rc[envp_index] = NULL;
	close(fd_rc);
	writeline("Done.\n", STDOUT_FD);

	//set up CWD;
	writeline("Initializing current working directory ...\n", STDOUT_FD);
	getopt(buf, "PWD", envp_rc);
	chdir(buf);
	printf("%s\n", buf);
	writeline("Done.\n", STDOUT_FD);

	//set up other envps;
	writeline("Initializing shell...\n", STDOUT_FD);
	getopt(buf, "SHELL", envp_rc);

	child_pid = fork();

	if (child_pid == 0){
		execve(buf, argv_rc, envp_rc);
	}

	while (1);
}
