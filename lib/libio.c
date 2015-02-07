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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libcommon.h"


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


/**
 * writeline - write a string with \0
 * @buf: The string
 * @fd: The file to be wrote
 *
 * Returns NULL
 */
char *
writeline(const char *buf, int fd)
{
	int buf_len = strlen(buf);
	write(fd, buf, buf_len);

	return NULL;
}


/**
 * pathtype - return the type of a path
 * @path: the path of a file or directory
 *
 * Returns an integer:
 *     * PATH_TYPE_NON if nothing exists
 *     * PATH_TYPE_FIL if the @path is a file
 *     * PATH_TYPE_DIR if the @path is a directory
 */
int
pathtype(const char *path)
{
	int fd;
	void * fd_d;
	int is_dir, is_fil;

	if ((fd = open(path, O_RDONLY)) < 0) {
		is_fil = 0;
	} else {
		is_fil = 1;
		close(fd);
	}

	if ((fd_d = opendir(path)) == NULL ) {
		is_dir = 0;
	} else {
		is_dir = 1;
		closedir(fd_d);
	}

	if (is_fil + is_dir == 0) {
		return PATH_TYPE_NON;
	} else if (is_fil == 1 && is_dir == 0) {
		return PATH_TYPE_FIL;
	} else if (is_fil == 1 && is_dir == 1) {
		return PATH_TYPE_DIR;
	}

	return PATH_TYPE_NON;
}
