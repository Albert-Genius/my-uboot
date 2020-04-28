
CC = arm-linux-gcc
LD = arm-linux-ld
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump

CFLAGS := -Wall -O2
CPPFLAGS := -nostdinc -fno-builtin

objs = start.o init.o uboot.o serial.o


uboot.bin: $(objs)
	$(LD) -Tuboot.lds -o uboot.elf $^
	$(OBJCOPY) -O binary -S uboot.elf $@
	$(OBJDUMP) -D -m arm uboot.elf > uboot.dis

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<
%.o: %.S
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf *.dis *.bin
