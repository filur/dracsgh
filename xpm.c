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
#include "SDL.h"
#include "xpm.h"

SDL_Surface* sdl_read_xpm(char** xpmdoc, SDL_PixelFormat* format)
{
	SDL_Surface* tmpsurface;
	SDL_Surface* result;
	int i, j, w, h, x, y, numcolors, symsize;
	char** sdlcolorkeys;
	uint32_t* sdlcolorvalues;
	uint32_t* pixeldata;
	char key[8];
	int red, green, blue, color;
	
	memset(key, 0, sizeof(char) * 8);
	
	sscanf(xpmdoc[0], "%d %d %d %d", &w, &h, &numcolors, &symsize);
	
	sdlcolorkeys = (char**) malloc(sizeof(char*) * numcolors);
	sdlcolorvalues = (uint32_t*) malloc(sizeof(uint32_t) * numcolors);
	
	for (i = 0; i < numcolors; ++i)
	{
		sdlcolorkeys[i] = (char*) malloc(sizeof(char) * symsize + 1);
		memset(sdlcolorkeys[i], 0, sizeof(char) * symsize + 1);
		
		for (j = 0; j < symsize; ++j)
		{
			sdlcolorkeys[i][j] = xpmdoc[i + 1][j];
		}
		
		sscanf(xpmdoc[i + 1] + 3, "%*c %*c%2x%2x%2x", &red, &green, &blue);
		sdlcolorvalues[i] = SDL_MapRGB(format, red, green, blue);
	}
	
	tmpsurface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
	result = SDL_DisplayFormat(tmpsurface);
	
	SDL_FreeSurface(tmpsurface);
	
	pixeldata = (uint32_t*) result->pixels;
	
	for (y = 0; y < h; ++y)
	{
		for (x = 0; x < w; ++x)
		{
			for (i = 0; i < symsize; ++i)
			{
				key[i] = xpmdoc[numcolors + 1 + y][(symsize * x) + i];
			}
			
			color = 0;
			
			for (i = 0; i < numcolors; ++i)
			{
				if (strcmp(sdlcolorkeys[i], key) == 0)
				{
					color = sdlcolorvalues[i];
					break;
				}
			}
			
			pixeldata[y * w + x] = color;
		}
	}
	
	for (i = 0; i < numcolors; ++i)
	{
		free(sdlcolorkeys[i]);
	}
	
	free(sdlcolorkeys);
	free(sdlcolorvalues);
	
	return result;
}
