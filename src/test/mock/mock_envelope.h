/*
 * mock_envelope.h - mocks of envelope.h
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

#include "../../envelope.h"

#include "../../thirdparty/fff.h"
DEFINE_FFF_GLOBALS

FAKE_VOID_FUNC(set_envelope_attack,  uint16_t)
FAKE_VOID_FUNC(set_envelope_release, uint16_t)
FAKE_VOID_FUNC(set_envelope_decay,   uint16_t)
FAKE_VOID_FUNC(set_envelope_sustain, float)
FAKE_VALUE_FUNC(float, envelope_nextval, lane_id)
FAKE_VALUE_FUNC(bool, envelope_is_running, lane_id)

void reset_envelope_mocks() {
	RESET_FAKE(set_envelope_attack)
	RESET_FAKE(set_envelope_release)
	RESET_FAKE(set_envelope_decay)
	RESET_FAKE(set_envelope_sustain)
	RESET_FAKE(envelope_nextval)
	RESET_FAKE(envelope_is_running)
}
