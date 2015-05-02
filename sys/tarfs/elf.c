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
//#include <sys/register.h>
//#include <syscall.h>
#include <sys/sbunix.h>
#include <string.h>
#include <sys/elf.h>
#include <sys/tarfs_api.h>
//#include <sys/sched/sched.h>

#include <sys/managemem.h>
#include <sys/mem.h>
#include <sys/register.h>
#include <sys/debug.h>

uint64_t check_ELF_format( Elf64_Ehdr *elf);


uint64_t load_elf(task_t *task, const char *task_name)
{
	Elf64_Ehdr *elfhdr;
	Elf64_Phdr *phdr;
	//struct Elf64_Shdr;
	uint64_t i = 0;
	Elf64_Phdr *ph_start;
	uint64_t ph_num = 0;
	uint64_t  fd = 0;

	uint64_t offset = 0;
	uint64_t size = 0;
	uint64_t vaddr = 0;
	//uint64_t flag = 0;
	uint64_t page;

	//fd = open_tarfs(binary, 0);
	fd = find_elf(task_name, 0);
	elfhdr = ( Elf64_Ehdr *)fd;

	if(check_ELF_format(elfhdr) == FALSE)
	{
		debug_print("ELF", "Error, the file is not ELF file.\n");
		return 0;
	}
	ph_start = ( Elf64_Phdr *)((uint8_t *)elfhdr + elfhdr->e_phoff);
	ph_num = (uint64_t)elfhdr->e_phnum;

	// CR3 setting

	for(i=0; i< ph_num; i++)
	{
		phdr =( Elf64_Phdr *)(ph_start + i);
		if(phdr->p_type == 0x01)
		{
			offset = phdr->p_offset;
			size = phdr->p_memsz;
			vaddr = phdr->p_va;
			//flag = phdr->p_flags;
			//permission =

			page = (uint64_t)allocframe(task->pid);
			kmmap(pe2physaddr(task->cr3), task->pid, page, (uint64_t)vaddr, TRUE, FALSE);
			copymem((uint64_t *)page,(uint64_t *)(offset+ (uint64_t)elfhdr), size);
		}

	}

	return elfhdr->e_entry;
}


uint64_t check_ELF_format(  Elf64_Ehdr *elf)
{
	if( (elf->e_ident[0] == 0x7f)
		&& (elf->e_ident[1] == 'E')
		&& (elf->e_ident[2] == 'L')
		&& (elf->e_ident[3] == 'F'))
			return TRUE;
	else
		return FALSE;	

}
