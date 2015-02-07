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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libio.h"
#include "libstr.h"
#include "libcommon.h"


/**
 * function - brief discription
 * @arg1: Discription
 * @arg2: Discription
 *
 * Returns and brief discription
 *
 * Details of this function
 * ...
 * ...
 */
char **
setopt(const char *opt, const char *name, char *opt_ptr[])
{
	char buf[PS_MAX_LEN];
	char newopt[PS_MAX_LEN];
	char **current_opt = opt_ptr;
	int optarrlen = lenstrarr(opt_ptr);

	strcpy(newopt, name);
	strcpy(newopt + strlen(name), "=");
	strcpy(newopt + strlen(name) + 1, opt);

	if (getopt(buf, name, opt_ptr) == NULL) {
		opt_ptr[optarrlen] = malloc(strlen(newopt) + 1);
		strcpy(opt_ptr[optarrlen], newopt);
		opt_ptr[optarrlen + 1] = NULL;
		return opt_ptr;
	}

	while (*current_opt != NULL){
		if (strncmp(*current_opt, name, strlen(name) - 1) == 0) {
			free(*current_opt);
			*current_opt = malloc(strlen(newopt) + 1);
			strcpy(*current_opt, newopt);
			return opt_ptr;
		}
		current_opt += 1;
	}
	
	return NULL;
}


/**
 * function - brief discription
 * @arg1: Discription
 * @arg2: Discription
 *
 * Returns and brief discription
 *
 * Details of this function
 * ...
 * ...
 */
char *
getopt(char *buf, const char *name, char *opt_ptr[])
{
	int i, opt_index;
	int failure;
	char *current_opt;
	size_t name_len;

	opt_index = 0;
	current_opt = opt_ptr[opt_index];
	name_len = strlen(name);

	i = 0;
	while (current_opt != NULL) {
		failure = 0;
		while (i < name_len) {
			if (current_opt[i] != name[i]){
				failure = 1;
				break;
			} else {
				i += 1;
			}
		}

		if (failure == 1 || (failure == 0 && current_opt[i] != '=' )){
			opt_index += 1;
			current_opt = opt_ptr[opt_index];
			i = 0;
			continue;
		} else{
			strcpy(buf, current_opt + i + 1);
			return buf;
		}
	}
	return NULL;

}


/**
 * function - brief discription
 * @arg1: Discription
 * @arg2: Discription
 *
 * Returns and brief discription
 *
 * Details of this function
 * ...
 * ...
 */
void
echoerr(const char *app, const char *operate, const char *error)
{
	writeline("-", STDOUT_FD);
	writeline(app, STDOUT_FD);
	writeline(": ", STDOUT_FD);
	writeline(operate, STDOUT_FD);
	writeline(": ", STDOUT_FD);
	writeline(error, STDOUT_FD);
	writeline("\n", STDOUT_FD);

	return;
}
