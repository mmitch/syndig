a simple software synthesizer in C
==================================

[![Code coverage status](https://codecov.io/github/mmitch/syndig/coverage.svg)](https://codecov.io/github/mmitch/syndig?branch=master)
[![Linux Build status](https://github.com/mmitch/syndig/workflows/Linux%20Build/badge.svg)](https://github.com/mmitch/syndig/actions?query=workflow%3A%22Linux+Build%22)
[![CodeQL status](https://github.com/mmitch/syndig/workflows/CodeQL/badge.svg)](https://github.com/mmitch/syndig/actions?query=workflow%3ACodeQL)

I started to make some music using (analog) synths, but I only have
monophonic synthesizers and a drum machine.  I still need something
simple and polyphonic for strings and chords.

I already wrote a simple interactive sequencer in Perl, so why not try
to roll my own soft-synth?  It's a lot more fun to tinker than to
choose an existing soft-synth and read a manual ;-)


project home: https://github.com/mmitch/syndig


MIDI implementation
-------------------

### Oscillator types

Oscillator types can be selected via Program Change messages:

- 000: square
- 001: sawtooth down
- 002: sawtooth up
- 003: triangle
- 004: sine
- 005: noise
- 006: impulse
- 007: square wavelet 25% duty
- 008: square wavelet 50% duty
- 009: sawtooth down wavelet
- 010: sawtooth up wavelet
- 011: triangle wavelet
- 012: sine wavelet
- 013: noise wavelet
- 014: double pulse wavelet

Wavelets are small PCM samples with a length of 8 samples.  
Unmapped programs wrap to 000.


### ADSR envelope

To control the ADSR envelope, send Control Change events with the
given parameter numbers to change these values:

- 072: release time
- 073: attack time
- 075: sustain level
- 076: decay time

The times map non-linearly from value 000 (0 seconds/off) to value 127
(about 5 seconds).  
The sustain level maps linearly from value 000 (envelope off) to value
127 (maximum envelope).


### Polyphony mode

Polyphony mode can be selected via Control Change event for
parameter 003.  The polyphony mode chooses the note to kill
prematurely to make room for a new note when polyphony is at maximum
capacity:

- 000: kill oldest note
- 001: kill lowest note
- 002: kill highest note
- 003: kill in a round robin fashion


### Other commands

These Control Change parameters are also honored:

- 007: channel volume
- 120: all sound off
- 123: all notes off


copyright
---------

syndig  -  a simple software synthesizer in C  
Copyright (C) 2020  Christian Garbs <mitch@cgarbs.de>  
Licensed under GNU GPL v3 (or later)  

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
