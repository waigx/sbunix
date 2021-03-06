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


#include <const.h>
#include <string.h>
#include <type.h>
#include <stdio.h>

int
ctou(char ch, uint8_t base)
{
	uint8_t ret = ch - '0';
	if (ret < 0 || ret >= base)
		return -1;
	return ret;
}

uint64_t
atou(const char *num, uint8_t base)
{
	uint64_t ret = 0;
	char *temp_ptr = (char *)num;

	for (; *temp_ptr != '\0'; temp_ptr++) {
		if (ctou(*temp_ptr, base) < 0)
			return -1;
		ret += ret * base + ctou(*temp_ptr, base);
	}
	return ret;
}
