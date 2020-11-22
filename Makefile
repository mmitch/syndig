# bash is only needed for autobuild target (read needs -t)
SHELL := bash

pkgconfigs := libpulse-simple alsa
warnings   := -Wall -Wextra -Wpedantic -Werror

CFLAGS  += $(shell pkg-config --cflags $(pkgconfigs)) $(warnings) -O3 -fPIC
LDFLAGS += $(shell pkg-config --libs $(pkgconfigs)) -lm -fPIC

srcdir   := ./src
builddir := ./build
bindir   := ./bin

sources := $(wildcard $(srcdir)/*.c)
objects := $(addprefix $(builddir)/,$(notdir $(sources:.c=.o)))
depends := $(objects:.o=.d))
binary  := $(bindir)/synth

all:	$(binary)

autobuild:
	-$(MAKE) all
	inotifywait -m -e modify -e create -e delete -e close_write -e move -r . \
	| grep --line-buffered -E '(Makefile|\.[cdho])$$' \
	| while read -r EVENT; do \
		while read -r -t 0.1 EVENT; do :; done; \
		$(MAKE) all; \
	done

include $(depends)

.PHONY: all clean autobuild

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

$(builddir)/%.o: $(srcdir)/%.c $(builddir)/%.d
	$(CC) $(CFLAGS) -c -o $@ $<

$(builddir)/%.d: $(srcdir)/%.c
	@echo dep $@
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(objects) $(depends): Makefile
