/******************************************************************
 * serial.h
 * interface of serial.c
 * by zhaoyuandong at 2020/4/21
 ******************************************************************/
#ifndef __SERIAL_H__
#define __SERIAL_H__

extern void uart_init(void);
extern void putc(unsigned char);
extern void puts(unsigned char *str);
extern void puthex(unsigned int data);

#endif /*__SERIAL_H__*/
