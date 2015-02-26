#ifndef _PIC_H
#define _PIC_H

#include <sys/defs.h>


void init_pic(void);

uint8_t in_port_byte(unsigned short port);


void out_port_byte(unsigned short port, unsigned char data);







#endif
