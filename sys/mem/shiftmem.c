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


#include <sys/mem.h>

/*
 * THE SHIFT NOT INCLUDE void *end
 *
 */

void shiftmem(void *start, void *end, long long offset)
{
	if (offset == 0)
		return;

	if (offset > 0) {
		while (start + offset < end) {
			*(char *)start = *((char *)(start) + offset);
			start += 1;
		}
		while (start < end) {
			*(char *)start = 0;
			start += 1;
		}
		return;
	}

	if (offset < 0) {
		end -= 1;
		while (end + offset >= start) {
			*(char *)end = *((char *)(end) + offset);
			end -= 1;
		}
		while (end >= start) {
			*(char *)end = 0;
			end -= 1;
		}
		return;
	}
}
