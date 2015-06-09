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

#include <math.h>
#include "main.h"
#include "goat.h"

void goat_make_scared(void* _goat)
{
	Goat* goat = (Goat*)_goat;
	
	goat->scared = 1;
	goat->speed_y = -16;
	goat->animtime = 500;
	
	memset(&goat->controls, 0, sizeof(Controls));
}

void goatfn(void* _goat, int frameDelta)
{
	SDL_Rect blitrect;
	Goat* goat = (Goat*)_goat;
	
	if (goat->controls.right)
	{
		goat->speed_x += 1;
		
		if (goat->speed_x > 5)
		{
			goat->speed_x = 5;
		}
	}
	
	if (goat->controls.left)
	{
		goat->speed_x -= 1;
		
		if (goat->speed_x < -5)
		{
			goat->speed_x = -5;
		}
	}
	
	if (!goat->controls.left && !goat->controls.right)
	{
		goat->speed_x *= 0.6;
	}
	
	if (goat->controls.button1 && goat->ground)
	{
		goat->speed_y = -16;
		goat->ground = 0;
	}
	
	if (goat->ground == 0)
	{
		goat->speed_y += gravity;
		
		if (goat->speed_y > 15)
		{
			goat->speed_y = 15;
		}
	}
	
	goat->ground = 0;
	
	if (goat->speed_y > 0)
	{
		if (level[cmod((int)floor((goat->rect.y + goat->rect.h + goat->speed_y) / 16), 30)][cmod((int)floor((goat->rect.x) / 16), 40)] == '.')
		{
			goat->rect.y = 16 * floor((goat->rect.y + goat->rect.h + goat->speed_y) / 16) - goat->rect.h;
			goat->speed_y = 0;
			goat->ground = 1;
		}
		else if (level[cmod((int)floor((goat->rect.y + goat->rect.h + goat->speed_y) / 16), 30)][cmod((int)floor((goat->rect.x + goat->rect.w - 1) / 16), 40)] == '.')
		{
			goat->rect.y = 16 * floor((goat->rect.y + goat->rect.h + goat->speed_y) / 16) - goat->rect.h;
			goat->speed_y = 0;
			goat->ground = 1;
		}
	}
	else if (goat->speed_y < 0)
	{
		if (level[cmod((int)floor((goat->rect.y + goat->speed_y) / 16), 30)][cmod((int)floor((goat->rect.x) / 16), 40)] == '.')
		{
			goat->rect.y = 16 + 16 * floor((goat->rect.y + goat->speed_y) / 16);
			goat->speed_y = 0;
		}
		else if (level[cmod((int)floor((goat->rect.y + goat->speed_y) / 16), 30)][cmod((int)floor((goat->rect.x + goat->rect.w - 1) / 16), 40)] == '.')
		{
			goat->rect.y = 16 + 16 * floor((goat->rect.y + goat->speed_y) / 16);
			goat->speed_y = 0;
		}
	}
	
	if (goat->speed_x > 0)
	{
		if (level[cmod((int)floor((goat->rect.y) / 16), 30)][cmod((int)floor((goat->rect.x + goat->rect.w + goat->speed_x) / 16), 40)] == '.')
		{
			goat->rect.x = 16 * floor((goat->rect.x + goat->rect.w + goat->speed_x) / 16) - goat->rect.w;
			goat->speed_x = 0;
		}
		else if (level[cmod((int)floor((goat->rect.y + (goat->rect.h / 2) - 1) / 16), 30)][cmod((int)floor((goat->rect.x + goat->rect.w + goat->speed_x) / 16), 40)] == '.')
		{
			goat->rect.x = 16 * floor((goat->rect.x + goat->rect.w + goat->speed_x) / 16) - goat->rect.w;
			goat->speed_x = 0;
		}
		else if (level[cmod((int)floor((goat->rect.y + goat->rect.h - 1) / 16), 30)][cmod((int)floor((goat->rect.x + goat->rect.w + goat->speed_x) / 16), 40)] == '.')
		{
			goat->rect.x = 16 * floor((goat->rect.x + goat->rect.w + goat->speed_x) / 16) - goat->rect.w;
			goat->speed_x = 0;
		}
	}
	else if (goat->speed_x < 0)
	{
		if (level[cmod((int)floor((goat->rect.y) / 16), 30)][cmod((int)floor((goat->rect.x + goat->speed_x) / 16), 40)] == '.')
		{
			goat->rect.x = 16 + 16 * floor((goat->rect.x + goat->speed_x) / 16);
			goat->speed_x = 0;
		}
		else if (level[cmod((int)floor((goat->rect.y + (goat->rect.h / 2) - 1) / 16), 30)][cmod((int)floor((goat->rect.x + goat->speed_x) / 16), 40)] == '.')
		{
			goat->rect.x = 16 + 16 * floor((goat->rect.x + goat->speed_x) / 16);
			goat->speed_x = 0;
		}
		else if (level[cmod((int)floor((goat->rect.y + goat->rect.h - 1) / 16), 30)][cmod((int)floor((goat->rect.x + goat->speed_x) / 16), 40)] == '.')
		{
			goat->rect.x = 16 + 16 * floor((goat->rect.x + goat->speed_x) / 16);
			goat->speed_x = 0;
		}
	}
	
	goat->rect.x += round(goat->speed_x);
	goat->rect.y += round(goat->speed_y);
	
	if (goat->speed_x > 0 && goat->spriterect.x == 0)
	{
		goat->spriterect.x = 192;
	}
	else if (goat->speed_x < 0 && goat->spriterect.x == 192)
	{
		goat->spriterect.x = 0;
	}
	
	if (goat->scared)
	{
		goat->spriterect.y = 96;
	}
	else if (goat->speed_x > 0.1 || goat->speed_x < -0.1)
	{
		goat->spriterect.y = 32;
	}
	else
	{
		goat->spriterect.y = 0;
	}
	
	if (goat->animtime > 0 || goat->scared)
	{
		goat->animtime -= frameDelta;
	}
	else
	{
		goat->animframe = (goat->animframe + 1) % 4;
		goat->animtime = 50;
	}
	
	// SDL_FillRect(screen, &goat->rect, SDL_MapRGB(screen->format, 255, 255, 0));
	
	blitrect.x = goat->rect.x;
	blitrect.y = goat->rect.y;
	blitrect.w = goat->rect.w;
	blitrect.h = goat->rect.h;
	
	goat->spriterect.x += 48 * goat->animframe;
	
	if (goat->rect.x + goat->rect.w >= 640)
	{
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		blitrect.x = goat->rect.x - 640;
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		if (goat->rect.x >= 640)
		{
			goat->rect.x -= 640;
		}
	}
	else if (goat->rect.x < 0)
	{
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		blitrect.x = goat->rect.x + 640;
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		if (goat->rect.x + goat->rect.w <= 0)
		{
			goat->rect.x += 640;
		}
	}
	else if (goat->rect.y + goat->rect.h >= 480)
	{
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		blitrect.y = goat->rect.y - 480;
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		if (goat->rect.y >= 480)
		{
			goat->rect.y -= 480;
		}
	}
	else if (goat->rect.y < 0)
	{
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		blitrect.y = goat->rect.y + 480;
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
		
		if (goat->rect.y + goat->rect.h < 0)
		{
			goat->rect.y += 480;
		}
	}
	else
	{
		SDL_BlitSurface(sprite_goat, &goat->spriterect, screen, &blitrect);
	}
	
	goat->spriterect.x -= 48 * goat->animframe;
}

Goat* goat_create(int x, int y)
{
	Goat* goat = (Goat*) malloc(sizeof(Goat));
	memset(goat, 0, sizeof(Goat));
	
	goat->rect.x = x;
	goat->rect.y = y;
	goat->rect.w = 48;
	goat->rect.h = 32;
	
	goat->spriterect.x = 0;
	goat->spriterect.y = 0;
	goat->spriterect.w = 48;
	goat->spriterect.h = 32;
	
	return goat;
}
