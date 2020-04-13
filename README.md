# my-uboot
develop a mini-uboot for S3C2440

include functions as follow:
1.boot linux core.
2.start from nand flash.

develop the bootloader:
1.close the watchdog.
1.1disable all interrupt.
2.config clock.
3.init SDRAM.
4.init Nand Flash.
5.relocate.
6.copy the kernel of core from Nand Flash to SDRAM.
7.set parameters given to kernel.
8.run kernel.
