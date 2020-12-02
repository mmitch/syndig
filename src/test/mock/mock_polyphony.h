/*
 * mock_polyphony.h - mocks of polyphony.h
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

#include "../../polyphony.h"

#include "../../thirdparty/fff.h"
DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(play_note, uint8_t, float)
FAKE_VOID_FUNC(stop_note, uint8_t)
FAKE_VOID_FUNC(stop_all_notes)
FAKE_VOID_FUNC(set_polyphony_mode, polyphony_mode)

void reset_polyphony_mocks() {
	RESET_FAKE(play_note)
	RESET_FAKE(stop_note)
	RESET_FAKE(stop_all_notes)
	RESET_FAKE(set_polyphony_mode)
}
	
