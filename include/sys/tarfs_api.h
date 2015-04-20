#ifndef _TARFS_API_H
#define _TARFS_API_H

#include <sys/defs.h>
#include <stdlib.h>

struct file_descript
{
	struct posix_header_ustar *header;
	uint64_t ptr;
	uint64_t mode;
};


uint64_t find_elf(const char *pathname, int flags);


int open_tarfs(const char *pathname, int flags);

ssize_t read_tarfs(int fd, void *buf, size_t count);

ssize_t write_tarfs(int fd, const void *buf, size_t count);

off_t lseek_tarfs(int fildes, off_t offset, int whence);

int close_tarfs(int fd);

void *opendir_tarfs(const char *name);

struct dirent *readdir_tarfs(void *dir);

int closedir_tarfs(void *dir);

#endif
