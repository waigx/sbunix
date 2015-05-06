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


#include <syscall.h>
#include <stdlib.h>

#include <stdio.h>
//#include <sys/sbunix.h>
#include <syscall.h>
#include <stdlib.h>
//#include <sys/mem.h>
#include <const.h>
#include <libstr.h>
#include <string.h>
#include <libio.h>


ssize_t read(int fd, void *buf, size_t count)
{
	ssize_t length = 0;
//	uint64_t i = 0;
	/* test code dongju */
#if(1)
	if(fd == STDIN_FD){
//		for(i = 0; i< count; i++){
			while( length == 0 || length == 254){  
				length = syscall_3(SYS_read, fd, (uint64_t)buf, count); 
				}
	//	}
	}else{
		 length = syscall_3(SYS_read, fd, (uint64_t)buf, count);
	}  


#else

	length = syscall_3(SYS_read, fd, (uint64_t)buf, count);
#endif
	return length;
}
