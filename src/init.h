/******************************************************************
 * init.h
 * the interface of init.c
 * by zhaoyuandong at 2020/4/24
 ******************************************************************/
#ifndef __INIT_H__
#define __INIT_H__

/* declare extern function interface */
extern void nand_init(void);
extern void nand_read(unsigned char *addr, unsigned char *buf, unsigned long len);

#endif /*__INIT_H__*/
