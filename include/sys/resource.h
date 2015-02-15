/*
 *
 *  This file is part of sblibc.
 *        sblibc is a simple libc implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  sblibc is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  sblibc is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with SBUsh.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/defs.h>
#include <time.h>

struct  rusage {
	struct timeval ru_utime;
	struct timeval ru_stime;
	int32_t ru_maxrss;
	int32_t ru_ixrss;
	int32_t ru_idrss;
	int32_t ru_isrss;
	int32_t ru_minflt;
	int32_t ru_majflt;
	int32_t ru_nswap;
	int32_t ru_inblock;
	int32_t ru_oublock;
	int32_t ru_msgsnd;
	int32_t ru_msgrcv;
	int32_t ru_nsignals;
	int32_t ru_nvcsw;
	int32_t ru_nivcsw;
};

