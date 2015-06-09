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

#include <stdint.h>
#include <math.h>
#include "SDL.h"
#include "sound.h"

#define SOUND_SYNTH_MAX 16

Synth* synths[SOUND_SYNTH_MAX];

// uint8_t* song1buf;
// uint32_t song1len;
// uint32_t song1pos;

uint8_t* song2buf;
uint32_t song2len;
uint32_t song2pos;

uint8_t* song3buf;
uint32_t song3len;
uint32_t song3pos;

int whichsong;

void sound_proc(void* userdata, Uint8* stream, int len)
{
	int i, j, numsamples, noise_wait;
	int16_t* s16stream;
	int16_t* songstream;
	float signal;
	
	noise_wait = 0;
	s16stream = (int16_t*) stream;
	
	numsamples = len / 2;
	// memset(stream, 0, len);
	
	if (whichsong != 0)
	{
		switch (whichsong)
		{
			case 2:
				songstream = (int16_t*) song2buf;
				
				for (i = 0; i < numsamples; i += 2)
				{
					s16stream[i] = songstream[song2pos];
					s16stream[i + 1] = songstream[song2pos];
					
					song2pos = (song2pos + 1) % song2len;
				}
				
				break;
			
			case 3:
			default:
				songstream = (int16_t*) song3buf;
				
				for (i = 0; i < numsamples; i += 2)
				{
					s16stream[i] = songstream[song3pos];
					s16stream[i + 1] = songstream[song3pos];
					
					song3pos = (song3pos + 1) % song3len;
				}
				
				break;
		}
	}
	
	for (j = 0; j < SOUND_SYNTH_MAX; ++j)
	{
		if (!synths[j]->active)
		{
			continue;
		}
		
		// printf("synth %d is active\n", j);
		
		for (i = 0; i < numsamples; i += 2)
		{
			if (synths[j]->usedelay > 0)
			{
				synths[j]->usedelay -= 1.0 / 44.1;
			}
			else
			{
				if (synths[j]->waveform == SYNTH_SQUARE)
				{
					signal = synths[j]->amp * round((sin(synths[j]->freq * 6.283 * synths[j]->phase / 44100) + 1) / 2);
					
					// if (sin(synths[j]->freq * 6.283 * synths[j]->phase / 44100) >= 0)
					// {
						// signal = synths[j]->amp / 2 * 1.0;
					// }
					// else
					// {
						// signal = synths[j]->amp / 2 * -1.0;
					// }
				}
				if (synths[j]->waveform == SYNTH_NOISE)
				{
					if (noise_wait > 0)
					{
						--noise_wait;
					}
					else
					{
						if ((rand() % 2) == 0)
						{
							signal = synths[j]->amp * 1.0;
						}
						else
						{
							signal = synths[j]->amp * -1.0;
						}
						
						noise_wait = 44100 / synths[j]->freq;
					}
				}
				
				// if (s16stream[i] != 0)
				// {
					s16stream[i] += 32767 * signal;
					s16stream[i + 1] += 32767 * signal;
				// }
				// else
				// {
					// u16stream[i] = 32768 + 32767 * signal;
					// u16stream[i + 1] = 32768 + 32767 * signal;
				// }
				
				synths[j]->phase = (synths[j]->phase + 1) % 44100;
				synths[j]->pos += 1.0 / 44.1;
				
				if (synths[j]->pos >= synths[j]->length)
				{
					synths[j]->active = 0;
					break;
				}
				
				synths[j]->amp += (synths[j]->amp_end - synths[j]->amp_start) * (1000 / (float)synths[j]->length / 44100.0);
				// printf("%.2f\n", synths[j]->amp);
				synths[j]->freq += (synths[j]->freq_end - synths[j]->freq_start) * (1000 / (float)synths[j]->length / 44100.0);
				// printf("%.2f\n", synths[j]->amp);
			}
		}
	}
}

int sound_setup()
{
	SDL_AudioSpec desired;
	SDL_AudioSpec obtained;
	int i;
	
	desired.freq = 44100;
	desired.format = AUDIO_S16SYS;
	desired.channels = 2;
	desired.samples = 2048;
	desired.callback = &sound_proc;
	
	for (i = 0; i < SOUND_SYNTH_MAX; ++i)
	{
		synths[i] = (Synth*) malloc(sizeof(Synth));
		memset(synths[i], 0, sizeof(Synth));
	}
	
	SDL_OpenAudio(&desired, &obtained);
	// printf("%d %d %d\n", obtained.freq, obtained.channels, obtained.samples);
	
	// song1buf = (uint8_t*) malloc(2709600);
	// SDL_LoadWAV("song1mono.wav", &obtained, &song1buf, &song1len);
	// song1len = song1len / 2;
	// song1pos = 0;
	
	song2buf = (uint8_t*) malloc(723800);
	SDL_LoadWAV("song2mono.wav", &obtained, &song2buf, &song2len);
	song2len = song2len / 2;
	song2pos = 0;
	
	song3buf = (uint8_t*) malloc(1254600);
	SDL_LoadWAV("song3mono.wav", &obtained, &song3buf, &song3len);
	song3len = song3len / 2;
	song3pos = 0;
	
	whichsong = 0;
	
	SDL_PauseAudio(0);
	
	return 1;
}

Synth* sound_synth_get(int n)
{
	if (n >= 0 && n < SOUND_SYNTH_MAX)
	{
		return synths[n];
	}
	else
	{
		return NULL;
	}
}

int sound_synth_play(Synth* synth)
{
	synth->freq = synth->freq_start;
	synth->amp = synth->amp_start;
	synth->pos = 0;
	synth->phase = 0;
	synth->usedelay = synth->delay;
	
	synth->active = 1;
	
	return 1;
}

int sound_set_song(int n)
{
	if (n >= 0 && n != 1 && n < 4)
	{
		song2pos = 0;
		song3pos = 0;
		
		whichsong = n;
	}
}

int sound_quit()
{
	int i;
	
	SDL_PauseAudio(1);
	
	for (i = 0; i < SOUND_SYNTH_MAX; ++i)
	{
		free(synths[i]);
	}
	
	return 1;
}
