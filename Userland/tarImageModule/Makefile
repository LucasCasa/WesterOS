include ../Makefile.inc

MODULE=tarImageModule.bin
SOURCES= $(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
STATICLIBS=

all: $(MODULE)

$(MODULE): $(OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T images.ld -o ../$(MODULE) 01greyjoy.o 02tyrell.o 03lannister.o 04stark.o 05tar.o $(STATICLIBS)
#
%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

clean:
	rm -rf *.o

.PHONY: all clean
