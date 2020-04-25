/******************************************************************
 * serial.c
 * put string to serial port
 * by zhaoyuandong at 2020/4/21
 ******************************************************************/

#include "s3c24xx.h"
#include "serial.h"

#define TXD0READY   (1<<2)

#define PCLK            50000000   
#define UART_CLK        PCLK        
#define UART_BAUD_RATE  115200      
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)

void uart_init(void)
{
    GPHCON  |= 0xa0;    /*gph2-tx0, gph3-rx0*/
    GPHUP   = 0x0c;     /*use pull-up resister*/

    ULCON0  = 0x03;     /*8n1 mode*/
    UCON0   = 0x05;     /*do not use DMA*/
    UFCON0  = 0x00;     /*do not use fifo*/
    UMCON0  = 0x00;     /*do not use flow-control*/
    UBRDIV0 = UART_BRD; 
}

void putc(unsigned char c)
{

    while (!(UTRSTAT0 & TXD0READY));
    
    UTXH0 = c;
}

void puts(unsigned char *str)
{
	putc('[');
	putc('A');
	putc('l');
	putc('b');
	putc('e');
	putc('r');
	putc('t');
	putc(']');
	putc(' ');
	while (*str)
		putc(*str++);
}

void puthex(unsigned int data)
{
	int i = 0;
	int slice = 0;
	
	puts("0x");
	for (i = 0; i < 8; i++) {
		slice = data >> ((7-i)*4) & 0xf;

		if (slice >= 10) {
			putc('A'+slice-10);	
		} else {
			putc('0'+slice);
		}
	}
}







