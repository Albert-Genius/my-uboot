/****************************************************************** 
 * init.c 
 * device initial 
 * by zhaoyuandong at 2020/4/14 
 ******************************************************************/ 
#define NFCONF (*((volatile unsigned long*)0x4E000000)
#define NFCONT (*((volatile unsigned long*)0x4E000004)
#define NFCMMD (*((volatile unsigned long*)0x4E000008)
#define NFADDR (*((volatile unsigned long*)0x4E00000C)
#define NFDATA (*((volatile unsigned long*)0x4E000010)
#define NFSTAT (*((volatile unsigned long*)0x4E000020)

#define TACLS	0
#define TWRPH0	2
#define TWRPH1	0

void nand_init(void) {
	/* set clk sequence */
	NFCONF = TACLS<<12  | TWRPH0<<8 | TWRPH1<<4 | 0<<0;
	/* init ecc, disable chip select, enable nandflash */
	NFCONT = 0x1<<4 | 0x1<<1 | 0x1<<0; 
} 

void nand_read(unsigned char *addr, unsigned char *buff, unsigned int len) 
{ 
	//wait to code 
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
	}
}

void clean_bss(void)
{
	extern int __bss_start, __bss_end;
	int *p = &__bss_start;

	for(; p < &__bss_end; p++)
		*p = 0;
}
