/*
 *
 *  This file is part of SBUsh.
 *        SBUsh is a simple shell implementation, basically, it is
 *  an academic project of CSE506 of Stony Brook University in Spring 
 *  2015. For more details, please refer to README.md.
 *
 *  Copyright (C) 2015 Yigong Wang <yigwang@cs.stonybrook.edu>
 * 
 *
 *  SBUsh is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  SBUsh is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with SBUsh.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <sys/defs.h>
#include <const.h>
#include <libio.h>


/**
 * echoerr - write a line of error infomation to standard out
 * @app: Program name
 * @operate: Operation name 
 * @error: Detailed error infomation
 */
void
echoerr(const char *app, const char *operate, const char *error)
{
	writeline("-", STDOUT_FD);
	writeline(app, STDOUT_FD);
	writeline(": ", STDOUT_FD);
	writeline(operate, STDOUT_FD);
	writeline(": ", STDOUT_FD);
	writeline(error, STDOUT_FD);
	writeline("\n", STDOUT_FD);

	return;
}
