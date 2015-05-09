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
//uint64_t copy_argument_heap(char **argv, char **envp, struct args_struct *args_st, uint8_t ** stack_space);
struct args_struct *copy_argument_kmem(char **argv, char **envp);
struct args_struct *copy_argument_ustack(char **argv, char **envp, char *user_stack);




// Temp buf
char kmalloc_buf[PAGE_SIZE * 4];
uint64_t kmalloc_ptr = 0;

void *kmalloc(uint64_t size)
{
	uint64_t s = 0;
	s = kmalloc_ptr;
	size = ((size + 8) >> 3)<< 3; 
	kmalloc_ptr += size;
	if(kmalloc_ptr > (PAGE_SIZE * 4))
	{
		kmalloc_ptr = 0;
		s = 0;
	}
	return (void *)(kmalloc_buf + s);


}


uint64_t
sys_execve(const char *filename, char *const argv[], char *const envp[])
{
	//uint64_t ret = 0;
	uint64_t entry_point;
	task_t *cur_task = gp_current_task;
	struct args_struct *args_st; 
	//uint8_t **stack_space = NULL;
	cr3e_t new_cr3;
	uint64_t *phy_stack_base;
	uint64_t i = 0;
	uint64_t *physpage;


	strcpy(cur_task->name, filename); 
	printf("sys_execve: filename=%s\n", filename);
	args_st = (struct args_struct *)copy_argument_kmem((char**)argv, (char **)envp);

	cur_task-> args = args_st;

	//argc = get_argc(argv);

	// copy pml4e map or not??
	task_t *kernel_task = gettask(KERNEL_PID);
        load_cr3(kernel_task->cr3);
	printf("sys_execve: filename=%s\n", filename);


	new_cr3 = newvmem();
	 // Allocate space for init VMAs
        // Use g_vma_phy_start in kernel cr3.
        for (i = (uint64_t)g_vma_start; i < (uint64_t)g_vma_end; i += PAGE_SIZE) {
                physpage = allocframe();
                if (i == (uint64_t)g_vma_start)
                        g_vma_phy_start = (vma_t *)physpage;
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

	// How to do vma

	printf("sys_execve: filename=%s\n", filename);

	
	entry_point = load_elf(cur_task, cur_task->name);
	cur_task->entry_point = entry_point;
	if(entry_point == -1){
		// load fail
		return -1;
	}

	//load_cr3(cur_task->cr3);
	  newvma(g_vma_phy_start + 1, (void *)(USER_HEAP_START), (void *)(USER_HEAP_START), VMA_HEAP_NAME, VMA_READABLE | VMA_WRITEABLE);
	
	// copy argument
	load_cr3(cur_task->cr3);

	args_st = copy_argument_ustack((char **)(cur_task->args->argv), (char **)(cur_task->args->envp), (char *)(USER_STACK_START + 1024 * 3));

	//copy_argument_ustack( (char**)args_st->argv, (char **)args_st->envp); 
	//copymem((uint8_t *)(USER_STACK_START + 1024 * 3), (uint8_t *)temp_buf, 1024);
	// setting regs structure
	//init_task_context(cur_task, entry_point, );
	init_task(cur_task, cur_task->entry_point, (uint64_t *)USER_STACK_START, (uint64_t *)KERNEL_STACK_START);
	
	// rdi, rsi, rdx
	cur_task->context.regs[CONTEXT_RDI_OFFSET] = (uint64_t)args_st->argc;
	cur_task->context.regs[CONTEXT_RSI_OFFSET] = (uint64_t)args_st->argv;
	cur_task->context.regs[CONTEXT_RDX_OFFSET] = (uint64_t)args_st->envp;
	cur_task->context.regs[CONTEXT_RSP_OFFSET] = (uint64_t)(USER_STACK_START + 1024 * 3 -0x10);


	sys_yield(1);

	return -1;
}

uint64_t get_argc(char *argv[])
{
	uint64_t i = 0;
	
	for(i=0; argv[i] != NULL; i++);

	return i;
}

struct args_struct *copy_argument_ustack(char **argv, char **envp, char *user_stack)
{


	 
	uint64_t argc = 0;
        uint64_t envc = 0;
        //char **heap_ptr;
        uint64_t len = 0;
        uint64_t i = 0;
        //char ** heap;
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
        //char **heap_ptr;
        uint64_t len = 0;
        uint64_t i = 0;
        //char ** heap;

	struct args_struct *args = kmalloc(sizeof(struct args_struct));

        argc = get_argc(argv);
        envc = get_argc(envp);
        
        args->argc = argc;
        args->envc = envc;

        // How to set malloc size
        //heap = (char **)malloc(PAGE_SIZE);
        //heap = (char **)stack_space;
        //heap_ptr = (char **)heap;

        //args_st->argv = (char  **)heap_ptr;    
	args->argv = kmalloc((argc + 1) * sizeof(char *));
 
        for(i=0; i < argc; i++)
        {
                len = strlen(argv[i]);
		args->argv[i] = kmalloc(len + 1);
                copymem( args->argv[i], argv[i], len + 1);
                //heap_ptr += len;
        }
	//args->argv = (char **)args->argv[0];
        argv[i] = NULL;
        //heap_ptr ++;

        //args_st->envp = (char **)heap_ptr;
	args->envp = kmalloc((envc + 1) * sizeof(char *));

        for(i=0; i < envc ; i++)
        {
                //len = strlen(envp[i]);
                //copymem(heap_ptr, envp[i], len);
                //heap_ptr += len;

		len = strlen(envp[i]);
                args->envp[i] = kmalloc(len + 1);
                copymem( args->envp[i], envp[i], len + 1);

        }
	//args->envp = (char **)args->envp[0];
        envp[i] = NULL;

        return args;
}




/*
uint64_t copy_argument_heap(char **argv, char **envp, struct args_struct *args_st, uint8_t ** stack_space)
{
	uint64_t argc = 0;
	uint64_t envc = 0;
	char **heap_ptr;
	uint64_t len = 0;
	uint64_t i = 0;
	char ** heap;

	argc = get_argc(argv);
	envc = get_argc(envp);
	
	args_st->argc = argc;
	args_st->envc = envc;

	// How to set malloc size
	//heap = (char **)malloc(PAGE_SIZE);
	heap = (char **)stack_space;
	heap_ptr = (char **)heap;

	args_st->argv = (char  **)heap_ptr;	

	for(i=0; i < argc; i++)
	{
		len = strlen(argv[i]);
		copymem(heap_ptr, argv[i], len);
		heap_ptr += len;
	}
	argv[i] = NULL;
	heap_ptr ++;

	args_st->envp = (char **)heap_ptr;

	for(i=0; i < envc ; i++)
        {
                len = strlen(envp[i]);
                copymem(heap_ptr, envp[i], len);
                heap_ptr += len;
        }
        envp[i] = NULL;

	return 0;
}

*/
	
