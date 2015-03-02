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


#include <sys/pic.h>
#include <sys/rtc.h>

#define RTC_CMOS_ADDRESS        0x70
#define RTC_CMOS_DATA           0x71

#define RTC_ADDRESS_SECOND      0x00
#define RTC_ADDRESS_MINUTE      0x02    
#define RTC_ADDRESS_HOUR        0x04    

#define BCD_TO_INTEGER(bcd)     ( ((bcd >> 4) & 0x0F) * 10 + (bcd & 0x0F) )             


void _read_utc_time(uint8_t *hour, uint8_t *min, uint8_t *sec);


struct rtc_t *
read_utc_time(struct rtc_t *realtime)
{
	realtime->is_nag = 0;
	_read_utc_time(&(realtime->hour), &(realtime->min), &(realtime->sec));
	return realtime;
}


struct rtc_t *
convert_time(struct rtc_t *timedest, struct rtc_t *timesrc, struct rtc_t *timedest_zone, struct rtc_t *timesrc_zone)
{
	int temp;
	int is_nag;
	struct rtc_t utctime = TIMEZONE_UTC;

	is_nag = timesrc_zone->is_nag == 0 ? 1 : -1;
	temp = (timesrc->hour + 24 - is_nag * timesrc_zone->hour) % 24;
	utctime.hour = temp;
	temp = (timesrc->min + 60 - is_nag * timesrc_zone->min) % 60;
	utctime.min = temp;
	temp = (timesrc->sec + 60 - is_nag * timesrc_zone->sec) % 60;
	utctime.sec = temp;

	is_nag = timedest_zone->is_nag == 0 ? 1 : -1;
	temp = (utctime.hour + 24 + is_nag * timedest_zone->hour) % 24;
	timedest->hour = temp;
	temp = (utctime.min + 60 + is_nag * timedest_zone->min) % 60;
	timedest->min = temp;
	temp = (utctime.sec + 60 + is_nag * timedest_zone->sec) % 60;
	timedest->sec = temp;

	return timedest;
}


struct rtc_t *
get_rtc_time(struct rtc_t *timedest, struct rtc_t *timedest_zone)
{
	struct rtc_t utctime;
	struct rtc_t utctimezone = TIMEZONE_UTC;

	read_utc_time(&utctime);
	convert_time(timedest, &utctime, timedest_zone, &utctimezone);

	return timedest;
}

void _read_utc_time(uint8_t *hour, uint8_t *min, uint8_t *sec)
{
        uint8_t temp = 0;
        out_port_byte(RTC_CMOS_ADDRESS, RTC_ADDRESS_HOUR);
        temp = in_port_byte(RTC_CMOS_DATA);
        *hour = BCD_TO_INTEGER(temp);

        out_port_byte(RTC_CMOS_ADDRESS, RTC_ADDRESS_MINUTE);
        temp = in_port_byte(RTC_CMOS_DATA);
        *min = BCD_TO_INTEGER(temp);

        out_port_byte(RTC_CMOS_ADDRESS, RTC_ADDRESS_SECOND);
        temp = in_port_byte(RTC_CMOS_DATA);
        *sec = BCD_TO_INTEGER(temp);
}

