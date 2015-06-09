/**
 * Copyright 2015 Mikael Forsberg
 *
 * This file is part of Dracula Super Goat Hunter.
 *
 * Dracula Super Goat Hunter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Dracula Super Goat Hunter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Dracula Super Goat Hunter.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _HAVE_SOUND_H
#define _HAVE_SOUND_H

#define SYNTH_SQUARE 1
#define SYNTH_NOISE 2

typedef struct
{
	float length, pos, delay, usedelay, freq, amp, freq_start, freq_end, amp_start, amp_end;
	int waveform, active, phase;
} Synth;

int sound_setup();
int sound_quit();
Synth* sound_synth_get(int n);
int sound_synth_play(Synth* synth);
int sound_set_song(int n);

#else
#endif
