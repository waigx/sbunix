#ifndef _TIMER_H
#define _TIMER_H

#include <sys/defs.h>
#include <sys/rtc.h>


#define TIME_FREQ   1000


typedef int64_t time_t;

struct timespec {
	time_t tv_sec;
	int64_t tv_nsec;
};

struct timeval {
	time_t tv_sec;
	int64_t tv_usec;
};


extern uint32_t g_timer_count;
extern struct rtc_t g_time_boot;
void set_timer(uint16_t);
void echotime(void);
int sys_nanosleep(const struct timespec *req, struct timespec *rem);

#endif
