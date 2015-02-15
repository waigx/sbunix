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
#include <const.h>
#include <sys/syscall.h>
#include <syscall.h>
#include <libsys.h>

struct dirent *readdir(void *dir)
{
	DIR *dir_in = dir;
	struct dirent *res_dirent;
	uint64_t sys_call_res;

	if ((char *)dir_in->dirent_next  - dir_in->buf + dir_in->dirent_next->d_reclen >= dir_in->size) {
		sys_call_res = syscall_3(SYS_getdents, (uint64_t)dir_in->dir_fd, (uint64_t)dir_in->buf, (uint64_t)DIR_READ_BUF);
		if ((ssize_t)sys_call_res < 0 || (ssize_t)sys_call_res == 0)
			return NULL;

		sbrk(0);

		dir_in->size = sys_call_res;
		dir_in->dirent_next = (struct sblib_dirent *)dir_in->buf;
	}

	res_dirent = (struct dirent *)dir_in->dirent_next;
	dir_in->dirent_next = (struct sblib_dirent *)((char *)dir_in->dirent_next + dir_in->dirent_next->d_reclen);

	return res_dirent;
}
