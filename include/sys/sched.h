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


#ifndef _SCHED_H
#define _SCHED_H

#include <sys/defs.h>
#include <sys/tarfs_api.h>
#include <sys/gdt.h>

// SS, RSP, RFLAGS, CS, RIP + registers as ISR //

#define REGISTERS_CONTEXT_SWITCH            (5 + 19)

#define CONTEXT_GS_OFFSET                          0
#define CONTEXT_FS_OFFSET                          1
#define CONTEXT_ES_OFFSET                          2
#define CONTEXT_DS_OFFSET                          3
#define CONTEXT_R15_OFFSET                         4
#define CONTEXT_R14_OFFSET                         5
#define CONTEXT_R13_OFFSET                         6
#define CONTEXT_R12_OFFSET                         7
#define CONTEXT_R11_OFFSET                         8
#define CONTEXT_R10_OFFSET                         9
#define CONTEXT_R9_OFFSET                         10
#define CONTEXT_R8_OFFSET                         11
#define CONTEXT_RSI_OFFSET                        12
#define CONTEXT_RDI_OFFSET                        13
#define CONTEXT_RDX_OFFSET                        14
#define CONTEXT_RCX_OFFSET                        15
#define CONTEXT_RBX_OFFSET                        16
#define CONTEXT_RAX_OFFSET                        17
#define CONTEXT_RBP_OFFSET                        18
#define CONTEXT_RIP_OFFSET                        19
#define CONTEXT_CS_OFFSET                         20
#define CONTEXT_RFLAG_OFFSET                      21
#define CONTEXT_RSP_OFFSET                        22
#define CONTEXT_SS_OFFSET                         23

#define GDT_KERNEL_CODE_SEG                     0x08
#define GDT_KERNEL_DATA_SEG                     0x10

#define GDT_USER_CODE_SEG                       0x1B
#define GDT_USER_DATA_SEG                       0x23
  
#define KERNEL_PID                                 1
#define MAX_PROC_NUM                        (1 << 8)
#define MAX_TASKS                           (1 << 8)
#define MAX_TASK_NAME                            256

#define MAX_OPEN_FILE_DESCRIPT                  1024

#define SWITCH_TIME_IN_MS                          5
#define SYS_YIELD_LOAD_ONLY                        1


typedef enum 
{
	PROCESS_NEW,
	PROCESS_READY,
	PROCESS_RUNNING,
	PROCESS_WAITING,
	PROCESS_TERMINATED
} process_status_t;

typedef enum 
{
	USER_PROCESS,
	KERNEL_PROCESS
} process_type_t;


struct regs_struct
{
	uint64_t regs[REGISTERS_CONTEXT_SWITCH];
};


struct args_struct
{
	uint64_t argc;
	uint64_t envc;
	char **argv;
	char **envp;
};


typedef struct
{
	kpid_t pid;
	kpid_t parent;

	char name[MAX_TASK_NAME];
	
	// related to memory and regs //
	cr3e_t cr3;

	// process status //
	process_status_t status;
	process_type_t type;

	uint64_t *u_stack_base;
	uint64_t *k_stack_base;
	uint64_t reserved[4]; /* Don't remove because k_stack_base is scrached, by dongju */
	struct regs_struct context;

	// TSS
	struct tss_t *tsss;

	// ring active
	uint64_t b_ring;

	//passing args for exec
	struct args_struct args;
	uint64_t entry_point;


	// Open file descript, stdin, stdout, and stderr are 0, 1, and 2 //
	struct file_descript *fd[MAX_OPEN_FILE_DESCRIPT];
} task_t;


extern uint16_t g_next_task_free_index;
extern task_t *g_task_start;
extern uint16_t g_next_task_index;
extern uint16_t g_task_bump;
extern task_t *gp_current_task;
extern uint16_t g_switch_timer;

void load_test_tasks(void);

task_t *gettask(kpid_t pid);
void loadtask(task_t *task);
void unloadtask(task_t *task);

task_t *get_next_task(void);

task_t *newtask(const char *task_name, process_type_t type);
uint64_t sys_execve(const char *filename, char *const argv[], char *const envp[]);
uint64_t sys_fork(void);

uint64_t sys_getpid(void);
uint64_t sys_getppid(void);
uint64_t sys_gettask(void *buf, kpid_t pid);

void sys_yield(uint64_t type);
void switch_context(struct regs_struct *current_regs, struct regs_struct *next_regs);
void sys_exit(uint64_t res);

void scheduler(void);
void switch_context(struct regs_struct *current_regs, struct regs_struct *next_regs);

void init_task(task_t *task, uint64_t entry_point, uint64_t *user_stack_base, uint64_t *kernel_stack);
void init_task_context(task_t *task, uint64_t entry_point, uint64_t *stack_base );

#endif
