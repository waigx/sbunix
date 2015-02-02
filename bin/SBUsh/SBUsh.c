/*
 *
 *  This file is part of SBUsh.
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

#define PS_MAX_LEN		512

char * getopt(const char *, char **);
char * parse_ps1(char *, char **);
char * getabbrcwd(char **);

int
main(int argc, char * argv[], char * envp[])
{
	
	return 0;
}


char *
parse_ps1(char *ps1, char *opt_ptr[])
{
	int i;
	size_t ps1_len = strlen(ps1);
	size_t ps_len = 0;
	char temp_info[PS_MAX_LEN];
	char *ps = malloc(PS_MAX_LEN);


	for (i = 0; i < ps1_len; i++){
		if (ps1[i] == '\\') {
			i += 1;
			switch (ps1[i]) {
			case 'u':
				strcpy(temp_info, getopt("USER", opt_ptr));
				break;
			case 'w':
				strcpy(temp_info, getabbrcwd(opt_ptr));
				break;
			default:
				return NULL;
				break;
			}
		} else {
			temp_info[0] = ps1[i];
			temp_info[1] = '\0';
		}

		if (strlen(temp_info) + ps_len > PS_MAX_LEN - 4) {
			strcpy(ps + ps_len, "...");
			ps_len += 3;
			break;
		} else {
			strcpy(ps + ps_len, temp_info);
			ps_len += strlen(temp_info);
		}
	}

	return ps;
}


char *
getopt(const char *name, char *opt_ptr[])
{
	int i, opt_index;
	int failure;
	char *opt;
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
			opt = malloc(strlen(current_opt) - name_len);
			strcpy(opt, current_opt + i + 1);
			return opt;
		}
	}
	return NULL;

}


char *
getabbrcwd(char *opt_ptr[])
{
	char *home_ptr;
	char cwd_ptr[PS_MAX_LEN];
	char *acwd = malloc(PS_MAX_LEN);
	size_t home_len;

	home_ptr = getopt("HOME", opt_ptr);
	home_len = strlen(home_ptr);

	getcwd(cwd_ptr, PS_MAX_LEN);

	if (home_len > strlen(cwd_ptr)) {
		return strcpy(acwd, cwd_ptr);
	}

	if ( strncmp(home_ptr, cwd_ptr, home_len) == 0 ) {
		acwd[0] = '~';
		strcpy(acwd + 1, cwd_ptr + home_len);
		return acwd;
	}
	
	return strcpy(acwd, cwd_ptr);

}
