#ifndef __STDOUT_USART_H__
#define __STDOUT_USART_H__

#include <stdint.h>


int stdout_putchar (int ch);
int stdout_putbuffer (uint32_t *buf, int len, int stride);
int stdout_init (void);

#endif