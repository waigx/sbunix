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


char *
writeline(const char *buf, int fd)
{
	int buf_len = strlen(buf);
	write(fd, buf, buf_len);

	return NULL;
}


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
