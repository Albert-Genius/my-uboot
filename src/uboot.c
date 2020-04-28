/******************************************************************
 * uboot.c
 * the phase2 of bootloader
 * by zhaoyuandong at 2020/4/20
 ******************************************************************/
#include "setup.h"
#include "serial.h"
#include "init.h"

#define MARCH_TYPE_S3C2440 168	//note:the kernel's number is 168(defined by TQ2440), but the right number is 362(defined by uboot organization)
#define PARTITION_SDRAM_PARAMS_START 0x30000100
#define PARTITION_SDRAM_KERNEL_START 0x30008000

static struct tag *params;


void setup_start_tag(void)
{
	params = (struct tag *)PARTITION_SDRAM_PARAMS_START;

	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size(tag_core); /*the value is five*/

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next(params); /*point to next parameter*/
}

void setup_memory_tags(void)
{
	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size(tag_mem32);

	params->u.mem.start = 0x30000000;
	params->u.mem.size = 0x4000000; /*64M*/

	params = tag_next(params);
}	

unsigned int strlen(unsigned char *str)
{
	unsigned int i = 0;
	
	while (str[i]) /*use array mode of pointer: it's a nice way*/
		i++;
	return i;
}

void strcpy(unsigned char *dest, unsigned char *src)
{
	while ('\0' != (*dest++ = *src++));
}

void setup_commandline_tag(unsigned char *cmdline)
{
	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size = (sizeof(struct tag_header) + strlen(cmdline) + 1 + 3) >> 2;
	
	strcpy(params->u.cmdline.cmdline, cmdline);

	params = tag_next(params);
}

void setup_end_tag(void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

int main(void)
{
	//int i = 0;
	void (*theKernel)(int, int, unsigned int);
	//volatile int *pDebug = (volatile int *)PARTITION_SDRAM_KERNEL_START;

	/* 0.initialize uart */
	uart_init();

	/* 1.copy kernel image from nandflash to sdram */
	putc('\r');putc('\n');
	puts("Read kernel image ... \r\n");
	nand_read((unsigned char *)0x200000, (unsigned char *)PARTITION_SDRAM_KERNEL_START, 0x300000);
	puts("Read kernel image done. \r\n");
#if 0
	puthex(0x1234ABCD);
	putc('\r');putc('\n');

	pDebug += 2*2048/4;
	pNandData += 2*2048/4;
	for (i=0; i<64; i++) {
		puthex((unsigned int)pDebug);
		putc(':');
		puthex(*pDebug++);
		putc('\r');putc('\n');
		putc('\r');putc('\n');
	}
#endif

	/* 2.set boot kernel parameters */
	puts("Set boot kernel parameters... \r\n");
	setup_start_tag();
	setup_memory_tags();
	setup_commandline_tag("noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0");
	setup_end_tag();
	puts("Set boot kernel parameters done. \r\n");

	/* 3.jump to run kernel */	
	puts("Run kernel ... \r\n");
	theKernel = (void (*)(int, int, unsigned int))PARTITION_SDRAM_KERNEL_START;
	/* mov r0, #0
	   ldr r1, =MARCH_TYPE_S3C2440
	   ldr r2, =PARTITION_SDRAM_PARAMS_START
	   ldr pc, =PARTITION_SDRAM_KERNEL_START
	*/
	theKernel(0, MARCH_TYPE_S3C2440, PARTITION_SDRAM_PARAMS_START);

	/* never come back */
	puts("Run kernel failed! \r\n");
	
	return -1;
}

/* the end of file */
