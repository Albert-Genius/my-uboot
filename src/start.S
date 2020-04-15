@ start.S
@ bootloader phase1
@ by zhaoyuandong at 2020/4/13

.text
.global _start
_start:

@ 1.close watch dog
#define WTCON 0x53000000
	ldr r0, =WTCON
	mov r1, #0x0
	str r1, [r0]

@ 2.set clock
#define CLKDIVN 0x4C000014
	@ set clk divisor FCLK:HCLK:PCLK=1:2:4
	ldr r0, =CLKDIVN
	mov r1, #0x3
	str r1, [r0]
#define MPLLCON 0x4C000004
#define MPLL_200MHZ (0x5c<<12 | 0x1<<4 | 0x2)
	@ set MPLL to 200MHZ (Fin=12MHz)
	ldr r0, =MPLLCON
	ldr r1, =MPLL_200MHZ
	str r1, [r0]

@ 3.init SDRAM
#define BWSCON 0x48000000
	ldr r0, =BWSCON
	adr r1, sdram_config
	add r2, r1, #13*4
0:	
	ldr r3, [r1], #4
	str r3, [r0], #4
	cmp r1, r2
	bne 0b

@ 4.relocate
#define SDRAM_START 0x30000000
#define SDRAM_END   0x34000000
	ldr sp, =SDRAM_END	@set stack pointer
	bl nand_init		@init nand controler
	mov r0, #0x0
	ldr r1, =_start
	ldr r2, =__bss_start
	sub r2, r2, r1	
	bl copy2sdram
	bl clean_bss

@ 5.jump to main
	ldr lr, =halt
	ldr pc, =main
halt:
	b halt



.algin 4
sdram_config:
    .long   0x22011110      @ BWSCON
    .long   0x00000700      @ BANKCON0
    .long   0x00000700      @ BANKCON1
    .long   0x00000700      @ BANKCON2
    .long   0x00000700      @ BANKCON3  
    .long   0x00000700      @ BANKCON4
    .long   0x00000700      @ BANKCON5
    .long   0x00018005      @ BANKCON6
    .long   0x00018005      @ BANKCON7
    .long   0x008C07A3      @ REFRESH
    .long   0x000000B1      @ BANKSIZE
    .long   0x00000030      @ MRSRB6
    .long   0x00000030      @ MRSRB7
	
