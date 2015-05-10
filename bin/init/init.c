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
#include <const.h>
#include <libio.h>


int main(int argc, char *argv[], char *envp[]) {
	//read init parameters;
	writeline("Reading configrations ...\n", STDOUT_FD);

	writeline("Done.\n", STDOUT_FD);
	//set up CWD;
	writeline("Initializing current working directory ...\n", STDOUT_FD);

	writeline("Done.\n", STDOUT_FD);
	//set up other envps;
	writeline("Initializing shell...\n", STDOUT_FD);

	//initialize bash;


	while (1);
}
