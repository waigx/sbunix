/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Dongju Ok   <dongju@stonybrook.edu,
 *                                  yardbirds79@gmail.com>
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sbunix is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sbunix is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with sbunix.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/defs.h>
#include <sys/tarfs.h>
#include <sys/file_op.h>
#include <sys/tarfs_api.h>
#include <string.h>
#include <sys/debug.h>
#include <sys/sbunix.h>
#include <sys/sched.h>
#include <sys/managemem.h>
#include <sys/mem.h>
#include <sys/kio.h>


#define OFFSET_SIZE_TARFS                     (100 + 8 + 8 + 8)
#define BYTES_SIZE_TARFS                                     12


uint64_t get_file_size(struct posix_header_ustar *tarfs_header);
int64_t get_fd(task_t *cur_task, struct posix_header_ustar *header_addr, int flags);
int64_t check_dir(char *next_dir, char *parent_dir);


ssize_t sys_write(int fd, const void *buf, size_t count)
{
	task_t *cur_task;
	struct file_descript *fdt;
	uint64_t i = 0;
	char *buf_ptr = (char *)buf;

	cur_task = gp_current_task;
	fdt = (struct file_descript *)cur_task->fd[fd];

	if(fd != STANDARD_IO_OUT) {
		debug_print("write", "Invalid file discriptor.");
		return 0;
	}

	for(i = 0; i < count; i++)
		writechar(*(buf_ptr + i));

	fdt->ptr += i;
	return (ssize_t)i;
}


uint64_t sys_getdentry(uint64_t fd, uint64_t *buf, uint64_t max_buf_size)
{
	struct posix_header_ustar *tarfs_header;
	//int64_t fd = 0;
	task_t *cur_task;
	struct file_descript *fdt;

	//uint64_t start_header = 0;
	uint64_t offset = 0;
	uint64_t address = 0;
	uint64_t i = 0;
	int64_t ret = 0;
	char *dirtype = "5";

	struct dirent *dirent = (struct dirent *)buf;

#if DEBUG_FS
	debug_print("FS", "Start getdentry_tarfs \n");
#endif
	cur_task = gp_current_task;
	fdt = (struct file_descript *)cur_task->fd[fd];

	address = (uint64_t)fdt->header;

	if( strcmp(fdt->header->typeflag, dirtype) != 0){
		printf("sys_getdentry: it is not directory\n");
		return -1;
	} else {
		for(i=0; i <= fdt->ptr ; i++ ){

			tarfs_header = (struct posix_header_ustar *)address;

			offset = get_file_size(tarfs_header);

#if DEBUG_FS
			if(offset != 0)
				debug_print("FS", "Offset is %x\n",offset);
#endif
			address += sizeof(struct posix_header_ustar);
			address += (offset >> 9 << 9);

			if(offset % 512)
				address += 512;
			
			if (address > (uint64_t)&_binary_tarfs_end)
				address = (uint64_t)&_binary_tarfs_start;


			tarfs_header = (struct posix_header_ustar *)address;
			ret = check_dir(tarfs_header->name, fdt->header->name);
			if(ret == -2){
				// not directory, file
				i = i -1;
			}else if(ret == 0 && i == fdt->ptr){
				dirent->d_ino = (long)tarfs_header;
				dirent->d_off = (off_t)tarfs_header->size;
				dirent->d_reclen = sizeof(struct dirent);
				copymem(dirent->d_name, tarfs_header->name + strlen(fdt->header->name), 100 - strlen(fdt->header->name));
				fdt->ptr += 1;
				return dirent->d_reclen;
				//return (struct posix_header_ustar *)buf;
			}
		}
	}

	return 0;
}


uint64_t find_elf(const char *pathname, int flags)
{
	struct posix_header_ustar *tarfs_header;
	char fullpath_buf[MAX_CWD_LEN];
	uint64_t fd = 0;
	uint64_t offset = 0;
	uint64_t address = 0;

	if (pathname[0] == '/') {
		strcpy(fullpath_buf, pathname);
	} else {
		strcpy(fullpath_buf, g_cwd);
		strcpy(fullpath_buf + strlen(g_cwd), pathname);
	}

	address = (uint64_t)&(_binary_tarfs_start);
	//printf("struct size = %x \n",sizeof(struct posix_header_ustar));
	while(1) {
		tarfs_header = (struct posix_header_ustar *)address;
#if DEBUG_ELF
		debug_print("ELF", "ELF name = %s %x\n", tarfs_header->name,tarfs_header);
#endif
		if(strcmp(tarfs_header->name, fullpath_buf + 1) == 0) {
#if DEBUG_ELF
			debug_print("ELF", "Finding %s elf\n", fullpath_buf + 1);
#endif
			fd = (uint64_t)(tarfs_header) + sizeof(struct posix_header_ustar);
			return fd;
			break;
		}

		if(tarfs_header->name[0] == '\0') {
			printf("there is not exist elf\n");
			return -1;
		}

		offset = get_file_size(tarfs_header);

		address += sizeof(struct posix_header_ustar);
		address += (offset >> 9 << 9);

		if(offset % 512)
			address += 512;
	}

	return -1;
}


int open_tarfs(const char *pathname, int flags)
{
	struct posix_header_ustar *tarfs_header;
	int64_t fd = 0;
	uint64_t start_header = 0;
	uint64_t offset = 0;
	uint64_t address = 0;

	address = (uint64_t)&(_binary_tarfs_start);

	while(address < (uint64_t)&_binary_tarfs_end)
	{
		tarfs_header = (struct posix_header_ustar *)address;
		if (strlen(pathname) == 0) {
			start_header = (uint64_t)(&_binary_tarfs_end);
			copymem((void *)start_header, (void *)tarfs_header, sizeof(struct posix_header_ustar));
			*((char *)start_header) = '\0';
			break;
		}
		if(strcmp(tarfs_header->name, pathname) == 0) {
#if DEBUG_FS
			debug_print("FS", "Found %s file\n", pathname);
#endif
			start_header = (uint64_t)tarfs_header;
			break;
		}

		if(tarfs_header->name[0] == '\0') {
#if DEBUG_FS
			debug_print("FS", "File %s does not exist.\n", pathname);
#endif
			return -1;
		}

		offset = get_file_size(tarfs_header);

#if DEBUG_FS
		if(offset != 0)
			debug_print("FS", "Offset is %x\n",offset);
#endif
		address += sizeof(struct posix_header_ustar);
		address += (offset >> 9 << 9);

		if(offset % 512)
			address += 512;

	}
	if (address >= (uint64_t)&_binary_tarfs_end + sizeof(struct posix_header_ustar))
		return -1;

	// get file descript
	fd = get_fd(gp_current_task, (struct posix_header_ustar *)start_header, flags); 
#if DEBUG_FS
	debug_print("FS", "open_tarfs: start_header = %x\n", start_header);
#endif
	return fd;
}


int64_t get_fd(task_t *cur_task, struct posix_header_ustar *header_addr, int flags)
{

	uint64_t i = 0;
	task_t *task = (task_t *)cur_task;

	for(i = STANDARD_IO_ERROR + 1; i < MAX_OPEN_FILE_DESCRIPT; i++) {
		if(task->fd[i] == NULL ) {
			task->fd[i] = g_fdt_start + i;
			task->fd[i]->header = header_addr;
			task->fd[i]->ptr = 0;
			task->fd[i]->mode = 0;
			break;
		} 
	}

	if(i >= MAX_OPEN_FILE_DESCRIPT)
	{
		printf("fail to alloc fd because fd is full\n");
		return -1;
	}
	return (int64_t)i;  
}


uint64_t get_file_size(struct posix_header_ustar *tarfs_header)
{
	char *size_aray;
	uint64_t size = 0;
	int64_t i = 0;
	uint64_t j = 1;
	size_aray = (char *)&(tarfs_header->size[0]);

	for(i= BYTES_SIZE_TARFS -1 ; i >= 0; i--)
	{
		if(size_aray[i] >= '0')
		{
			size += ((size_aray[i]- '0') * (j));
			//printf("size_arary - '0' = %d\n", size_aray[i] - '0');
			//printf("10 ^ j= %d\n", 10 ^ j);
			j *= 8;
		}
	}

	return size;
}


ssize_t read_tarfs(int fd, void *buf, size_t count)
{
	struct file_descript *fdt;
	char *filetype = "0";
	char *dirtype = "5";
	task_t *cur_task;
	char *start;
	uint64_t size = 0;
	uint64_t i = 0;
	uint64_t ptr= 0;

#if DEBUG_FS
	debug_print("FS", "Start read tarfs\n");
#endif

	cur_task = gp_current_task;
	fdt = (struct file_descript *)cur_task->fd[fd]; 

	if(fdt == NULL) {
		printf("fd is NULL\n");
		return -1;
	}

#if DEBUG_FS
	debug_print("FS", "read tarfs: fdt->header: %x\n", fdt->header);
#endif

	if(strcmp(fdt->header->typeflag, dirtype) == 0) {
		printf("Trying to read a directory, aborted.\n");
		return -1;
	} else if(strcmp(fdt->header->typeflag, filetype) == 0) {
		ptr = fdt->ptr;
		start = (char *)((uint64_t)fdt->header + sizeof(struct posix_header_ustar) + ptr);
		size = get_file_size(fdt->header);
		for(i = 0; i < count; i++) {
			if( ptr + i >= size)
				break;
			*((char *)buf + i) = *(start + i);
		}
	}

	fdt->ptr += i;
	return i;
}


off_t lseek_tarfs(int fildes, off_t offset, int whence)
{
	task_t *cur_task;
	struct file_descript *fdt;

	cur_task = gp_current_task;
	fdt = (struct file_descript *)cur_task->fd[fildes];

	if(whence == SEEK_SET) {
		fdt->ptr = offset;
	} else if(whence == SEEK_CUR) {
		fdt->ptr += offset;
	} else if(whence == SEEK_END) {
		/* TODO how to do that  The offset is set to the size of the file plus offset bytes.*/
		fdt->ptr += offset;
	}

	return fdt->ptr;
}


int close_tarfs(int fd)
{
	//struct file_descript *fdt;
	task_t *cur_task;

#if DEBUG_FS
	debug_print("FS", "start close tarfs \n");
#endif

	cur_task = gp_current_task;
	if(fd <= STANDARD_IO_ERROR) {
		printf("Invalid File Discriptor: %d\n", fd);
		return -1;
	}
	if(cur_task->fd[fd] == NULL) {
		printf("close_tarfs: invalid fd\n");
		return -1;
	} else
		cur_task->fd[fd] = NULL;

	return 0;
}


int64_t check_dir(char *next_dir, char *parent_dir)
{
	if (strlen(parent_dir) + strlen(next_dir) == 0)
		return -1;
	if (*parent_dir != '\0')
		while(*parent_dir++  == *next_dir++);

	if( *parent_dir != '\0')
		return -1;
	while(*next_dir != '\0'){
		next_dir++;
		if(*next_dir == '/')
			break;
	}
	next_dir++;
	if(*next_dir == '\0')
		return 0;
	else
		return -2;
}

//struct dirent *readdir_tarfs(void *dir)
struct posix_header_ustar *readdir_tarfs(int fd, uint64_t buf) 
{
	struct posix_header_ustar *tarfs_header;
	//int64_t fd = 0;
	task_t *cur_task;
	struct file_descript *fdt;

	//uint64_t start_header = 0;
	uint64_t offset = 0;
	uint64_t address = 0;
	uint64_t i = 0;
	int64_t ret = 0;
	char *dirtype = "5";

#if DEBUG_FS
	debug_print("FS", "Start read dir in tarfs \n");
#endif

	cur_task = gp_current_task;
	fdt = (struct file_descript *)cur_task->fd[fd];

	address = (uint64_t)fdt->header;

	if(strcmp(fdt->header->typeflag, dirtype) != 0){
		printf("Trying to read a non-directory entry, aborted.\n");
	} else {
		for(i=0; i <= fdt->ptr ; i++ ){

			tarfs_header = (struct posix_header_ustar *)address;
			offset = get_file_size(tarfs_header);

#if DEBUG_FS
			if(offset != 0)
				debug_print("FS", "Offset is %x\n",offset);
#endif

			address += sizeof(struct posix_header_ustar);
			address += (offset >> 9 << 9);
			if(offset % 512)
				address += 512;
			tarfs_header = (struct posix_header_ustar *)address;
			ret = check_dir(tarfs_header->name, fdt->header->name);
			if(ret == -2  ){
				i = i -1;
			}else if(ret == 0 ){
				if(i == fdt->ptr){
					copymem((struct posix_header_ustar *)buf, tarfs_header, sizeof(struct posix_header_ustar));
					fdt->ptr += 1;
					return (struct posix_header_ustar *)buf;
				}
			}
		}
	}
	return NULL;
}

int closedir_tarfs(void *dir)
{
	return 0;
}
