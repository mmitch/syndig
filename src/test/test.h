/*
 * test.h - unit test interface
 *
 * Copyright (C) 2020  Christian Garbs <mitch@cgarbs.de>
 * Licensed under GNU GPL v3 (or later)
 *
 * This file is part of syndig, a simple software synthesizer written in C.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "stdio.h"
#define START_TESTS         { puts(__FILE__ ": start tests"); }
#define RUN_TEST(testcase)  { char *result = testcase(); if (result == NULL) { puts("  " #testcase " ok "); } else { puts( "  ERROR IN " #testcase ":" ); puts(result); return -1; }}
#define ASSERT(condition)   { if (! (condition)) { return "ASSERTION FAILED: " #condition; } }
#define END_TESTS           { puts(__FILE__ ": all tests ok"); return 0; }
