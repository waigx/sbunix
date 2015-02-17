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
#include <libstr.h>
#include <libio.h>
#include <libcommon.h>
#include <const.h>


#define SHELL_NAME "SBUsh"

/**
 * This is the default root path of SBUsh for testing purpose.
 * 
 * ROOT_PATH append before every path-related operation in user
 * interface.
 * Note the directory without last "/";
 */
#define ROOT_PATH "" 

/**
 * This constant gives the config files' path, later config
 * will overwrite the prevous one, path will be appended after
 * ROOT_PATH
 */
#define CONFIG_FILE {"/etc/SBUsh.SBUshrc", "~/.SBUshrc", ""}

#define DEFAULT_PS1 "\\u@\\h$\\w\\b"


int execute(char *);


char * parse_dir(char *, char *);

int _cd(int, char **);
int _export(int, char **);
int _setenv(int, char **);
int _logout();
int _echo();

void _initshell();
void _update_ps1_path();
void _welcome_info();


int _execute_1(char *, int, int *);
char * _parse_ps1(char *, char *);
char * _parse_command_path(char *, char *);
char * _getabbrcwd(char *);
char * _gethostname(char *);
char * _getabbrhostname(char *);


char *g_opt_ptr[PS_MAX_LEN];
char g_ps1[PS_MAX_LEN];
char g_path[PS_MAX_LEN];
char g_root[PS_MAX_LEN];



int 
main(int argc, char *argv[], char *envp[]) 
{
	char buf[MAXLINE];
	int script_fd;
	int i;
	_update_ps1_path();
	cpystrarr(g_opt_ptr, envp);
	_initshell();

	if (argc > 1) {
		for (i = 1; i < argc; i++) {
			parse_dir(buf, argv[i]);
			if ((script_fd = open(buf, O_RDONLY)) < 0) {
				echoerr(SHELL_NAME, argv[i], "file does not exists");
				continue;
			}
			while (readline(buf, script_fd) != NULL) {
				if (buf[0] == '#')
					continue;
				execute(buf);
			}
			close(script_fd);
		}
		return 0;
	}

	_welcome_info();
	while (1){
		_parse_ps1(buf, g_ps1);
		writeline(buf, STDOUT_FD);
		readline(buf, STDIN_FD);
		execute(buf);
	}

	return 0;
}


/**
 * execute - execute a commandline
 * @line: The commandline
 *
 * returns value indicates whether execution success or not
 *
 * This function supports pipeline.
 */
int
execute(char *line)
{
	int pfd_out[2];
	int pfd_in;
	char **commands = splitstr(line, "|");
	char **command_ptr = commands;

	pfd_in = STDIN_FD;
	while (*command_ptr != NULL) {
		if (*(command_ptr + 1) == NULL) {
			pfd_out[1] = STDOUT_FD;
		} else {
			pipe(pfd_out);
		}
		_execute_1(*command_ptr, pfd_in, pfd_out);
		command_ptr += 1;
		pfd_in = pfd_out[0];
	}

	freestrarr(commands, FREE_ALL);
	return 0;
}


/**
 * _execute_1 - execute a commandline, and this function
 * @line: The commandline
 * @pfd_in: The input file discriptor
 * @pfd_out: The output file discriptor, result of dup2, or pfd_out[1] = STDOUT_FD
 *
 * Returns value indicates whether execution success or not
 *
 * Note: * This function DOES NOT support pipeline.
 *       * Current version of SBUsh does not yet support quotation mark("") and escape sequences space(\ )
 */
int
_execute_1(char *line, int pfd_in, int *pfd_out)
{
	char **raw_argv = splitstr(line, " ");
	char **argv = malloc(sizeof(char *) * (1 + lenstrarr(raw_argv)));
	char **raw_argv_ptr = raw_argv;
	char **argv_ptr = argv;
	char exe_path[NAME_MAX + 1];
	int child_pid;
	int child_status;
	int execute_status = 1;

	argv[0] = NULL;
	while (*raw_argv_ptr != NULL) {
		if (strlen(*raw_argv_ptr) != 0) {
			cpynstrarr(argv_ptr, raw_argv_ptr, 1);
			argv_ptr += 1;
		}
		raw_argv_ptr += 1;
	}

	if (lenstrarr(argv) == 0) {
		execute_status = 1;
		goto end;
	}

	/* Begin of recognize internal functions */ 
	if (strcmp(argv[0], "cd") == 0) {
		execute_status = _cd(lenstrarr(argv), argv);
		if (execute_status < 0)
			execute_status = 1;
		else
			execute_status = 0;
		goto end;
	}
	if (strcmp(argv[0], "logout") == 0 || strcmp(argv[0], "exit") == 0) {
		execute_status = _logout();
		if (execute_status < 0)
			execute_status = 1;
		else
			execute_status = 0;
		goto end;
	}
	if (strcmp(argv[0], "export") == 0) {
		_export(lenstrarr(argv), argv);
		goto end;
	}
	if (strcmp(argv[0], "setenv") == 0) {
		_setenv(lenstrarr(argv), argv);
		goto end;
	}

	/* Begin of envoke function in path*/
	if (_parse_command_path(exe_path, argv[0]) == NULL) {
		echoerr(SHELL_NAME, argv[0], "command not found");
		goto end;
	}

	child_pid = fork();
	if (child_pid == 0) {
		if (pfd_in != STDIN_FD) {
			dup2(pfd_in, STDIN_FD);
			close(pfd_in);
		}
		if (pfd_out[1] != STDOUT_FD) {
			close(pfd_out[0]);
			dup2(pfd_out[1], STDOUT_FD);
			close(pfd_out[1]);
		}
		if (execve(exe_path, argv, g_opt_ptr) < 0) {
			echoerr(SHELL_NAME, exe_path, "execve function fault");
			exit(EXIT_FAILURE);
		}
	} else if (child_pid < 0) {
		echoerr(SHELL_NAME, exe_path, "fork error");
		execute_status = 2;
	} else {
		if (pfd_in != STDIN_FD) {
			close(pfd_in);
		}
		if (pfd_out[1] != STDOUT_FD) {
			close(pfd_out[1]);
		}
		waitpid(-1, &child_status, 0);
		if (child_status == EXIT_SUCCESS) {
			execute_status = 0;
		} else {
			echoerr(SHELL_NAME, exe_path, "execute command error");
			execute_status = 2;
		}
	}

end:
	free(raw_argv);
	free(argv);
	return execute_status;
}


/**
 * _parse_command_path - Parse the full path of command (binary or script)
 * @buf: The result full path
 * @command: The command to be prased 
 *
 * Returns an char pointer, which point to the first char of @buf, If command 
 * is not found or is a directory, return NULL.
 *
 * If the command exists as a absolute path or relevant path, then return
 * the path. Or this function will search in the $PATH, return the first
 * found file. 
 */
char *
_parse_command_path(char *buf, char *command)
{
	char **path_lst;
	char **path_lst_ptr;
	char tempbuf[PS_MAX_LEN];
	char *tempbuf_ptr = tempbuf + strlen(g_root);

	if (pathtype(parse_dir(buf, command)) == PATH_TYPE_FIL) {
		return buf;
	}

	strcpy(tempbuf, g_root);
	
	path_lst = splitstr(g_path, ":");
	path_lst_ptr = path_lst;

	while (*path_lst_ptr != NULL) {
		strcpy(tempbuf_ptr, *path_lst_ptr);
		strcpy(tempbuf_ptr + strlen(*path_lst_ptr), "/");
		strcpy(tempbuf_ptr + strlen(*path_lst_ptr) + 1, command);
		if (pathtype(tempbuf) == PATH_TYPE_FIL) {
			strcpy(buf, tempbuf);
			freestrarr(path_lst, FREE_ALL);
			return buf;
		}
		path_lst_ptr += 1;
	}

	freestrarr(path_lst, FREE_ALL);
	return NULL;
}


/**
 * _initshell - initialize the shell
 *
 * set g_ps1, g_path and g_root as their default value then 
 * initialize the config in the order of CONFIG_FILE
 */
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
	strcpy(g_path, getopt(config_path, "PATH", g_opt_ptr));
	strcpy(g_root, ROOT_PATH);

	basenconfig[0] = malloc(strlen(g_root)+1);
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

	freestrarr(basenconfig, FREE_ALL);
	return;

}


/**
 * parse_dir - parse the absolute path based on working directory
 * @buf: Result buffer
 * @cd_arg: Inputed path of cd command
 *
 * Returns a pointer points to the first char of @buf
 *
 * This function is design for cd command style path parsing, 
 * regardless the node exists or not.
 */
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

	freestrarr(cwd_lst, FREE_ALL);
	freestrarr(cd_lst, FREE_ALL);
	freestrarr(path_lst, FREE_ALL);

	return buf;
}


/**
 * _parse_ps1 - parse the PS1 string
 * @buf: Result buffer
 * @ps1: PS1 string 
 *
 * Returns a pointer points to the first char of @buf, NULL
 * when escape sequence is not supported
 *
 * Limited support of PS1 escape sequences:
 *     * \b a space character
 *     * \h hostname before first dot
 *     * \H full hostname
 *     * \u user name
 *     * \w current working directory with replacement
 *          of home by '~'
 *     * \W full working directory
 *     * \s shell name (SHELL_NAME)
 */
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
			case 'b':
				strcpy(temp_info, " ");
				break;
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


/**
 * _update_ps1_path - updates g_path, g_root, g_ps1 by g_opt_ptr
 * 
 */
void
_update_ps1_path()
{
	char buf[PS_MAX_LEN];
	if (getopt(buf, "PS1", g_opt_ptr) != NULL) {
		strcpy(g_ps1, buf);
	}
	if (getopt(buf, "PATH", g_opt_ptr) != NULL) {
		strcpy(g_path, buf);
	}
	if (getopt(buf, "ROOT", g_opt_ptr) != NULL) {
		strcpy(g_root, buf);
	}
	return;
}


/**
 * _getabbrcwd - brief discription
 * @buf: Result buffer
 *
 * Returns a pointer points to the first char of @buf
 * 
 * The home folder based on environment variables
 */
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


/**
 * _gethostname - get the host name
 * @buf: Result buffer
 *
 * Returns a pointer points to the first char of @buf, NULL if failed
 *
 * The mechanism is copy HOSTNAME_FILE directly to buf
 */
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


/**
 * _gethostname - get the host name before first dot
 * @buf: Result buffer
 *
 * Returns a pointer points to the first char of @buf, NULL if failed
 *
 * The mechanism is copy HOSTNAME_FILE directly to buf
 */
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


/**
 * _cd - change directory
 * @argc: Arguments number
 * @argv: Arguments vector
 *
 * Returns an integer, 0 is success, -1 means failed
 */
int
_cd(int argc, char *argv[])
{
	char abs_path[PS_MAX_LEN];
	int path_type;
	int is_success;
	
	if (argc == 1) {
		parse_dir(abs_path, "");
	} else {
		parse_dir(abs_path, argv[1]);
	}

	path_type = pathtype(abs_path);

	if (path_type == PATH_TYPE_NON) {
		echoerr(SHELL_NAME, abs_path, "No such file or directory");
		return -1;
	}

	if (path_type == PATH_TYPE_FIL) {
		echoerr(SHELL_NAME, abs_path, "Not a directory");
		return -1;
	}

	if ((is_success = chdir(abs_path)) < 0) {
		echoerr(SHELL_NAME, abs_path, "Unknown error");
		return -1;
	}

	return 0;
}


/**
 * _logout - logout or exit SBUsh
 *
 * Returns -1 if not successful
 */
int
_logout()
{
	echoerr(SHELL_NAME, "exit", "Now closing SBUsh, see you.");
	freestrarr(g_opt_ptr, FREE_LEAF);
	exit(EXIT_SUCCESS);
	return -1;
}


/**
 * _echo - TODO
 * @argc: Arguments number
 * @argv: Arguments vector
 *
 * Returns and brief discription
 *
 * Details of this function
 * ...
 * ...
 */
int
_echo(int argc, char *argv[])
{
	
	return 0;
}


/**
 * _setenv - set environment variables
 * @argc: Arguments number
 * @argv: Arguments vector
 *
 * Returns 0
 *
 * This function will set environment variables and update global
 * variables, the usage should like:
 *     $ setenv NAME option
 * Currently not support $NAME notation in option
 */
int
_setenv(int argc, char *argv[])
{
	if (argc < 3) {
		echoerr(SHELL_NAME, "setenv", "missing arguments");
		return 0;
	}
	setopt(argv[2], argv[1], g_opt_ptr);
	_update_ps1_path();
	return 0;
}


/**
 * _export - set environment variables
 * @argc: Arguments number
 * @argv: Arguments vector
 *
 * Returns 0
 *
 * This function will set environment variables and update global
 * variables, the usage should like:
 *     $ export NAME=option
 * Currently not support $NAME notation in option
 */
int
_export(int argc, char *argv[])
{
	char **new_opt;
	if (argc < 2) {
		echoerr(SHELL_NAME, "export", "missing arguments");
		return 0;
	}

	new_opt = splitstr(argv[1], "=");
	setopt(new_opt[1], new_opt[0], g_opt_ptr);
	freestrarr(new_opt, FREE_ALL);

	_update_ps1_path();
	return 0;
}


/**
 * _welcome_info - write copyright info to STDOUT
 */
void
_welcome_info()
{
	writeline("    ____  ____  _   _     _     \n", STDOUT_FD);
	writeline("   / ___|| __ )| | | |___| |__  \n", STDOUT_FD);
	writeline("   \\___ \\|  _ \\| | | / __| '_ \\ \n", STDOUT_FD);
	writeline("    ___) | |_) | |_| \\__ | | | |\n", STDOUT_FD);
	writeline("   |____/|____/ \\___/|___|_| |_|\n", STDOUT_FD);

	writeline("\nWelcome to SBUsh.\n", STDOUT_FD);

	writeline("\nCopyright (C) 2015, Yigong Wang\n\n", STDOUT_FD);
	writeline("This program comes with ABSOLUTELY NO WARRANTY; for details see LICENSE.\n", STDOUT_FD);
	writeline("This is free software, and you are welcome to redistribute it\n", STDOUT_FD);
	writeline("under certain conditions; see LICENSE for details.\n\n", STDOUT_FD);
}
