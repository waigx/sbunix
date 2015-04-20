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


#include <stdarg.h>
#include <sys/defs.h>
#include <string.h>
#include <const.h>
#include <type.h>
#include <libio.h>

#include <syscall.h>

int printf(const char *format, ...) {
	va_list val;
	char buf[PRINTF_LEN];
	int printed;
	va_start(val, format);

// Temp code for using printf in USER program //

if(1)
{
	syscall_1(SYS_printf, (uint64_t)format);
	printed = 0;
}
else	
{
	printed = strlistprintf(buf, format, val);
	writeline(buf, STDOUT_FD);
}
	va_end(val);

	return printed;
}


