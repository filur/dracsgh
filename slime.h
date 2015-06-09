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

#ifndef _HAVE_SLIME_H
#define _HAVE_SLIME_H

#include "SDL.h"
#include "main.h"

typedef struct
{
	float speed_x, speed_y, maxspeed;
	int ground, nextthink, animframe, animtime;
	Controls controls;
	SDL_Rect rect;
	SDL_Rect spriterect;
	
} Slime;

void slimefn(void* slime, int frameDelta);
Slime* slime_create(int x, int y, float maxspeed);

#else
#endif
