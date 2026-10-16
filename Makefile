PS3DEV ?= /usr/local/ps3dev
PPU_CC = $(PS3DEV)/ppu/bin/ppu-gcc
PPU_STRIP = $(PS3DEV)/ppu/bin/ppu-strip

TARGET = RE4-Demake-PS3
OBJS = main.o

CFLAGS = -I$(PS3DEV)/ppu/include -O2 -Wall
LDFLAGS = -L$(PS3DEV)/ppu/lib -lrsx -lgcm_sys -lio -lsysutil -lrt -llv2 -lm

all: $(TARGET).self

$(TARGET).self: $(TARGET).elf
	$(PPU_STRIP) -o $@ $<

$(TARGET).elf: $(OBJS)
	$(PPU_CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(PPU_CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).self
