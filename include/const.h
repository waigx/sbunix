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


#ifndef _CONST_H
#define _CONST_H


#define STDIN_FD 0
#define STDOUT_FD 1


#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1


#define PS_MAX_LEN 2048
#define DIR_MAX_DEPTH 256
#define DIR_READ_BUF 256

#define MAXLINE 2048
#define HOSTNAME_FILE "/proc/sys/kernel/hostname"

#define MAX_ARGS 128
#define MAX_ENVP 128

#define UINT64_LEN 21

#define PRINTF_LEN 512

#endif
