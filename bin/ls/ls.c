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
#include <const.h>
#include <libio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char* argv[], char* envp[]) {
	char currentDir[MAXLINE];
	char *target = argv[1];
	struct dirent *pDirent;
	DIR *pDir;

	getcwd(currentDir, MAXLINE);

	if (argc < 2) {
		target = currentDir;
	}

	pDir = opendir(target);
	if (pDir == NULL) {
		return 1;
	}

	while ((pDirent = readdir(pDir)) != NULL) {
		writeline(pDirent->d_name, STDOUT_FD);
		writeline("  ", STDOUT_FD);
	}
	writeline("\n", STDOUT_FD);
	return 0;
}
