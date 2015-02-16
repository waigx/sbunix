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


#include <sys/syscall.h>
#include <syscall.h>
#include <stdlib.h>
#include <sys/resource.h>


pid_t waitpid(pid_t pid, int *status, int options)
{
	struct rusage *ru = malloc(sizeof(struct rusage));
	uint64_t res;

	res = syscall_4(SYS_wait4, (uint64_t)pid, (uint64_t)status, (uint64_t)options, (uint64_t)ru);

	free(ru);

	return (pid_t)res;
}
