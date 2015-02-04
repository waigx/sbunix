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

#define SHELL_NAME "SBUsh"


char * parse_ps1(char *, char *, char **);
char * getabbrcwd(char *, char **);
char * gethostname(char *);
char * getabbrhostname(char *);



int 
main(int argc, char *argv[], char *envp[]) 
{
	return 0;
}



char *
parse_ps1(char *buf, char *ps1, char *opt_ptr[])
{
	int i;
	size_t ps1_len = strlen(ps1);
	size_t buf_len = 0;
	char temp_info[PS_MAX_LEN];

	for (i = 0; i < ps1_len; i++){
		if (ps1[i] == '\\') {
			i += 1;
			switch (ps1[i]) {
			case 'h':
				getabbrhostname(temp_info);
				break;
			case 'H':
				gethostname(temp_info);
				break;
			case 'u':
				getopt(temp_info, "USER", opt_ptr);
				break;
			case 'w':
				getabbrcwd(temp_info, opt_ptr);
				break;
			case 'W':
				getcwd(temp_info, PS_MAX_LEN);
				break;
			case 's':
				strcpy(temp_info, SHELL_NAME);
				break;
			default:
				return NULL;
				break;
			}
		} else {
			temp_info[0] = ps1[i];
			temp_info[1] = '\0';
		}

		if (strlen(temp_info) + buf_len > PS_MAX_LEN - 4) {
			strcpy(buf + buf_len, "...");
			buf_len += 3;
			break;
		} else {
			strcpy(buf + buf_len, temp_info);
			buf_len += strlen(temp_info);
		}
	}

	return buf;
}


char *
getabbrcwd(char *buf, char *opt_ptr[])
{
	char home[PS_MAX_LEN];
	char cwd_ptr[PS_MAX_LEN];
	size_t home_len;

	getopt(home, "HOME", opt_ptr);
	home_len = strlen(home);

	getcwd(cwd_ptr, PS_MAX_LEN);

	if (home_len > strlen(cwd_ptr)) {
		return strcpy(buf, cwd_ptr);
	}

	if ( strncmp(home, cwd_ptr, home_len) == 0 ) {
		buf[0] = '~';
		strcpy(buf + 1, cwd_ptr + home_len);
		return buf;
	}
	
	return strcpy(buf, cwd_ptr);

}


char *
gethostname(char *buf)
{
	int hostname_fd;
	
	if ( (hostname_fd = open(HOSTNAME_FILE, O_RDONLY)) < 0 )
		return NULL;

	readline(buf, hostname_fd);
	close(hostname_fd);

	return buf;

}


char *
getabbrhostname(char *buf)
{
	char *dot_ptr;

	if (gethostname(buf) == NULL)
		return NULL;

	if ((dot_ptr = strstr(buf, ".")) == NULL)
		return buf;
	*dot_ptr = '\0';

	return buf;

}
