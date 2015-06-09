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

#ifndef _HAVE_MAIN_H
#define _HAVE_MAIN_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "sound.h"

typedef struct
{
	int up, down, left, right, button1, button2, buttonctrl;
} Controls;

typedef struct
{
	float x, y, speed_x, speed_y, jumppower;
	int ground, animtime, animframe, shooting, shootwait, hurting, hurttime, hearts, score;
	SDL_Rect rect;
	SDL_Rect spriterect;
} Player;

SDL_Surface* screen;
SDL_Surface* fontsurf;
SDL_Surface* sprite_player;
SDL_Surface* sprite_goat;
SDL_Surface* sprite_slime;
SDL_Surface* gfx_brick;
SDL_Surface* gfx_goat_count;
SDL_Surface* gfx_heart;

TTF_Font* font;
Controls controls;
Player player;
Synth* sfx_player_jump;
Synth* sfx_player_scare;
Synth* sfx_player_scare2;
Synth* sfx_clear_goat1;
Synth* sfx_clear_goat2;
Synth* sfx_hurting;
Synth* sfx_c4;
Synth* sfx_f4;

char** level;
int numgoats, activegoats, newlevelpause, nthlevel;
float slimespeedmin, slimespeedmax;
float gravity;
void (*framefn)(int);

int cmod(int a, int b);

#else
#endif
