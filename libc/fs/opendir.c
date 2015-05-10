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


#include <stdlib.h>
#include <sys/defs.h>
#include <syscall.h>
#include <const.h>
#include <stdio.h>


void *opendir(const char *name)
{
	DIR *dir;
	int fd = open(name, O_RDONLY);
	char *buf = malloc(DIR_READ_BUF);
	uint64_t sys_call_res;

	if (fd < 0)
		return NULL;

	sys_call_res = syscall_3(SYS_getdents, (uint64_t)fd, (uint64_t)buf, (uint64_t)DIR_READ_BUF);

	if ((ssize_t)sys_call_res < 0)
		return NULL;

	dir = malloc(sizeof(DIR)); 

	dir->dir_fd = fd;
	dir->size = sys_call_res;
	dir->dirent_next = (struct sblib_dirent *)buf;
	dir->buf = buf;
	
	return dir;
}
