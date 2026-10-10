PS3DEV ?= /usr/local/ps3dev
PPU_CC = $(PS3DEV)/ppu/bin/ppu-gcc
PPU_LD = $(PS3DEV)/ppu/bin/ppu-ld

TARGET = RE4-Demake-PS3
OBJS = main.o

CFLAGS = -I$(PS3DEV)/ppu/include -I$(PS3DEV)/ppu/include/tiny3d -O2 -Wall
LDFLAGS = -L$(PS3DEV)/ppu/lib -L$(PS3DEV)/ppu/lib/tiny3d -ltiny3d -lrsx -lgcm_sys -lio -lsysutil -lrt -llv2 -lm

all: $(TARGET).self

$(TARGET).self: $(TARGET).elf
	$(PS3DEV)/ppu/bin/ppu-strip -o $(TARGET).self $<

$(TARGET).elf: $(OBJS)
	$(PPU_CC) $(OBJS) $(LDFLAGS) -o $@

%.o: %.c
	$(PPU_CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).self
