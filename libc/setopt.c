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
#include <libcommon.h>
#include <const.h>
#include <stdlib.h>
#include <string.h>
#include <libstr.h>


/**
 * setopt - set option into an option list
 * @opt: The option to be set
 * @name: The option name 
 * @opt_ptr: The option list
 *
 * Returns @opt_ptr or NULL if error occurred 
 *
 * This function will alter @opt if the @name exited in @opt_ptr,
 * or appended a new opt as "@name=@opt" to @opt_ptr
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
