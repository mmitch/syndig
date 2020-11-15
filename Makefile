pkgconfigs := libpulse-simple alsa
warnings   := -Wall -Wextra

CFLAGS  += $(shell pkg-config --cflags $(pkgconfigs)) $(warnings)
LDFLAGS += $(shell pkg-config --libs $(pkgconfigs))

srcdir   := ./src
builddir := ./build
bindir   := ./bin

sources := $(wildcard $(srcdir)/*.c)
objects := $(addprefix $(builddir)/,$(notdir $(sources:.c=.o)))
depends := $(objects:.o=.d))
binary  := $(bindir)/synth

all:	$(binary)

include $(depends)

.PHONY: all clean

$(binary): $(objects) | $(bindir)
	$(CC) -o $@ $^ $(LDFLAGS)

$(bindir):
	mkdir -p $@

$(builddir):
	mkdir -p $@

$(objects) $(depends): | $(builddir)

clean:
	rm -f  *~
	rm -f  $(srcdir)/*~
	rm -rf $(bindir)
	rm -rf $(builddir)

$(builddir)/%.o: $(srcdir)/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

$(builddir)/%.d: $(srcdir)/%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
