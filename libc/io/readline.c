/*
 *
 *  This file is part of SBUsh.
 *        SBUsh is a simple shell implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  SBUsh is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  SBUsh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with SBUsh.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/defs.h>
#include <stdlib.h>


/**
 * readline - read a line end with ASCII 0, 10 or 13
 * @buf: The result buffer
 * @fd: The file to be read
 *
 * Returns @buf from @fd, NULL if @fd has nothing to read
 *
 * If contents successfully read, function will replace  the last
 * ASCII 10 or 13 to 0
 */
char *
readline(char *buf, int fd)
{
	int n;
	int buf_index = 0;

	while ((n = read(fd, buf + buf_index, 1)) != 0) {
		if ( buf[buf_index] == 0 || buf[buf_index] == 10 || buf[buf_index] == 13) {
			buf[buf_index] = '\0';
			return buf;
		}
		buf_index += 1;
	}

	return NULL;
}
