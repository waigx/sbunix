#ifndef _TIMER_H
#define _TIMER_H

#include <sys/defs.h>
#include <sys/rtc.h>

extern uint8_t g_timer_count;
extern struct rtc_t g_time_boot;
void set_timer(uint16_t);
void echotime(void);

#endif
