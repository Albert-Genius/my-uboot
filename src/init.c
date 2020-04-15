/******************************************************************
 * init.c
 * device initial
 * by zhaoyuandong at 2020/4/14
 ******************************************************************/
void nand_init(void)
{
	//wait to code
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
