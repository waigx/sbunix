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


#include <sys/timer.h>
#include <sys/rtc.h>
#include <sys/kio.h>


void echotime(void)
{
	struct rtc_t timezone = TIMEZONE_EST_S;
	struct rtc_t timenow;

	g_time_boot.nano += 1000000000/TIME_FREQ;
	if (g_time_boot.nano >= 1000000000) {
		g_time_boot.nano = 0;
		g_time_boot.sec += 1;
	}
	if (g_time_boot.sec == 60) {
		g_time_boot.sec = 0;
		g_time_boot.min += 1;
	}
	if (g_time_boot.min == 60) {
		g_time_boot.min = 0;
		g_time_boot.hour += 1;
	}
	if (g_time_boot.hour == 24)
		g_time_boot.hour = 0;

	printfat(CONSOLE_ROW - 1, CONSOLE_COL - 10, "%d:%d:%d   ", g_time_boot.hour, g_time_boot.min, g_time_boot.sec);

	get_rtc_time(&timenow, &timezone);
	printfat(CONSOLE_ROW - 1, 0, "%d:%d:%d   ", timenow.hour, timenow.min, timenow.sec);
}
