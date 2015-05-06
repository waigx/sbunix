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


#include <sys/kio.h>
#include <sys/keyboard.h>
#include <sys/mem.h>
#include <string.h>


static uint8_t g_scancode2ascii[] = {SCANCODE2ASCII_TABLE};
static uint8_t g_scancode2ascii_shift[] = {SCANCODE2ASCII_TABLE_SHIFT};
static uint8_t g_scancode2ascii_ctrl[] = {SCANCODE2ASCII_TABLE_CTRL};
static char g_ascii2char[] = ASCII2CHAR;
static char g_sc2char[] = SC2CHAR;


char echokeyboard(void)
{
	uint8_t sc = 0;
	char ch = 0;
	char ctrlch = 0;
	char display[4];
	char ret_ch = 0;
	sc = get_scancode_keyboard();
	
	switch (sc) {
		case SC_SHIFT_LEFT:
		case SC_SHIFT_RIGHT:
			is_shifted = 1;
			break;
		case SC_SHIFT_LEFT + SC_RELEASE_OFFSET:
		case SC_SHIFT_RIGHT + SC_RELEASE_OFFSET:
			is_shifted = 0;
			break;
		case SC_CTRL_LEFT:
			is_ctrled = 1;
			break;
		case SC_CTRL_LEFT + SC_RELEASE_OFFSET:
			is_ctrled = 0;
			break;
		default:
			if (is_shifted) {
				ch = g_scancode2ascii_shift[sc];
				break;
			}
			if (is_ctrled) {
				ch = g_scancode2ascii_ctrl[sc];
				ctrlch = g_scancode2ascii[sc];
				break;
			}
			ch = g_scancode2ascii[sc];
			break;
	}

	if ((ch < 128 && ch >31) || ch == 0x8 || ch == 0xA || ch == 0xD) {
		writechar(ch);
		ret_ch = ch;
	}
	if (ctrlch < 128 && ctrlch > 31) {
		printfat(CONSOLE_ROW - 1, CONSOLE_COL / 2 + 4, "%c", ctrlch);
	}
	if (sc < SC_RELEASE_OFFSET){
		if (ch != 0) {
			strncpy(display, g_ascii2char + 3 * ch, 3);
		} else {
			strncpy(display, g_sc2char + 3 * sc, 3);
		}
		display[3] = '\0';

		if (is_shifted) {
			printfat(CONSOLE_ROW - 1, CONSOLE_COL / 2 - 8, "SFT");
		} else {
			printfat(CONSOLE_ROW - 1, CONSOLE_COL / 2 - 8, "   ");
		}

		if (is_ctrled) {
			printfat(CONSOLE_ROW - 1, CONSOLE_COL / 2 + 3, "^");
		} else {
			printfat(CONSOLE_ROW - 1, CONSOLE_COL / 2 + 3, "  ");
		}

		printfat(CONSOLE_ROW - 1, CONSOLE_COL / 2 - 3, "%s", display);
	}

	return ret_ch;
}
