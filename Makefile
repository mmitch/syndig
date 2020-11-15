pkgconfigs := libpulse-simple
warnings   := -Wall -Wextra

CFLAGS  += $(shell pkg-config --cflags $(pkgconfigs)) $(warnings)
LDFLAGS += $(shell pkg-config --libs $(pkgconfigs))

sources := $(wildcard *.c)
objects := $(sources:.c=.o)
binary  := synth

all:	build

.PHONY: all build clean

build:	$(binary)

$(binary): $(objects)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~
	rm -f $(binary)
	rm -f $(objects)
