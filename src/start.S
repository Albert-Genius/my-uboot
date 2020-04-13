# start.S
# bootloader phase1
# by zhaoyuandong at 2020/4/13

.text
.global _start
_start:

# 1.close watch dog
#define WTCON 0x53000000
	ldr r0, =WTCON
	mov r1, #0x0
	str r1, [r0]

# 2.set clock
#define CLKDIVN 0x4C000014
	# set clk divisor FCLK:HCLK:PCLK=1:2:4
	ldr r0, =CLKDIVN
	mov r1, #0x3
	str r1, [r0]
#define MPLLCON 0x4C000004
#define MPLL_200MHZ (0x5c<<12 | 0x1<<4 | 0x2)
	# set MPLL to 200MHZ (Fin=12MHz)
	ldr r0, =MPLLCON
	ldr r1, =MPLL_200MHZ
	str r1, [r0]

# 3.init SDRAM

# 4.relocate

# 5.jump to main
