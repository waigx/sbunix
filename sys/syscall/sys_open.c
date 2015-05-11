/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sbunix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sbunix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sbunix.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/defs.h>
#include <sys/tarfs.h>
#include <sys/file_op.h>
#include <sys/tarfs_api.h>
#include <string.h>
#include <sys/debug.h>
#include <sys/sbunix.h>
#include <sys/sched.h>
#include <sys/managemem.h>
#include <sys/mem.h>
#include <sys/kio.h>


uint64_t
sys_open(const char *pathname, int flags)
{
	int ret;
	uint64_t pathlen;
	char fullpath_buf[MAX_CWD_LEN];

	if (pathname[0] == '/') {
		strcpy(fullpath_buf, pathname);
	} else {
		strcpy(fullpath_buf, g_cwd);
		strcpy(fullpath_buf + strlen(g_cwd), pathname);
	}

	ret = open_tarfs(fullpath_buf + 1, flags);

	if (ret < 0){
		pathlen = strlen(fullpath_buf);
		fullpath_buf[pathlen] = '/';
		fullpath_buf[pathlen + 1] = '\0';
		ret = open_tarfs(fullpath_buf + 1, flags);
		if (ret < 0) {
			ret -= 1;
		}
	}

	return ret;
}
