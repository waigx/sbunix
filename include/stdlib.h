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


#ifndef _STDLIB_H
#define _STDLIB_H

extern int errno;

int main(int argc, char* argv[], char* envp[]);
void exit(int status);

// memory
typedef unsigned long size_t;
void *malloc(size_t size);
void free(void *ptr);
int brk(void *end_data_segment);

// processes
typedef unsigned pid_t;
pid_t fork(void);
pid_t getpid(void);
pid_t getppid(void);
int execve(const char *filename, char *const argv[], char *const envp[]);
int pipe(int filedes[2]);
pid_t waitpid(pid_t pid, int *status, int options);
unsigned int sleep(unsigned int seconds);
unsigned int alarm(unsigned int seconds);

// paths
char *getcwd(char *buf, size_t size);
int chdir(const char *path);

// files
typedef long ssize_t;
enum { O_RDONLY = 0, O_WRONLY = 1, O_RDWR = 2, O_CREAT = 64 };
int open(const char *pathname, int flags);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
enum { SEEK_SET = 0, SEEK_CUR = 1, SEEK_END = 2 };
typedef int off_t;
off_t lseek(int fildes, off_t offset, int whence);
int close(int fd);
int dup(int oldfd);
int dup2(int oldfd, int newfd);

// directories
#define NAME_MAX 255
struct dirent
{
	long d_ino;
	off_t d_off;
	unsigned short d_reclen;
	char d_name [NAME_MAX+1];
};
void *opendir(const char *name);
int readdir(unsigned int fd, struct dirent *dirp, unsigned int count);
int closedir(void *dir);

// sockets
int socket(int domain, int type, int protocol);
struct sockaddr {
	unsigned short sa_family;
	char           sa_data[14];
};
typedef unsigned int socklen_t;
int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);


#endif
