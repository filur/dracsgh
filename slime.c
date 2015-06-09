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
#include "slime.h"

void slimefn(void* _slime, int frameDelta)
{
	SDL_Rect blitrect;
	Slime* slime = (Slime*)_slime;
	
	if (slime->controls.right)
	{
		slime->speed_x += 1;
		
		if (slime->speed_x > slime->maxspeed)
		{
			slime->speed_x = slime->maxspeed;
		}
	}
	else if (slime->controls.left)
	{
		slime->speed_x -= 1;
		
		if (slime->speed_x < -slime->maxspeed)
		{
			slime->speed_x = -slime->maxspeed;
		}
	}
	else
	{
		slime->speed_x *= 0.6;
	}
	
	if (slime->controls.button1 && slime->ground)
	{
		slime->speed_y = -16;
		slime->ground = 0;
	}
	
	if (slime->ground == 0)
	{
		slime->speed_y += gravity;
		
		if (slime->speed_y > 15)
		{
			slime->speed_y = 15;
		}
	}
	
	slime->ground = 0;
	
	if (slime->speed_y > 0)
	{
		if (level[cmod((int)floor((slime->rect.y + slime->rect.h + slime->speed_y) / 16), 30)][cmod((int)floor((slime->rect.x) / 16), 40)] == '.')
		{
			slime->rect.y = 16 * floor((slime->rect.y + slime->rect.h + slime->speed_y) / 16) - slime->rect.h;
			slime->speed_y = 0;
			slime->ground = 1;
		}
		else if (level[cmod((int)floor((slime->rect.y + slime->rect.h + slime->speed_y) / 16), 30)][cmod((int)floor((slime->rect.x + slime->rect.w - 1) / 16), 40)] == '.')
		{
			slime->rect.y = 16 * floor((slime->rect.y + slime->rect.h + slime->speed_y) / 16) - slime->rect.h;
			slime->speed_y = 0;
			slime->ground = 1;
		}
	}
	else if (slime->speed_y < 0)
	{
		if (level[cmod((int)floor((slime->rect.y + slime->speed_y) / 16), 30)][cmod((int)floor((slime->rect.x) / 16), 40)] == '.')
		{
			slime->rect.y = 16 + 16 * floor((slime->rect.y + slime->speed_y) / 16);
			slime->speed_y = 0;
		}
		else if (level[cmod((int)floor((slime->rect.y + slime->speed_y) / 16), 30)][cmod((int)floor((slime->rect.x + slime->rect.w - 1) / 16), 40)] == '.')
		{
			slime->rect.y = 16 + 16 * floor((slime->rect.y + slime->speed_y) / 16);
			slime->speed_y = 0;
		}
	}
	
	if (slime->speed_x > 0)
	{
		if (level[cmod((int)floor((slime->rect.y) / 16), 30)][cmod((int)floor((slime->rect.x + slime->rect.w + slime->speed_x) / 16), 40)] == '.')
		{
			slime->rect.x = 16 * floor((slime->rect.x + slime->rect.w + slime->speed_x) / 16) - slime->rect.w;
			slime->speed_x = 0;
		}
		else if (level[cmod((int)floor((slime->rect.y + (slime->rect.h / 2) - 1) / 16), 30)][cmod((int)floor((slime->rect.x + slime->rect.w + slime->speed_x) / 16), 40)] == '.')
		{
			slime->rect.x = 16 * floor((slime->rect.x + slime->rect.w + slime->speed_x) / 16) - slime->rect.w;
			slime->speed_x = 0;
		}
		else if (level[cmod((int)floor((slime->rect.y + slime->rect.h - 1) / 16), 30)][cmod((int)floor((slime->rect.x + slime->rect.w + slime->speed_x) / 16), 40)] == '.')
		{
			slime->rect.x = 16 * floor((slime->rect.x + slime->rect.w + slime->speed_x) / 16) - slime->rect.w;
			slime->speed_x = 0;
		}
		
		if (level[cmod((int)floor((slime->rect.y + (slime->rect.h / 2) - 1) / 16), 30)][cmod((int)floor((slime->rect.x + slime->rect.w + slime->speed_x) / 16), 40)] == '!')
		{
			slime->controls.right = 0;
			slime->controls.left = 1;
		}
	}
	else if (slime->speed_x < 0)
	{
		if (level[cmod((int)floor((slime->rect.y) / 16), 30)][cmod((int)floor((slime->rect.x + slime->speed_x) / 16), 40)] == '.')
		{
			slime->rect.x = 16 + 16 * floor((slime->rect.x + slime->speed_x) / 16);
			slime->speed_x = 0;
		}
		else if (level[cmod((int)floor((slime->rect.y + (slime->rect.h / 2) - 1) / 16), 30)][cmod((int)floor((slime->rect.x + slime->speed_x) / 16), 40)] == '.')
		{
			slime->rect.x = 16 + 16 * floor((slime->rect.x + slime->speed_x) / 16);
			slime->speed_x = 0;
		}
		else if (level[cmod((int)floor((slime->rect.y + slime->rect.h - 1) / 16), 30)][cmod((int)floor((slime->rect.x + slime->speed_x) / 16), 40)] == '.')
		{
			slime->rect.x = 16 + 16 * floor((slime->rect.x + slime->speed_x) / 16);
			slime->speed_x = 0;
		}
		
		if (level[cmod((int)floor((slime->rect.y + (slime->rect.h / 2) - 1) / 16), 30)][cmod((int)floor((slime->rect.x + slime->speed_x) / 16), 40)] == '!')
		{
			slime->controls.right = 1;
			slime->controls.left = 0;
		}
	}
	
	slime->rect.x += round(slime->speed_x);
	slime->rect.y += round(slime->speed_y);
	
	// if (slime->speed_x > 0 && slime->spriterect.x == 0)
	// {
		// slime->spriterect.x = 192;
	// }
	// else if (slime->speed_x < 0 && slime->spriterect.x == 192)
	// {
		// slime->spriterect.x = 0;
	// }
	
	// if (slime->speed_x > 0.1 || slime->speed_x < -0.1)
	// {
		// slime->spriterect.y = 32;
	// }
	// else
	// {
		// slime->spriterect.y = 0;
	// }
	
	if (slime->animtime > 0)
	{
		slime->animtime -= frameDelta;
	}
	else
	{
		slime->animframe = (slime->animframe + 1) % 2;
		slime->animtime = 150;
	}
	
	// SDL_FillRect(screen, &slime->rect, SDL_MapRGB(screen->format, 255, 255, 0));
	
	blitrect.x = slime->rect.x;
	blitrect.y = slime->rect.y;
	blitrect.w = slime->rect.w;
	blitrect.h = slime->rect.h;
	
	slime->spriterect.x += 32 * slime->animframe;
	
	if (slime->rect.x + slime->rect.w >= 640)
	{
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		blitrect.x = slime->rect.x - 640;
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		if (slime->rect.x >= 640)
		{
			slime->rect.x -= 640;
		}
	}
	else if (slime->rect.x < 0)
	{
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		blitrect.x = slime->rect.x + 640;
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		if (slime->rect.x + slime->rect.w <= 0)
		{
			slime->rect.x += 640;
		}
	}
	else if (slime->rect.y + slime->rect.h >= 480)
	{
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		blitrect.y = slime->rect.y - 480;
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		if (slime->rect.y >= 480)
		{
			slime->rect.y -= 480;
		}
	}
	else if (slime->rect.y < 0)
	{
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		blitrect.y = slime->rect.y + 480;
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
		
		if (slime->rect.y + slime->rect.h < 0)
		{
			slime->rect.y += 480;
		}
	}
	else
	{
		SDL_BlitSurface(sprite_slime, &slime->spriterect, screen, &blitrect);
	}
	
	slime->spriterect.x -= 32 * slime->animframe;
}

Slime* slime_create(int x, int y, float maxspeed)
{
	Slime* slime = (Slime*) malloc(sizeof(Slime));
	memset(slime, 0, sizeof(Slime));
	
	slime->maxspeed = maxspeed;
	
	slime->rect.x = x;
	slime->rect.y = y;
	slime->rect.w = 32;
	slime->rect.h = 24;
	
	slime->spriterect.x = 0;
	slime->spriterect.y = 0;
	slime->spriterect.w = 32;
	slime->spriterect.h = 24;
	
	return slime;
}
