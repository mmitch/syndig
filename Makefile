pkgconfigs := libpulse-simple alsa
warnings   := -Wall -Wextra

CFLAGS  += $(shell pkg-config --cflags $(pkgconfigs)) $(warnings)
LDFLAGS += $(shell pkg-config --libs $(pkgconfigs))

sources := $(wildcard *.c)
objects := $(sources:.c=.o)
depends := $(sources:.c=.d)
binary  := synth

all:	build

include $(depends)

.PHONY: all build clean

build:	$(binary)

$(binary): $(objects)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~
	rm -f $(binary)
	rm -f $(objects)
	rm -f $(depends)

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
