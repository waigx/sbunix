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
#include <string.h>
#include <const.h>
#include <type.h>


int
strlistprintf(char *buf, const char *format, va_list val)
{
	int var_int;
	uint32_t var_uint;
	char var_char;
	char *var_charp;
	char num_buf[UINT64_LEN];
	int printed = 0;


	for (; *format != '\0'; format++, printed++) {
		if (*format != '%') {
			*(buf + printed) = *format;
			continue;
		}
		format += 1;
		if (*format == '\0')
			break;
		switch (*format) {
			case 's':
				var_charp = va_arg(val, char *);
				strcpy(buf + printed, var_charp);
				printed += strlen(var_charp) - 1;
				break;
			case 'd':
				var_int = va_arg(val, int);
				itoa(num_buf, var_int, 10);
				strcpy(buf + printed, num_buf);
				printed += strlen(num_buf) - 1;
				break;
			case 'u':
				var_uint = va_arg(val, uint32_t);
				utoa(num_buf, var_uint, 10);
				strcpy(buf + printed, num_buf);
				printed += strlen(num_buf) - 1;
				break;
			case 'b':
				var_uint = va_arg(val, uint32_t);
				utoa(num_buf, var_uint, 2);
				strcpy(buf + printed, num_buf);
				printed += strlen(num_buf) - 1;
				break;
			case 'x':
				var_uint = va_arg(val, uint32_t);
				utoa(num_buf, var_uint, 16);
				strcpy(buf + printed, num_buf);
				printed += strlen(num_buf) - 1;
				break;
			case 'p':
				strcpy(buf + printed, "0x");
				printed += 2;
				var_uint = va_arg(val, uint32_t);
				utoa(num_buf, var_uint, 16);
				strcpy(buf + printed, num_buf);
				printed += strlen(num_buf) - 1;
				break;
			case 'c':
				var_char = va_arg(val, int);
				*(buf + printed) = var_char;
				break;
			default:
				break;
		}
	}
	*(buf + printed) = '\0';

	return printed;
}
