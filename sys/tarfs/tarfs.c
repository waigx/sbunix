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
#include <sys/mem.h>
#include <sys/kio.h>

#define	OFFSET_SIZE_TARFS	(100 + 8 + 8 + 8)
#define BYTES_SIZE_TARFS	12

uint64_t get_file_size(struct posix_header_ustar *tarfs_header);
int64_t get_fd(struct task_t *cur_task, struct posix_header_ustar *header_addr, int flags);
struct file_descript* alloc_fd_buf(void);
int64_t check_dir(char *next_dir, char *parent_dir);

ssize_t sys_write(int fd, const void *buf, size_t count)
{
        task_t *cur_task;
        struct file_descript *fdt;
	uint64_t i = 0;
        cur_task = (task_t *)get_current_task();
        fdt = (struct file_descript *)cur_task->fd[fd];

	//printf("sys_write: starting \n");

        /* TODO sys_write is allowed only to STAND_OUT by dongju */
        if(fd != STANDARD_IO_OUT)
        {
                printf("sys_write: fd is not allowed");
                return 0;
        }

        rollscreen(1);
        //int printfat(10, 0, const char *fmt, ...);
        for(i=0; i< count; i++)
                writechar(*((char *)buf+i));

        fdt->ptr += i;
        return (ssize_t)i;
}




uint64_t sys_getdentry(uint64_t fd, uint64_t *buf, uint64_t max_buf_size)
{
//	struct posix_header_ustar *readdir_tarfs(int fd, uint64_t buf);

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



        printf("start getdentry_tarfs \n");
        cur_task = (task_t *)get_current_task();
        fdt = (struct file_descript *)cur_task->fd[fd];

        address = (uint64_t)fdt->header;

        if(strcmp(fdt->header->typeflag, dirtype) != 0){
                printf("sys_getdentry: it is not directory\n");
        }
        else{
                for(i=0; i <= fdt->ptr ; i++ ){

                        tarfs_header = (struct posix_header_ustar *)address;

                        offset = get_file_size(tarfs_header);

                        if(offset != 0)
                                printf("offset = %x\n",offset);
                        address += sizeof(struct posix_header_ustar);
                        address += ((((offset ) /512 ) /*+ 1 */) * 512);

                        if(offset % 512)
                                address += 512;

                        tarfs_header = (struct posix_header_ustar *)address;


                        ret = check_dir(tarfs_header->name, fdt->header->name);
                        if(ret == -2  ){
                                // not directory, file
                                i = i -1;
                        }else if(ret == 0 ){

                                if(i == fdt->ptr){
                                        //copymem((struct posix_header_ustar *)buf, tarfs_header, sizeof(struct posix_header_ustar));
                                  	dirent->d_ino = (long)tarfs_header;
					dirent->d_off = (off_t)tarfs_header->size;
					dirent->d_reclen = sizeof(struct dirent);
					copymem(dirent->d_name, tarfs_header->name, 100);
				        fdt->ptr += 1;
					return dirent->d_reclen;
                                        //return (struct posix_header_ustar *)buf;
                                }


                        }
                }
        }

        return 0;




}



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

	//	if(offset != 0)
	//		printf("offset = %x\n",offset);
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
			start_header = (uint64_t)tarfs_header;
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
	printf("open_tarfs: start_header = %x\n", start_header);	
	return fd;
}

#if(1)
int64_t get_fd(struct task_t *cur_task, struct posix_header_ustar *header_addr, int flags)
{

	uint64_t i = 0;
	task_t *task = (task_t *)cur_task;
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

	struct file_descript *fdt;
	char *filetype = "0";
	char *dirtype = "5";
	task_t *cur_task;
	char *start;	
	uint64_t size = 0;
	uint64_t i = 0;
	uint64_t ptr= 0;

	printf("start read_tarfs \n");
	cur_task = (task_t *)get_current_task();
	fdt = (struct file_descript *)cur_task->fd[fd]; 

	printf("read_tarfs: fdt->header: %x\n", fdt->header);	

	if(strcmp(fdt->header->typeflag, dirtype) == 0)
	{
		printf("it is directory, cannot read it\n");
	}
	else if(strcmp(fdt->header->typeflag, filetype) == 0)
	//else if((char)*(fdt->header->typeflag) == '0')
	{
		//memcpy((uint64_t)(fdt->header + sizeof(struct posix_header_ustar)) + ptr, buf, count);
		ptr = fdt->ptr;
		start = (char *)((uint64_t)fdt->header + sizeof(struct posix_header_ustar) + ptr);
		size = get_file_size(fdt->header);
		for(i=0; i < count; i++)
		{
			*((char *)buf + i) = (char)*(start + i);
			if( ptr + i >= size)
				break;
		}
 	}
	
	fdt->ptr += i;
	return i;
}


off_t lseek_tarfs(int fildes, off_t offset, int whence)
{
	task_t *cur_task;
	struct file_descript *fdt;

	

	cur_task = (task_t *)get_current_task();
        fdt = (struct file_descript *)cur_task->fd[fildes];

	if(whence == SEEK_SET){
		fdt->ptr = offset;
	}
	else if(whence == SEEK_CUR){
		fdt->ptr += offset;
	}
	else if(whence == SEEK_END){
		/* TODO how to do that  The offset is set to the size of the file plus offset bytes.*/
                fdt->ptr += offset;
        }


	
	return fdt->ptr;
}

int close_tarfs(int fd)
{
	;
	return 0;
}

void *opendir_tarfs(const char *name)
{

#if(1)
	struct posix_header_ustar *tarfs_header;
	//struct file_descript *fdt;

        int64_t fd = 0;
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
                        printf("finding %s dir\n",name);
                        //fd = (uint64_t)&tarfs_header;
                        start_header = (uint64_t)tarfs_header;
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
        fd = get_fd((struct task_t *)get_current_task(), (struct posix_header_ustar *)start_header, 0 /*flags*/);

        //return (d *)fd;
#endif
	
	return (uint64_t *)fd;

}

int64_t check_dir(char *next_dir, char *parent_dir)
{
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


	printf("start readdir_tarfs \n");
        cur_task = (task_t *)get_current_task();
        fdt = (struct file_descript *)cur_task->fd[fd];
	
	address = (uint64_t)fdt->header;

	if(strcmp(fdt->header->typeflag, dirtype) != 0){
		printf("readdir_tarfs: it is not directory\n");
	}
	else{
		for(i=0; i <= fdt->ptr ; i++ ){

			tarfs_header = (struct posix_header_ustar *)address;

                	offset = get_file_size(tarfs_header);

                	if(offset != 0)
                        	printf("offset = %x\n",offset);
                	address += sizeof(struct posix_header_ustar);
                	address += ((((offset ) /512 ) /*+ 1 */) * 512);

                	if(offset % 512)
                        	address += 512;

			tarfs_header = (struct posix_header_ustar *)address;

		
			ret = check_dir(tarfs_header->name, fdt->header->name);
			if(ret == -2  ){
				// not directory, file
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
	
	return 0;
}

int closedir_tarfs(void *dir)
{
	;
	return 0;
}
 
