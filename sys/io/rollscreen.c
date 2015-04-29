/*
 *
 *  This file is part of sbunix.
 *        sbunix is a simple linux implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
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


#include <sys/kio.h>
#include <sys/console.h>
#include <sys/mem.h>


void rollscreen(int n)
{
	shiftmem(g_screenshot, g_screenshot + 2 * CONSOLE_COL * CONSOLE_ROW, 2 * n * CONSOLE_COL);
	/*
	 * Comment out for optimized shiftmem
	setmem( g_screenshot + 2 * CONSOLE_COL * CONSOLE_ROW - 2 * n * CONSOLE_COL, g_screenshot + 2 * CONSOLE_COL * CONSOLE_ROW, 0);
	 */
	g_current_pos -= 2 * n * CONSOLE_COL;
	flushscreen();
}
