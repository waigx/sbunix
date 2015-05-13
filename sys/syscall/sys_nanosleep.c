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


#include <sys/sched.h>
#include <sys/managemem.h>
#include <sys/debug.h>


uint64_t
sys_nanosleep(uint64_t end_data_segment)
{
	vma_t *heap_vma = lookupvmabyname(VMA_HEAP_NAME);
	if (end_data_segment == 0)
		return (uint64_t)(heap_vma->vaddr_end);
	if (end_data_segment < (uint64_t)(heap_vma->vaddr_start))
		return -1;
	heap_vma->vaddr_end = (void *)end_data_segment;
	return 0;
}
