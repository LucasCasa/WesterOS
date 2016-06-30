include ../Makefile.inc

MODULE=tarImageModule.bin
SOURCES= $(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
STATICLIBS=

all: $(MODULE)

$(MODULE): $(OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T images.ld -o ../$(MODULE) manager.o $(STATICLIBS)
#
%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

clean:
	rm -rf *.o

.PHONY: all clean
