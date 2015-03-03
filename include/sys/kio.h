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


#include <stdarg.h>
#include <sys/defs.h>
#include <sys/console.h>


//extern char *g_current_pos;
extern uint32_t g_current_pos;
extern uint8_t g_default_color;
extern char g_screenshot[CONSOLE_ROW * CONSOLE_COL * 2];


int strlistprintf(char *buf, const char *format, va_list val);

int printfat(int r, int c, const char *fmt, ...);
int backspacechar(int n);

void writechar(char c);
void writenewline(int n);
void rollscreen(int n);
void flushscreen(void);
void screenshot(void);
void writecharcolor(char c, uint8_t color);
void writecharpos(char *pos, char c, uint8_t color);
