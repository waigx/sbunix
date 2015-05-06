/*
 *
 *  This file is part of sblibc.
 *        sblibc is a simple libc implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sblibc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sblibc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sblibc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <stdio.h>
#include <libio.h>
#include <libsys.h>
#include <string.h>
#include <const.h>
#include <syscall.h>
#include <sys/syscall.h>


#define BUFSIZE                       128
#define PID_POS                         1
#define PARENT_PID_POS                  8
#define STATUS_POS                     15
#define NAME_POS                       28


void _tracetask(pid_t pid)
{
	int i;
	char task_buf[sizeof(task_t)];
	char buf[BUFSIZE];
	i = syscall_2(SYS_gettask, (uint64_t)task_buf, pid);
	task_t *target_task = (task_t *)task_buf;

	if (i < 0)
		return;
	for (i = 0; i < BUFSIZE; i++)
		buf[i] = ' ';

	i = sprintf(buf + PID_POS, "%d", target_task->pid);
	buf[PID_POS + i] = ' ';
	i = sprintf(buf + PARENT_PID_POS, "%d", target_task->parent);
	buf[PARENT_PID_POS + i] = ' ';
	switch (target_task->status){
		case PROCESS_NEW:
			i = sprintf(buf + STATUS_POS, "new");
			break;
		case PROCESS_READY:
			i = sprintf(buf + STATUS_POS, "ready");
			break;
		case PROCESS_RUNNING:
			i = sprintf(buf + STATUS_POS, "running");
			break;
		case PROCESS_WAITING:
			i = sprintf(buf + STATUS_POS, "waiting");
			break;
		case PROCESS_TERMINATED:
			i = sprintf(buf + STATUS_POS, "terminated");
			break;
		default:
			i = sprintf(buf + STATUS_POS, "unknown");
	}
	buf[STATUS_POS + i] = ' ';
	i = sprintf(buf + NAME_POS, "%s\n", target_task->name);
	writeline(buf, STDOUT_FD);
	return;
}


int main(int argc, char* argv[], char* envp[]) {
	int i;
	pid_t pid;
	char buf[BUFSIZE];

	for (i = 0; i < BUFSIZE; i++)
		buf[i] = ' ';

	i = sprintf(buf + PID_POS, "PID");
	buf[PID_POS + i] = ' ';
	i = sprintf(buf + PARENT_PID_POS, "PPID");
	buf[PARENT_PID_POS + i] = ' ';
	i = sprintf(buf + STATUS_POS, "Status");
	buf[STATUS_POS + i] = ' ';
	i = sprintf(buf + NAME_POS, "File\n");
	writeline(buf, STDOUT_FD);
	writeline("\n", STDOUT_FD);

	for (pid = 1; pid < 5; pid++) {
		_tracetask(pid);
	}
	while (1);
	return 0;
}
