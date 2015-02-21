/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
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


#include <stdarg.h>
#include <sys/defs.h>
#include <sys/kio.h>
#include <string.h>
#include <const.h>
#include <type.h>


int printf(const char *format, ...) {
	va_list val;
	char buf[PRINTF_LEN];
	char *buf_ptr;
	int printed;
	va_start(val, format);
	
	printed = strlistprintf(buf, format, val);
	for (buf_ptr = buf; *buf_ptr != '\0'; buf_ptr++) {
		writechar(*buf_ptr);
	}

	va_end(val);

	return printed;
}


