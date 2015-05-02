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
#include <sys/tarfs_api.h>
#include <string.h>
//#include <syscall.h>
#include <sys/sbunix.h>
#include <sys/sched/sched.h>
#include <sys/managemem.h>


#define	OFFSET_SIZE_TARFS	100 + 8 + 8 + 8
#define BYTES_SIZE_TARFS	12

uint64_t get_file_size(struct posix_header_ustar *tarfs_header);
int64_t get_fd(struct task_t *cur_task, struct posix_header_ustar *header_addr, int flags);
struct file_descript* alloc_fd_buf(void);


uint64_t find_elf(const char *pathname, int flags)
{
        struct posix_header_ustar *tarfs_header;
        //uint64_t i = 0;
        uint64_t fd = 0;
        uint64_t offset = 0;
        uint64_t address = 0;

        address = (uint64_t)&(_binary_tarfs_start);
	//printf("struct size = %x \n",sizeof(struct posix_header_ustar));
        while(1)
        {
                tarfs_header = (struct posix_header_ustar *)address;
                printf("elf name = %s %x\n", tarfs_header->name,tarfs_header);
                if(strcmp(tarfs_header->name, pathname) == 0)
                {
                        printf("finding %s elf\n",pathname);
                        fd = (uint64_t)(tarfs_header) + sizeof(struct posix_header_ustar);
                        return fd;
                        break;
                }

                if(tarfs_header->name[0] == '\0')
                {
                        printf("there is not exist %s elf\n");
                        return -1;
                }
                
		offset = get_file_size(tarfs_header);

                if(offset != 0)
                        printf("offset = %x\n",offset);
                address += sizeof(struct posix_header_ustar);
                address += ((((offset ) /512 ) /*+ 1 */) * 512);

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
	
	while(1)
	{
		tarfs_header = (struct posix_header_ustar *)address;
		if(strcmp(tarfs_header->name, pathname) == 0)
		{
			printf("finding %s file\n",pathname);
			//fd = (uint64_t)&tarfs_header;
			start_header = (uint64_t)&tarfs_header;
			//return fd;
			break;
		}
		
		if(tarfs_header->name[0] == '\0')
		{
			printf("there is not exist %s file\n");
			return -1;
		}

		offset = get_file_size(tarfs_header);

		if(offset != 0)
			printf("offset = %x\n",offset);
		address += sizeof(struct posix_header_ustar);	
		address += ((((offset ) /512 ) /*+ 1 */) * 512);
 
		if(offset % 512)
			address += 512;
	}

	// get file descript
	fd = get_fd((struct task_t *)get_current_task(), (struct posix_header_ustar *)start_header, flags); 
	
	return fd;
}

#if(1)
int64_t get_fd(struct task_t *cur_task, struct posix_header_ustar *header_addr, int flags)
{

	uint64_t i = 0;
	struct task_t *task = cur_task;
	//uint64_t *fd_struct_addr;

	for(i = STANDARD_IO_ERROR + 1; i < MAX_OPEN_FILE_DESCRIPT; i++)
	{
		if(task->fd[i] == NULL )
		{
			// TODO need t modify to use malloc about allocating fd struct //
			//	fd_struct_addr = (uint64_t *)allocframe(task->pid);  
			//task->fd[i] = (struct file_descript *)fd_struct_addr;
			task->fd[i] = alloc_fd_buf();
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
#endif

/* temp fd mememory by dongju */
char temp_fd_buf[1024];
uint64_t g_fd_index = 0;
struct file_descript* alloc_fd_buf(void)
{
	uint64_t *fd_addr;
	fd_addr = (uint64_t *)(temp_fd_buf + g_fd_index * sizeof(struct file_descript)); 
	g_fd_index++;
	return (struct file_descript *)fd_addr;

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
#if(1)
	struct file_descript *fdt;
	char *filetype = "0";
	char *dirtype = "5";
	sturct task_t *cur_task;

	cur_task = get_current_task();
	fdt = cur_task->fd[fd]; 
	

	if(strcmp(fdt->header->typeflag, dirtype) == 0)
	{
		printf("it is directory, cannot read it\n");
	}
	else if(strcmp(fdt->header->typeflag, filetype) == 0)
	{
		


	}
	
	
#endif	
	
	;
	return 0;
}


off_t lseek_tarfs(int fildes, off_t offset, int whence)
{
	;
	return 0;
}

int close_tarfs(int fd)
{
	;
	return 0;
}

void *opendir_tarfs(const char *name)
{

#if(0)
	struct posix_header_ustar *tarfs_header;
        //int64_t fd = 0;
        uint64_t start_header = 0;
        uint64_t offset = 0;
        uint64_t address = 0;

        address = (uint64_t)&(_binary_tarfs_start);


	/* Need checking dir or file by dongju */
        while(1)
        {
                tarfs_header = (struct posix_header_ustar *)address;
                if(strcmp(tarfs_header->name, name) == 0)
                {
                        printf("finding %s file\n",name);
                        //fd = (uint64_t)&tarfs_header;
                        start_header = (uint64_t)&tarfs_header;
			printf("%x \n", start_header);
                        //return fd;
                        break;
                }

                if(tarfs_header->name[0] == '\0')
                {
                        printf("there is not exist %s file\n");
                        break;
			//return -1;
                }

                offset = get_file_size(tarfs_header);

                if(offset != 0)
                        printf("offset = %x\n",offset);
                address += sizeof(struct posix_header_ustar);
                address += ((((offset ) /512 ) /*+ 1 */) * 512);

                if(offset % 512)
                        address += 512;
        }

        // get file descript
        //fd = get_fd((struct task_t *)get_current_task(), (struct posix_header_ustar *)start_header, flags);

        //return (d *)fd;
#endif
	;
	return 0;

}

struct dirent *readdir_tarfs(void *dir)
{


	
	return 0;
}

int closedir_tarfs(void *dir)
{
	;
	return 0;
}

