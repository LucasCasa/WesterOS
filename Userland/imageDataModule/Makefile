include ../Makefile.inc

MODULE=imageDataModule.bin
SOURCES= $(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
STATICLIBS=

all: $(MODULE)

$(MODULE): $(OBJECTS) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T images.ld  -o ../$(MODULE) $(OBJECTS) $(STATICLIBS) 

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

clean:
	rm -rf *.o

.PHONY: all clean
