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
#include "libstr.h"
#include "libio.h"
#include "libcommon.h"


char * parse_dir(char *, char *, char **);


int
main(int argc, char *argv[], char *envp[])
{
	return 0;
}


char *
parse_dir(char *buf, char *cd_arg, char *opt_ptr[])
{
	char **cwd_lst;
	char **cd_lst;
	char **path_lst = malloc(sizeof(char *) * DIR_MAX_DEPTH);
	char cwd[PS_MAX_LEN];
	int cwd_lst_index = 0;
	int cd_lst_index = 0;
	int path_lst_index = 0;

	if (strlen(cd_arg) == 0)
		return getopt(buf, "HOME", opt_ptr);
	
	cd_lst = splitstr(cd_arg, "/");

	if (strcmp(cd_lst[0], "~") == 0) {
		getopt(cwd, "HOME", opt_ptr);
		cd_lst_index += 1;
	} else {
		getcwd(cwd, PS_MAX_LEN);
	}

	cwd_lst = splitstr(cwd, "/");

	if (strlen(cd_lst[0]) != 0) {
		for (; cwd_lst[cwd_lst_index] != NULL; cwd_lst_index++) {
			path_lst[path_lst_index] = cwd_lst[cwd_lst_index];
			path_lst_index += 1;
		}
	}

	for (; cd_lst[cd_lst_index] != NULL; cd_lst_index++) {
		if (strcmp(cd_lst[cd_lst_index], ".") == 0)
			continue;
		if (strcmp(cd_lst[cd_lst_index], "..") == 0) {
			if (path_lst_index != 0)
				path_lst_index -= 1;
			continue;
		}
		path_lst[path_lst_index] = cd_lst[cd_lst_index];
		path_lst_index += 1;
	} 

	path_lst[path_lst_index] = NULL;
	joinstrlst(buf, path_lst, "/");

	freestrarr(cwd_lst);
	freestrarr(cd_lst);
	freestrarr(path_lst);

	return buf;
}
