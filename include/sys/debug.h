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


#ifndef _DEBUG_H
#define _DEBUG_H


#include <sys/defs.h>
#include <sys/managemem.h>

#define DEBUG_FS                             0
#define DEBUG_MEM                            1
#define DEBUG_ELF                            1
#define DEBUG_PAGEFAULT                      1
#define DEBUG_SCHED                          0
#define DEBUG_FATAL                          1

extern uint8_t g_debug_mode;

uint64_t *debug_convadd2phy(cr3e_t cr3e, void *vaddr);
void debug_pause();
void debug_showvmas(vma_t *vma_start);
void debug_print(const char *category, const char *format, ...);

#endif
