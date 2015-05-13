#ifndef _RTC_H
#define _RTC_H

#include <sys/defs.h>

/****
 * Timezones:
 *   * CST - China
 *   * EST - East Coast Time
 *   * KST - Korea
 *   * UTC - Coordinated Universal Time
 *
 */
#define TIMEZONE_CST {.is_nag = 0, .hour = 8, .min = 0, .sec = 0, .nano = 0}
#define TIMEZONE_EST {.is_nag = 1, .hour = 5, .min = 0, .sec = 0, .nano = 0}
#define TIMEZONE_KST {.is_nag = 0, .hour = 9, .min = 0, .sec = 0, .nano = 0}
#define TIMEZONE_UTC {.is_nag = 0, .hour = 0, .min = 0, .sec = 0, .nano = 0}

#define TIMEZONE_EST_S {.is_nag = 1, .hour = 4, .min = 0, .sec = 0, .nano = 0}

struct rtc_t{
	uint8_t is_nag;
	uint8_t   hour;
	uint8_t    min;
	uint8_t    sec;
	uint64_t  nano;
};

struct rtc_t * read_utc_time(struct rtc_t *realtime);
struct rtc_t * convert_time(struct rtc_t *timedest, struct rtc_t *timesrc, struct rtc_t *timedest_zone, struct rtc_t *timesrc_zone);
struct rtc_t * get_rtc_time(struct rtc_t *timedest, struct rtc_t *timedest_zone);

#endif        
