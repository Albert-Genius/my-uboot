# my-uboot
develop a mini-uboot for S3C2440

include functions as follow:
1.boot linux core.
2.start from nand flash.


develop the bootloader:
1.close the watchdog.
2.1disable all interrupt.
3.config clock.
4.init SDRAM.
5.init Nand Flash.
6.relocate.
7.copy the kernel of core from Nand Flash to SDRAM.
8.set parameters given to kernel.
9.run kernel.
