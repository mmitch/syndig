# Makefile - build script for syndig
#
# Copyright (C) 2020  Christian Garbs <mitch@cgarbs.de>
# Licensed under GNU GPL v3 (or later)
#
# This file is part of syndig, a simple software synthesizer written in C.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# bash is only needed for autobuild target (read needs -t)
SHELL := bash

pkgconfigs := libpulse-simple alsa
warnings   := -Wall -Wextra -Wpedantic -Werror

CFLAGS  += $(shell pkg-config --cflags $(pkgconfigs)) $(warnings)
LDFLAGS += $(shell pkg-config --libs $(pkgconfigs)) -lm

# auto-optimize only when no other optimization level is given
# this enables -O0 to be set for coverage builds
ifeq (,$(findstring -O,$(CFLAGS)))
	CFLAGS  += -O3 -fPIC
	LDFLAGS += -fPIC
endif

srcdir   := ./src
builddir := ./build
bindir   := ./bin

sources  := $(wildcard $(srcdir)/*.c)
objects  := $(addprefix $(builddir)/,$(notdir $(sources:.c=.o)))
depends  := $(objects:.o=.d))
binary   := $(bindir)/syndig

testsrcdir   := $(srcdir)/test
testbuilddir := $(builddir)/test
testbindir   := $(bindir)/test

testsources  := $(wildcard $(testsrcdir)/*.c)
testobjects  := $(addprefix $(testbuilddir)/,$(notdir $(testsources:.c=.o)))
testdepends  := $(testobjects:.o=.d))
testbinaries := $(addprefix $(testbindir)/,$(notdir $(testsources:.c=)))

define gendep =
@echo dep $@
@set -e; rm -f $@; \
$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
rm -f $@.$$$$
endef

define compile =
$(CC) $(CFLAGS) -c -o $@ $<
endef

define link =
$(CC) -o $@ $^ $(LDFLAGS)
endef

all:	$(binary) test

autobuild:
	-$(MAKE) all
	inotifywait -m -e modify -e create -e delete -e close_write -e move -r . \
	| grep --line-buffered -E '(Makefile|\.[cdho])$$' \
	| while read -r EVENT; do \
		while read -r -t 0.1 EVENT; do :; done; \
		$(MAKE) all; \
	done

include $(depends) $(testdeps)

.PHONY: all clean autobuild test

test: $(testbinaries)
	@for TEST in $^; do $$TEST || exit 1; rm $$TEST; done

$(binary): $(objects) | $(bindir)
	$(link)

$(testbindir)/%: $(testbuilddir)/%.o
	$(link)

$(bindir) $(builddir) $(testbindir) $(testbuilddir):
	mkdir -p $@

$(objects) $(depends): | $(builddir)

$(testobjects) $(testdepends): | $(testbuilddir)

$(testbinaries): | $(testbindir)

clean:
	rm -f  *~
	rm -f  $(srcdir)/*~
	rm -rf $(bindir)
	rm -rf $(builddir)

$(builddir)/%.o: $(srcdir)/%.c $(builddir)/%.d
	$(compile)

$(testbuilddir)/%.o: $(testsrcdir)/%.c $(testbuilddir)/%.d
	$(compile)

$(builddir)/%.d: $(srcdir)/%.c
	$(gendep)

$(testbuilddir)/%.d: $(testsrcdir)/%.c
	$(gendep)

$(objects) $(depends) $(testdepends): Makefile
