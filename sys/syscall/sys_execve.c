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

#include <sys/sched.h>
#include <sys/managemem.h>
#include <sys/debug.h>
#include <sys/sbunix.h>
#include <string.h>
#include <sys/mem.h>
#include <sys/elf.h>
#include <sys/register.h>


uint64_t get_argc(char *argv[]);
struct args_struct *copy_argument_kmem(char **argv, char **envp);
struct args_struct *copy_argument_ustack(char **argv, char **envp, char *user_stack);


void *kmalloc(uint64_t size)
{
	uint64_t _size = g_page_frame_buf_ptr;
	size = ((size + 8) >> 3)<< 3; 
	g_page_frame_buf_ptr += size;
	if(g_page_frame_buf_ptr > (PAGE_SIZE * 4))
	{
		debug_print("exec", "Args are too long\n");
		debug_pause();
	}
	return (void *)(g_page_frame_buf + _size);
}


uint64_t _setup_task()
{
	uint64_t entry_point;
	cr3e_t new_cr3;
	uint64_t *phy_stack_base;
	uint64_t i = 0;
	uint64_t *physpage;
	task_t *cur_task = gp_current_task;

	new_cr3 = newvmem();

	// Allocate space for init VMAs
	// Use g_vma_phy_start in kernel cr3.
	for (i = (uint64_t)g_vma_start; i < (uint64_t)g_vma_end; i += PAGE_SIZE) {
		physpage = allocframe();
		if (i == (uint64_t)g_vma_start)
			g_vma_phy_start = (vma_t *)physpage;
		kmmap(pe2physaddr(new_cr3), (uint64_t)physpage, i, FALSE, FALSE);
	}

	// Allocate space for init file discriptors;
	for (i = (uint64_t)g_fdt_start; i < (uint64_t)g_fdt_end; i += PAGE_SIZE) {
		physpage = allocframe();
		kmmap(pe2physaddr(new_cr3), (uint64_t)physpage, i, FALSE, FALSE);
	}

	cur_task->cr3 = new_cr3;
	phy_stack_base = allocframe();
	kmmap(pe2physaddr(new_cr3), (uint64_t)phy_stack_base, USER_STACK_START, TRUE, FALSE);
	// Init User stack VMA
	newvma(g_vma_phy_start + 2, (void *)(USER_STACK_START - USER_STACK_SIZE), (void *)(USER_STACK_START + PAGE_SIZE), VMA_STACK_NAME, VMA_READABLE | VMA_WRITEABLE);

	/* KERNEL STACK Setting */
	phy_stack_base = allocframe();
	kmmap(pe2physaddr(new_cr3), (uint64_t)phy_stack_base, KERNEL_STACK_START, FALSE, TRUE);

	entry_point = load_elf(cur_task, cur_task->name);
	cur_task->entry_point = entry_point;
	if(entry_point == -1){
		// load fail
		return -1;
	}

	// Init User heap VMA
	newvma(g_vma_phy_start + 1, (void *)(USER_HEAP_START), (void *)(USER_HEAP_START), VMA_HEAP_NAME, VMA_READABLE | VMA_WRITEABLE);

	return 0;
}


uint64_t
sys_execve(const char *filename, char *const argv[], char *const envp[])
{
	task_t *cur_task = gp_current_task;
	struct args_struct *args_st; 

	freevmem(cur_task->pid);
	strcpy(cur_task->name, filename); 
	args_st = (struct args_struct *)copy_argument_kmem((char**)argv, (char **)envp);
	copymem((void *)&(cur_task->args), (void *)args_st, sizeof(struct args_struct));
	task_t *kernel_task = gettask(KERNEL_PID);

	load_cr3(kernel_task->cr3);
	if (_setup_task() != 0)
		return -1;
	load_cr3(cur_task->cr3);

	// copy argument
	args_st = copy_argument_ustack((char **)(cur_task->args.argv), (char **)(cur_task->args.envp), (char *)(USER_STACK_START + 1024 * 3));
	copymem((void *)&(cur_task->args), (void *)args_st, sizeof(struct args_struct));

	init_task(cur_task, cur_task->entry_point, (uint64_t *)USER_STACK_START, (uint64_t *)KERNEL_STACK_START);

	// rdi, rsi, rdx
	cur_task->context.regs[CONTEXT_RDI_OFFSET] = (uint64_t)args_st->argc;
	cur_task->context.regs[CONTEXT_RSI_OFFSET] = (uint64_t)args_st->argv;
	cur_task->context.regs[CONTEXT_RDX_OFFSET] = (uint64_t)args_st->envp;
	cur_task->context.regs[CONTEXT_RSP_OFFSET] = (uint64_t)(USER_STACK_START + 1024 * 3 -0x10);

	g_page_frame_buf_ptr = 0;
	sys_yield(1);

	return -1;
}


uint64_t get_argc(char *argv[])
{
	uint64_t i = 0;
	while(argv[i] != NULL) {
		i += 1;
	}
	return i;
}


struct args_struct *copy_argument_ustack(char **argv, char **envp, char *user_stack)
{
	uint64_t argc = 0;
	uint64_t envc = 0;
	uint64_t len = 0;
	uint64_t i = 0;
	struct args_struct *args = NULL;
	char *ptr;

	args = (struct args_struct *)user_stack; 
	ptr = (char *)user_stack;
	args->argc = get_argc(argv);
	args->envc = get_argc(envp);
	argc = args->argc;
	envc = args->envc;

	ptr += sizeof(struct args_struct);

	args->argv = (char  **)ptr;     

	ptr += (argc + 1) * sizeof(char *);

	for(i=0; i < argc; i++)
	{
		args->argv[i] = ptr;
		len = strlen(argv[i]);
		copymem(ptr, argv[i], len + 1);
		ptr += (len + 1);
	}
	argv[i] = NULL;
	ptr ++;

	args->envp = (char **)ptr;
	ptr += (envc + 1) * sizeof(char *);

	for(i=0; i < envc ; i++)
	{
		args->envp[i] = ptr;
		len = strlen(envp[i]);
		copymem(ptr, envp[i], len + 1);
		ptr += (len + 1);
	}
	envp[i] = NULL;

	return args;
}


struct args_struct *copy_argument_kmem(char **argv, char **envp)
{
	uint64_t argc = 0;
	uint64_t envc = 0;
	uint64_t len = 0;
	uint64_t i = 0;

	struct args_struct *args = kmalloc(sizeof(struct args_struct));

	argc = get_argc(argv);
	envc = get_argc(envp);

	args->argc = argc;
	args->envc = envc;

	args->argv = kmalloc((argc + 1) * sizeof(char *));

	for(i=0; i < argc; i++)
	{
		len = strlen(argv[i]);
		args->argv[i] = kmalloc(len + 1);
		copymem( args->argv[i], argv[i], len + 1);
	}
	argv[i] = NULL;

	args->envp = kmalloc((envc + 1) * sizeof(char *));

	for(i=0; i < envc ; i++)
	{
		len = strlen(envp[i]);
		args->envp[i] = kmalloc(len + 1);
		copymem( args->envp[i], envp[i], len + 1);

	}
	envp[i] = NULL;

	return args;
}

