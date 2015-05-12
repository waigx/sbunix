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
#include <string.h>
#include <stdio.h>


/**
 * joinstrlst - concatenate a string array with another string
 * @buf: The result buffer
 * @str_ptr: A string array to be concatenated
 * @salt: A string to be added to strings
 *
 * Returns a pointer points to the first char of @buf
 */
char *
joinstrlst(char *buf, char **str_ptr, const char *salt)
{
	char *buf_ptr = buf;
	int salt_len = strlen(salt);

	if (str_ptr[0] == NULL) {
		buf[0] = '\0';
		return buf;
	}

	while (*(str_ptr + 1) != NULL) {
		strcpy(buf_ptr, *str_ptr);
		buf_ptr += strlen(*str_ptr);
		strcpy(buf_ptr, salt);
		buf_ptr += salt_len;
		str_ptr += 1;
	}
	strcpy(buf_ptr, *str_ptr);

	return buf;
	
}
