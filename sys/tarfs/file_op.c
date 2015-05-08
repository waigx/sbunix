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
#include <sys/tarfs_api.h>
#include <string.h>
#include <sys/sbunix.h>
#include <sys/keyboard.h>
#include <sys/sched.h>
#include <sys/managemem.h>
#include <sys/mem.h>
#include <sys/kio.h>


extern uint64_t g_keyboard_buf;
extern uint64_t g_valid_keyboard;
extern uint64_t  g_diable_scheduler;
uint64_t read_terminal(void *buf, size_t count);


ssize_t sys_read(int fd, void *buf, size_t count)
{
	uint64_t ret = 0;

	if(fd < STANDARD_IO_IN){
		printf("sys_read: fd is invalid\n");
		return 0;

	} else if(fd == STANDARD_IO_IN){
		ret = read_terminal(buf, count);

	}else if(fd == STANDARD_IO_OUT){
		printf("sys_read: fd error\n");
		return 0;

	}else if(fd == STANDARD_IO_ERROR){

	}else
		ret = read_tarfs(fd, (char *)buf, count);

	
	//printf("sys_read: ret = %x\n", ret);
	 return (ssize_t)ret;
}


uint64_t read_terminal(void *buf, size_t count)
{
	uint64_t i = 0;

	for(i = 0; i < count; i++) {
		if(start_queue > end_queue) {
			*((char *)buf+i ) = (char)kb_queue[end_queue++];
			if(start_queue <= end_queue) {
				start_queue = 0;
				end_queue = 0;
			}
		}
		else
			i = 254 - 1;

	}

	return (uint64_t)i;
}


