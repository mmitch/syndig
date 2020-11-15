pkgconfigs := libpulse-simple
warnings   := -Wall -Wextra

CFLAGS  += $(shell pkg-config --cflags $(pkgconfigs)) $(warnings)
LDFLAGS += $(shell pkg-config --libs $(pkgconfigs))

sources := $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(sources))
binary  := synth

all:	build

build:	$(binary)

$(binary): $(objects)

clean:
	rm -f *~
	rm -f $(binary)
	rm -f $(objects)
