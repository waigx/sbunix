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
#define MAXLINE			1024
#define HOSTNAME_FILE	"/proc/sys/kernel/hostname"

char * readline(char *, int); 
char * getopt(char *, const char *, char **);
char ** splitstr(char **, char *, const char *);
char * joinstrlst(char *, char **, const char *);

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
readline(char *buf, int fd)
{
	int n;
	int buf_index = 0;

	while ((n = read(fd, buf + buf_index, 1)) != 0) {
		if ( buf[buf_index] == 0 || buf[buf_index] == 10 || buf[buf_index] == 13) {
			buf[buf_index] = '\0';
			return buf;
		}
		buf_index += 1;
	}

	return NULL;
}


char *
parse_dir(char *buf, char *cd_arg, char *opt_ptr[])
{
	if (strlen(cd_arg) == 0)
		return getopt(buf, "HOME", opt_ptr);

	return NULL;
}


char **
splitstr(char **buf_ptr, char *haystack, const char *needle)
{
	char **current_buf_ptr = buf_ptr;
	char *last_ptr = haystack;
	char *current_ptr = haystack;
	size_t needle_len = strlen(needle);

	while (1) {
		current_ptr = strstr(last_ptr, needle);
		if (current_ptr == NULL) {
			strcpy(*current_buf_ptr, last_ptr);
			break;
		}
		strncpy(*current_buf_ptr, last_ptr, current_ptr - last_ptr);
		(*current_buf_ptr)[current_ptr - last_ptr] = '\0';
		last_ptr = current_ptr + needle_len;
		current_buf_ptr += 1;
	}
	current_buf_ptr += 1;
	*current_buf_ptr = NULL;

	return buf_ptr;

}


char *
joinstrlst(char *buf, char **str_ptr, const char *salt)
{
	char *buf_ptr = buf;
	int salt_len = strlen(salt);

	if (str_ptr == NULL) {
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
