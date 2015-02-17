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


#include <sys/defs.h>
#include <const.h>
#include <string.h>
#include <type.h>


char *
utoa(char *buf, uint64_t num)
{
	int i = 0;
	char res_rev[UINT64_LEN];

	while (num != 0) {
		res_rev[i] = num % CONV_BASE + CHAR_ZERO;
		num /= CONV_BASE;
		i += 1;
	}
	res_rev[i] = '\0';
	strrev(buf, res_rev);

	return buf;
}
