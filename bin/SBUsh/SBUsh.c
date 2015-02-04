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
#define ROOT_PATH "/home/waigx/Documents/CSE506/SBUsh/rootfs" // directory without last "/";
#define CONFIG_FILE {"/etc/SBUsh.SBUshrc", "~/.SBUshrc", ""}
#define DEFAULT_PATH "/bin"
#define DEFAULT_PS1 "\\s:$"


int execute(char *);
char * parse_dir(char *, char *);

int _execute_1(char *);

void _initshell();

char * _parse_ps1(char *, char *);
char * _getabbrcwd(char *);
char * _gethostname(char *);
char * _getabbrhostname(char *);


char **g_opt_ptr;
char g_ps1[PS_MAX_LEN];
char g_path[PS_MAX_LEN];


int 
main(int argc, char *argv[], char *envp[]) 
{
	return 0;
}


int
execute(char *line)
{
	printf("%s\n", line);
	return 0;
}


// Current version of SBUsh does not yet support quotation mark("") and escape sequences space(\ )
int
_execute_1(char *line)
{
	char **raw_argv = splitstr(line, " ");
	char **argv = malloc(sizeof(char *) * (1 + lenstrarr(raw_argv)));
	char **raw_argv_ptr = raw_argv;
	char **argv_ptr = argv;
	char exe_path[NAME_MAX + 1];

	while (*raw_argv_ptr != NULL) {
		if (strlen(*raw_argv_ptr) != 0)
			cpynstrarr(argv_ptr, raw_argv_ptr, 1);
		raw_argv_ptr += 1;
		argv_ptr += 1;
	}

	parse_dir(exe_path, argv[0]);
	if (execve(exe_path, argv, g_opt_ptr) < 0) {
		printf("%s: %s: execve function fault\n", SHELL_NAME, exe_path);
	}

	free(raw_argv);
	free(argv);
	return 0;
}


void
_initshell()
{
	char **basenconfig = malloc(sizeof(char *) * 3);
	char config_path[PS_MAX_LEN];
	char commandline[MAXLINE];
	char *config_lst[] = CONFIG_FILE;
	int config_lst_i = 0;
	int config_fd;

	strcpy(g_ps1, DEFAULT_PS1);
	strcpy(g_path, ROOT_PATH DEFAULT_PATH);

	basenconfig[0] = malloc(strlen(ROOT_PATH)+1);
	strcpy(basenconfig[0], ROOT_PATH);
	basenconfig[1] = malloc(PS_MAX_LEN);
	basenconfig[2] = NULL;
	while (strlen(config_lst[config_lst_i]) != 0) {
		parse_dir(basenconfig[1], config_lst[config_lst_i]);
		joinstrlst(config_path, basenconfig, "");
		
		if ((config_fd = open(config_path, O_RDONLY)) < 0) {
			config_lst_i += 1;
			continue;
		}

		while (readline(commandline, config_fd) != NULL)
			execute(commandline);

		close(config_fd);
		config_lst_i += 1;
	}

	freestrarr(basenconfig);
	return;

}


char *
parse_dir(char *buf, char *cd_arg)
{
	char **cwd_lst;
	char **cd_lst;
	char **path_lst = malloc(sizeof(char *) * DIR_MAX_DEPTH);
	char cwd[PS_MAX_LEN];
	int cwd_lst_index = 0;
	int cd_lst_index = 0;
	int path_lst_index = 0;

	if (strlen(cd_arg) == 0)
		return getopt(buf, "HOME", g_opt_ptr);
	
	cd_lst = splitstr(cd_arg, "/");


	if (strcmp(cd_lst[0], "~") == 0) {
		getopt(cwd, "HOME", g_opt_ptr);
		cd_lst_index += 1;
	} else {
		getcwd(cwd, PS_MAX_LEN);
	}

	cwd_lst = splitstr(cwd, "/");

	if (strlen(cd_lst[0]) != 0) {
		for (; cwd_lst[cwd_lst_index] != NULL; cwd_lst_index++) {
			path_lst[path_lst_index] = malloc(strlen(cwd_lst[cwd_lst_index]) + 1);
			strcpy(path_lst[path_lst_index], cwd_lst[cwd_lst_index]);
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
		path_lst[path_lst_index] = malloc(strlen(cd_lst[cd_lst_index]) + 1);
		strcpy(path_lst[path_lst_index], cd_lst[cd_lst_index]);
		path_lst_index += 1;
	} 

	path_lst[path_lst_index] = NULL;
	joinstrlst(buf, path_lst, "/");

	freestrarr(cwd_lst);
	freestrarr(cd_lst);
	freestrarr(path_lst);

	return buf;
}


char *
_parse_ps1(char *buf, char *ps1)
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
				_getabbrhostname(temp_info);
				break;
			case 'H':
				_gethostname(temp_info);
				break;
			case 'u':
				getopt(temp_info, "USER", g_opt_ptr);
				break;
			case 'w':
				_getabbrcwd(temp_info);
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
_getabbrcwd(char *buf)
{
	char home[PS_MAX_LEN];
	char cwd_ptr[PS_MAX_LEN];
	size_t home_len;

	getopt(home, "HOME", g_opt_ptr);
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
_gethostname(char *buf)
{
	int hostname_fd;
	
	if ( (hostname_fd = open(HOSTNAME_FILE, O_RDONLY)) < 0 )
		return NULL;

	readline(buf, hostname_fd);
	close(hostname_fd);

	return buf;

}


char *
_getabbrhostname(char *buf)
{
	char *dot_ptr;

	if (_gethostname(buf) == NULL)
		return NULL;

	if ((dot_ptr = strstr(buf, ".")) == NULL)
		return buf;

	*dot_ptr = '\0';

	return buf;

}


int
_cd(int argc, char *argv[])
{
	char abs_path[PS_MAX_LEN];
	int is_success;
	
	if (argc == 1) {
		parse_dir(abs_path, "");
	} else {
		parse_dir(abs_path, argv[1]);
	}
	if ( (is_success = chdir(abs_path))<0 ) {
		printf("%s: %s: No such directory\n", SHELL_NAME, abs_path);
		return -1;
	}
	return 0;
}
