#ifndef _TARFS_API_H
#define _TARFS_API_H

#include <sys/defs.h>
#include <stdlib.h>

#define MAX_OPEN_FILE_DESCRIPT   1024
#define STANDARD_IO_IN              0
#define STANDARD_IO_OUT             1
#define STANDARD_IO_ERROR           2


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

//struct dirent *readdir_tarfs(void *dir);
struct posix_header_ustar *readdir_tarfs(int fd, uint64_t buf);

int closedir_tarfs(void *dir);

#endif
