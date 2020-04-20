/******************************************************************
 * uboot.c
 * the phase2 of bootloader
 * by zhaoyuandong at 2020/4/20
 ******************************************************************/
#include "setup.h"

#define MARCH_TYPE_S3C2440 362
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

	params->u.memory.start = 0x30000000;
	params->u.memory.size = 0x4000000; /*64M*/

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
	while (*dest++ = *src++);
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
	params->hdr.size = 0;{	
}

void main(void)
{
	void (*theKernel)(int, int, unsigned int);

	/* 1.copy kernel image from nandflash to sdram */
	nand_read((unsigned char *)0x200000+64, (unsigned char *)PARTITION_SDRAM_KERNEL_START, 0x300000);

	/* 2.set boot kernel parameters */
	setup_start_tag();
	setup_memory_tags();
	setup_commandline_tag("noinitrd root=/dev/mtdblock2 init=/linuxrc console=ttySAC0");
	setup_end_tag();

	/* 3.jump to run kernel */	
	theKernel = (void (*)(int, int, unsigned int))PARTITION_SDRAM_KERNEL_START;
	theKernel(0, MARCH_TYPE_S3C2440, PARTITION_SDRAM_PARAMS_START);

	/* never come back */
}
