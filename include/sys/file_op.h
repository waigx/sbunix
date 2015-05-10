#ifndef _FILE_OP_H
#define _FILE_OP_H

#include <sys/defs.h>

uint64_t sys_open(const char *pathname, int flags);
uint64_t sys_chdir(const char *path);
uint64_t sys_getcwd(char *buf, size_t size);

ssize_t sys_read(int fd, void *buf, size_t count);
uint64_t sys_getdentry(uint64_t fd, uint64_t *buf, uint64_t max_buf_size);
ssize_t sys_write(int fd, const void *buf, size_t count);

#endif
