/****************************************************************** 
 * init.c 
 * device initial 
 * by zhaoyuandong at 2020/4/14 
 ******************************************************************/ 
#include "serial.h"

#define NFCONF (*((volatile unsigned long*)0x4E000000))
#define NFCONT (*((volatile unsigned long*)0x4E000004))
#define NFCMMD (*((volatile unsigned char*)0x4E000008))
#define NFADDR (*((volatile unsigned char*)0x4E00000C))
#define NFDATA (*((volatile unsigned char*)0x4E000010))
#define NFSTAT (*((volatile unsigned char*)0x4E000020))

/* nand flash parameters */
#define NAND_FLASH_PAGE_SIZE 2048
#define TACLS	0
#define TWRPH0	2
#define TWRPH1	0

void nand_init(void) {
	/* set clk sequence */
	NFCONF = TACLS<<12  | TWRPH0<<8 | TWRPH1<<4 | 0<<0;
	/* init ecc, disable chip select, enable nandflash */
	NFCONT = 0x1<<4 | 0x1<<1 | 0x1<<0; 
} 

void nand_send_command(unsigned char command)
{
	volatile int i = 0;

	NFCMMD = command;
	for (i=0; i<10; i++); /*wait for a while after send a command*/
}

void nand_send_address(unsigned int address)
{
	unsigned int page = address / NAND_FLASH_PAGE_SIZE;
	unsigned int column = address % NAND_FLASH_PAGE_SIZE;
	volatile int i = 0;

	/* 1.column 1st address */
	NFADDR = (unsigned char)column;
	for (i=0; i<10; i++); /*wait for a while after send a address*/
	/* 2.column 2st address */
	NFADDR = (unsigned char)(column >> 8) & 0x0f;
	for (i=0; i<10; i++); 

	/* 3.row 1st address */
	NFADDR = (unsigned char)page;
	for (i=0; i<10; i++);
	/* 4.row 2st address */
	NFADDR = (unsigned char)(page >> 8);
	for (i=0; i<10; i++);
	/* 5.row 3st address */
	NFADDR = (unsigned char)(page >> 16);
	for (i=0; i<10; i++);
}

unsigned char nand_data(void)
{
	return NFDATA;
}

void nand_chip_select(void)
{
	NFCONT &= ~(0x1 << 1);	
}

void nand_chip_deselect(void)
{
	NFCONT |= 0x1 << 1;	
}

void nand_wait_ready(void)
{
	while (!(NFSTAT & 0x1));
}

void nand_read(unsigned int addr, unsigned char *buff, unsigned int len) 
{ 
	unsigned int i = 0; /*control one dimensional cycle*/
	unsigned int colCurrent = addr % NAND_FLASH_PAGE_SIZE; /*calculate column start address of the fist page*/
	//puts("---------debug-----------\r\n");
	//puthex(colCurrent);putc('\r');putc('\n');

	/* 1.enable chip select */
	nand_chip_select();

	for (i=0; i<len; /*i++*/){
	/* read ONE PAGE by Step2-6 */
		/* 2.send 0x00 command code */
		nand_send_command(0x00);

		/* 3.send 5 cycle address */
		nand_send_address(addr);

		/* 4.send 0x30 command code */
		nand_send_command(0x30);

		/* 5.wait nand flash read the data from the selected column address to data register */ 
		nand_wait_ready();

		/* 6.read the all the output data of ONE PAGE */
		for (; colCurrent < NAND_FLASH_PAGE_SIZE; colCurrent++){
			buff[i++] = nand_data();
			addr++;
#if 0
			if (0 == colCurrent) {
			 
				puthex(addr-1);putc('-');puthex(buff[i-1]);
				putc('\r');putc('\n');
			}
#endif
		}
		
		colCurrent = 0; /*reset the colCurrent*/
	}

	/* 7.disable chip select */
	nand_chip_deselect();
} 

int is_boot_from_nor_flash(void) 
{ 
	volatile int *p = (volatile int *)0x0;
	int valueStore = 0;

	valueStore = *p;
	*p = 0x5a5a5a5a;
	if (0x5a5a5a5a == *p){
		*p = valueStore;
		return 0;	//nand flash
	}
	else{
		return 1;	//nor flash
	}
}

void copy2sdram(unsigned char *src, unsigned char *dest, unsigned int len)
{
	int i = 0;

	/* 1.boot from nor flash */
	if (is_boot_from_nor_flash()){
		while (i < len){	
			dest[i] = src[i];	
			i++;
		}
	}
	else{	
	/* 2.boot from nand flash */
		nand_read((unsigned int)src, dest, len);
	}
}

void clean_bss(void)
{
	extern int __bss_start, __bss_end;
	int *p = &__bss_start;

	for(; p < &__bss_end; p++)
		*p = 0;
}
