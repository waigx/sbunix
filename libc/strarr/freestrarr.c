/*
 *
 *  This file is part of SBUsh.
 *        SBUsh is a simple shell implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  SBUsh is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  SBUsh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with SBUsh.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/defs.h>
#include <stdlib.h>


/**
 * freestrarr - free a string array
 * @buf_ptr_ptr: the pointer points to char * array
 * @isfreeall: Free the char * array or not
 *
 * Returns NULL
 *
 * Using parameter FREE_ALL to free the memory root @buf_ptr_ptr 
 * points to, and using FREE_LEAF to free all leaves which char *
 * array pointed
 */
char *
freestrarr(char **buf_ptr_ptr, int isfreeall)
{
	char **ptr_ptr = buf_ptr_ptr;
	while (*ptr_ptr != NULL) {
		free(*ptr_ptr);
		ptr_ptr += 1;
	}
	if (isfreeall == 1) {
		free(buf_ptr_ptr);
	}
	return NULL;
}
