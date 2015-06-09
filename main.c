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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "SDL.h"
#include "SDL_ttf.h"

#include "main.h"
#include "xpm.h"
#include "sound.h"
#include "goat.h"
#include "slime.h"

#include "levels.h"
#include "sprites.h"

Goat** mygoats;
Slime** myslimes;

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int cmod(int a, int b)
{
	int result;
	
	result = a % b;
	
	if (result < 0)
	{
		result += b;
	}
	
	return result;
}

void playerfn(int frameDelta)
{
	SDL_Rect blitrect;
	SDL_Rect portalblitrect;
	SDL_Rect portalspriterect;
	int blink;
	
	if (controls.right)
	{
		player.speed_x += 1;
		
		if (player.speed_x > 5)
		{
			player.speed_x = 5;
		}
	}
	else if (controls.left)
	{
		player.speed_x -= 1;
		
		if (player.speed_x < -5)
		{
			player.speed_x = -5;
		}
	}
	else
	{
		player.speed_x *= 0.6;
	}
	
	if (controls.button1 && player.ground)
	{
		sound_synth_play(sfx_player_jump);
		
		player.speed_y = -10;
		player.jumppower = 2.5;
		player.ground = 0;
	}
	else if (controls.button1 && player.speed_y < 0 && player.jumppower > 0)
	{
		player.speed_y -= player.jumppower / 1.7;
		player.jumppower *= 0.89;
	}
	
	if (!player.shootwait && controls.button2 && player.shooting == 0)
	{
		player.shooting = 1;
		player.shootwait = 60;
		
		if (nthlevel >= 6)
		{
			player.shootwait += nthlevel;
		}
		
		player.animtime = 250;
		sound_synth_play(sfx_player_scare);
		sound_synth_play(sfx_player_scare2);
	}
	else if (player.shootwait > 0)
	{
		--player.shootwait;
	}
	
	if (player.ground == 0)
	{
		player.speed_y += gravity;
		
		if (player.speed_y > 15)
		{
			player.speed_y = 15;
		}
	}
	
	player.ground = 0;
	
	if (1/*player.speed_y > 0*/)
	{
		if (level[cmod((int)floor((player.rect.y + player.rect.h + player.speed_y) / 16), 30)][cmod((int)floor((player.rect.x) / 16), 40)] == '.')
		{
			player.rect.y = 16 * floor((player.rect.y + player.rect.h + player.speed_y) / 16) - player.rect.h;
			player.speed_y = 0;
			player.ground = 1;
			player.jumppower = 0;
		}
		else if (level[cmod((int)floor((player.rect.y + player.rect.h + player.speed_y) / 16), 30)][cmod((int)floor((player.rect.x + player.rect.w - 1) / 16), 40)] == '.')
		{
			player.rect.y = 16 * floor((player.rect.y + player.rect.h + player.speed_y) / 16) - player.rect.h;
			player.speed_y = 0;
			player.ground = 1;
			player.jumppower = 0;
		}
	}
	
	/*else */if (player.speed_y < 0)
	{
		if (level[cmod((int)floor((player.rect.y + player.speed_y) / 16), 30)][cmod((int)floor((player.rect.x) / 16), 40)] == '.')
		{
			player.rect.y = 16 + 16 * floor((player.rect.y + player.speed_y) / 16);
			player.speed_y = 0;
			player.jumppower = 0;
		}
		else if (level[cmod((int)floor((player.rect.y + player.speed_y) / 16), 30)][cmod((int)floor((player.rect.x + player.rect.w - 1) / 16), 40)] == '.')
		{
			player.rect.y = 16 + 16 * floor((player.rect.y + player.speed_y) / 16);
			player.speed_y = 0;
			player.jumppower = 0;
		}
	}
	
	if (player.speed_x > 0)
	{
		if (level[cmod((int)floor((player.rect.y) / 16), 30)][cmod((int)floor((player.rect.x + player.rect.w + player.speed_x) / 16), 40)] == '.')
		{
			player.rect.x = 16 * floor((player.rect.x + player.rect.w + player.speed_x) / 16) - player.rect.w;
			player.speed_x = 0;
		}
		else if (level[cmod((int)floor((player.rect.y + (player.rect.h / 2) - 1) / 16), 30)][cmod((int)floor((player.rect.x + player.rect.w + player.speed_x) / 16), 40)] == '.')
		{
			player.rect.x = 16 * floor((player.rect.x + player.rect.w + player.speed_x) / 16) - player.rect.w;
			player.speed_x = 0;
		}
		else if (level[cmod((int)floor((player.rect.y + player.rect.h - 1) / 16), 30)][cmod((int)floor((player.rect.x + player.rect.w + player.speed_x) / 16), 40)] == '.')
		{
			player.rect.x = 16 * floor((player.rect.x + player.rect.w + player.speed_x) / 16) - player.rect.w;
			player.speed_x = 0;
		}
	}
	else if (player.speed_x < 0)
	{
		if (level[cmod((int)floor((player.rect.y) / 16), 30)][cmod((int)floor((player.rect.x + player.speed_x) / 16), 40)] == '.')
		{
			player.rect.x = 16 + 16 * floor((player.rect.x + player.speed_x) / 16);
			player.speed_x = 0;
		}
		else if (level[cmod((int)floor((player.rect.y + (player.rect.h / 2) - 1) / 16), 30)][cmod((int)floor((player.rect.x + player.speed_x) / 16), 40)] == '.')
		{
			player.rect.x = 16 + 16 * floor((player.rect.x + player.speed_x) / 16);
			player.speed_x = 0;
		}
		else if (level[cmod((int)floor((player.rect.y + player.rect.h - 1) / 16), 30)][cmod((int)floor((player.rect.x + player.speed_x) / 16), 40)] == '.')
		{
			player.rect.x = 16 + 16 * floor((player.rect.x + player.speed_x) / 16);
			player.speed_x = 0;
		}
	}
	
	player.rect.x += round(player.speed_x);
	player.rect.y += round(player.speed_y);
	
	if (player.speed_x > 0 && player.spriterect.x == 0)
	{
		player.spriterect.x = 192;
	}
	else if (player.speed_x < 0 && player.spriterect.x == 192)
	{
		player.spriterect.x = 0;
	}
	
	if (player.shooting)
	{
		player.spriterect.y = 192;
	}
	else if (!player.ground)
	{
		player.spriterect.y = 128;
	}
	else if (player.speed_x > 0.1 || player.speed_x < -0.1)
	{
		player.spriterect.y = 64;
	}
	else
	{
		player.spriterect.y = 0;
	}
	
	if (player.animtime > 0)
	{
		player.animtime -= frameDelta;
	}
	else
	{
		player.animframe = (player.animframe + 1) % 4;
		player.animtime = 50;
		
		if (player.shooting)
		{
			player.shooting = 0;
		}
	}
	
	// SDL_FillRect(screen, &player.rect, SDL_MapRGB(screen->format, 255, 255, 0));
	
	if (player.hurting)
	{
		blink = (int)floor((float)player.hurttime / 100.0) % 2;
	}
	else
	{
		blink = 0;
	}
	
	blitrect.x = player.rect.x;
	blitrect.y = player.rect.y;
	blitrect.w = player.rect.w;
	blitrect.h = player.rect.h;
	
	player.spriterect.x += 48 * player.animframe;
	
	if (player.rect.x + player.rect.w >= 640)
	{
		if (!blink)
		{
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
			
			blitrect.x = player.rect.x - 640;
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
		}
		
		if (player.rect.x >= 640)
		{
			player.rect.x -= 640;
		}
	}
	else if (player.rect.x < 0)
	{
		if (!blink)
		{
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
			
			blitrect.x = player.rect.x + 640;
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
		}
		
		if (player.rect.x + player.rect.w <= 0)
		{
			player.rect.x += 640;
		}
	}
	else if (player.rect.y + player.rect.h >= 480)
	{
		if (!blink)
		{
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
			
			blitrect.y = player.rect.y - 480;
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
		}
		
		if (player.rect.y >= 480)
		{
			player.rect.y -= 480;
		}
	}
	else if (player.rect.y < 0)
	{
		if (!blink)
		{
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
			
			blitrect.y = player.rect.y + 480;
			SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
		}
		
		if (player.rect.y + player.rect.h < 0)
		{
			player.rect.y += 480;
		}
	}
	else if (!blink)
	{
		SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
	}
	
	player.spriterect.x -= 48 * player.animframe;
	
	if (player.hurting)
	{
		player.hurttime += frameDelta;
		
		if (player.hurttime >= 2000)
		{
			player.hurting = 0;
			player.hurttime = 0;
		}
	}
}

void level_place_player()
{
	int x, y;
	
	for (y = 0; y < 30; ++y)
	{
		for (x = 0; x < 40; ++x)
		{
			if (level[y][x] == 'P')
			{
				player.rect.x = 16 * x - (player.rect.w / 2);
				player.rect.y = 16 * y - player.rect.h - 4;
				
				player.spriterect.x = 192;
				
				return;
			}
		}
	}
}



void spawngoats()
{
	int x, y, n;
	
	if (activegoats == 0)
	{
		n = 0;
		
		for (y = 0; y < 30; ++y)
		{
			for (x = 0; x < 40; ++x)
			{
				if (level[y][x] == 'G')
				{
					mygoats[n] = goat_create(16 * x - 24, 16 * y - 32);
					++activegoats;
					++n;
				}
			}
		}
	}
}

void spawnslimes()
{
	int x, y, n;
	float speed;
	
	n = 0;
	
	for (y = 0; y < 30; ++y)
	{
		for (x = 0; x < 40; ++x)
		{
			if (level[y][x] == '<' || level[y][x] == '>')
			{
				speed = slimespeedmax * (float)((rand() % 100) / 100.0);
				
				if (speed < slimespeedmin)
				{
					speed = slimespeedmin;
				}
				
				myslimes[n] = slime_create(16 * x - 16, 16 * y - 25, speed);
				
				myslimes[n]->controls.left = (level[y][x] == '<');
				myslimes[n]->controls.right = (level[y][x] == '>');
				
				++n;
			}
		}
	}
}

void loadlevel()
{
	int i;
	
	++nthlevel;
	
	if (level == 0)
	{
		level = level1;
	}
	else if (level == level1)
	{
		level = level2;
	}
	else if (level == level2)
	{
		level = level3;
	}
	else if (level == level3)
	{
		level = level4;
	}
	else if (level == level4)
	{
		level = level5;
	}
	else if (level == level5)
	{
		level = level1;
	}
	
	for (i = 0; i < 16; ++i)
	{
		if (mygoats[i] != NULL)
		{
			free(mygoats[i]);
			mygoats[i] = NULL;
		}
		
		if (myslimes[i] != NULL)
		{
			free(myslimes[i]);
			myslimes[i] = NULL;
		}
	}
	
	numgoats = atoi(level[30]);
	activegoats = 0;
	
	slimespeedmin = atof(level[31]);
	slimespeedmax = atof(level[32]);
	
	spawngoats();
	spawnslimes();
	
	level_place_player();
}

void _framefn(int frameDelta)
{
	SDL_Rect r;
	SDL_Rect blitrect;
	int x, y, i, j;
	int red, black;
	char buf[32];
	
	if (level == 0)
	{
		loadlevel();
	}
	
	if (activegoats == 0)
	{
		spawngoats();
	}
	
	red = SDL_MapRGB(screen->format, 255, 0, 0);
	black = SDL_MapRGB(screen->format, 44, 33, 44);
	
	// SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
	r.x = 0;
	r.y = 0;
	r.w = 16;
	r.h = 16;
	
	for (y = 0; y < 30; ++y)
	{
		r.x = 0;
		
		for (x = 0; x < 40; ++x)
		{
			if (level[y][x] == '.')
			{
				// SDL_FillRect(screen, &r, red);
				
				blitrect.x = r.x;
				blitrect.y = r.y;
				blitrect.w = r.w;
				blitrect.h = r.h;
				
				SDL_BlitSurface(gfx_brick, NULL, screen, &blitrect);
			}
			else
			{
				SDL_FillRect(screen, &r, black);
			}
			
			r.x += 16;
		}
		
		r.y += 16;
	}
	
	playerfn(frameDelta);
	
	if (player.shooting)
	{
		for (i = 0; i < 16; ++i)
		{
			if (mygoats[i] == NULL)
			{
				continue;
			}
			
			if (!mygoats[i]->scared && distance(
				player.rect.x + (player.rect.w / 2),
				player.rect.y + (player.rect.h / 2),
				mygoats[i]->rect.x + (mygoats[i]->rect.w / 2),
				mygoats[i]->rect.y + (mygoats[i]->rect.h / 2)) < 50)
			{
				goat_make_scared(mygoats[i]);
			}
		}
	}
	
	for (i = 0; i < 16; ++i)
	{
		if (mygoats[i] == NULL)
		{
			continue;
		}
		
		if (mygoats[i]->scared && mygoats[i]->animtime <= 0 && distance(
			player.rect.x + (player.rect.w / 2),
			player.rect.y + (player.rect.h / 2),
			mygoats[i]->rect.x + (mygoats[i]->rect.w / 2),
			mygoats[i]->rect.y + (mygoats[i]->rect.h / 2)) < 30)
		{
			free(mygoats[i]);
			mygoats[i] = NULL;
			
			--activegoats;
			--numgoats;
			
			sound_synth_play(sfx_clear_goat1);
			sound_synth_play(sfx_clear_goat2);
			
			player.score += nthlevel * player.hearts;
		}
		else if (mygoats[i]->scared && mygoats[i]->animtime < -2000)
		{
			free(mygoats[i]);
			mygoats[i] = NULL;
			
			--activegoats;
			--numgoats;
		}
	}
	
	if (numgoats == 0)
	{
		for (i = 0; i < 4; ++i)
		{
			SDL_Delay(130);
			
			if (i < 3)
			{
				sound_synth_play(sfx_c4);
			}
			else
			{
				sound_synth_play(sfx_f4);
			}
		}
		
		player.hurting = 0;
		
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		SDL_Delay(650);
		
		newlevelpause = 1;
		
		loadlevel();
		
		return;
	}
	
	for (i = 0; i < 16; ++i)
	{ 
		if (mygoats[i] == NULL)
		{
			continue;
		}
		
		goatfn(mygoats[i], frameDelta);
		
		if (mygoats[i]->scared == 1)
		{
			continue;
		}
		
		if (rand() % 100 > 90)
		{
			mygoats[i]->controls.right = 1;
		}
		if (rand() % 100 > 90)
		{
			mygoats[i]->controls.right = 0;
		}
		if (rand() % 100 > 90)
		{
			mygoats[i]->controls.left = 1;
		}
		if (rand() % 100 > 90)
		{
			mygoats[i]->controls.left = 0;
		}
		if (rand() % 100 > 95)
		{
			mygoats[i]->controls.button1 = 1;
		}
		if (rand() % 100 > 50)
		{
			mygoats[i]->controls.button1 = 0;
		}
	}
	
	for (i = 0; i < 16; ++i)
	{ 
		if (myslimes[i] == NULL)
		{
			continue;
		}
		
		if (!player.hurting && distance(
			player.rect.x + (player.rect.w / 2),
			player.rect.y + (player.rect.h / 2),
			myslimes[i]->rect.x + (myslimes[i]->rect.w / 2),
			myslimes[i]->rect.y + (myslimes[i]->rect.h / 2)) < 30)
		{
			sound_synth_play(sfx_hurting);
			player.hurting = 1;
			
			if (player.hearts > 0)
			{
				if (--player.hearts == 0)
				{
					for (j = 0; j < 5; ++j)
					{
						sound_synth_play(sfx_hurting);
						SDL_Delay(120);
					}
					
					SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
					
					fontsurf = TTF_RenderText_Blended(font, "Game Over", (SDL_Color){255, 0, 0});
					
					blitrect.x = 270;
					blitrect.y = 200;
					
					SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
					SDL_FreeSurface(fontsurf);
					
					sprintf(buf, "Score: %d", player.score);
					
					fontsurf = TTF_RenderText_Blended(font, buf, (SDL_Color){255, 255, 255});
					
					blitrect.x = 260;
					blitrect.y = 230;
					
					SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
					SDL_FreeSurface(fontsurf);
					
					SDL_UpdateRect(screen, 0, 0, 0, 0);
					SDL_Delay(1000);
					
					level = 0;
					player.hearts = 10;
					player.score = 0;
					nthlevel = 0;
					loadlevel();
					newlevelpause = 1;
					return;
				}
			}
		}
	}
	
	for (i = 0; i < 16; ++i)
	{ 
		if (myslimes[i] == NULL)
		{
			continue;
		}
		
		slimefn(myslimes[i], frameDelta);
	}
	
	
	
	blitrect.x = 18;
	blitrect.y = 18;
	
	SDL_BlitSurface(gfx_goat_count, NULL, screen, &blitrect);
	
	blitrect.x = 44;
	sprintf(buf, "%d", numgoats);
	
	fontsurf = TTF_RenderText_Blended(font, buf, (SDL_Color){255, 255, 255});
	SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
	SDL_FreeSurface(fontsurf);
	
	sprintf(buf, "%d", player.score);
	
	blitrect.x = 18;
	blitrect.y = 32;
	
	fontsurf = TTF_RenderText_Blended(font, buf, (SDL_Color){255, 255, 255});
	SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
	SDL_FreeSurface(fontsurf);
	
	blitrect.x = 604;
	
	for (i = 0; i < player.hearts; ++i)
	{
		SDL_BlitSurface(gfx_heart, NULL, screen, &blitrect);
		blitrect.x -= 20;
	}
	
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	
	if (newlevelpause)
	{
		newlevelpause = 0;
		
		blitrect.x = 270;
		blitrect.y = 220;
		blitrect.w = 128;
		blitrect.h = 32;
		
		SDL_FillRect(screen, &blitrect, SDL_MapRGB(screen->format, 0, 0, 0));
		
		sprintf(buf, "Level %d", nthlevel);
		
		fontsurf = TTF_RenderText_Blended(font, buf, (SDL_Color){255, 255, 255});
		
		blitrect.x = 280;
		blitrect.y = 230;
		
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		
		SDL_Delay(400);
		sound_synth_play(sfx_c4);
		SDL_Delay(800);
	}
}

void load_sprites()
{
	sprite_player = sdl_read_xpm(sprite_drac_xpm, screen->format);
	SDL_SetColorKey(sprite_player, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 0, 255));
	
	sprite_goat = sdl_read_xpm(sprite_goat_xpm, screen->format);
	SDL_SetColorKey(sprite_goat, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 0, 255));
	
	sprite_slime = sdl_read_xpm(sprite_slime_xpm, screen->format);
	SDL_SetColorKey(sprite_slime, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 0, 255));
	
	gfx_brick = sdl_read_xpm(gfx_brick_xpm, screen->format);
	
	gfx_goat_count = sdl_read_xpm(gfx_goat_count_xpm, screen->format);
	SDL_SetColorKey(gfx_goat_count, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 0, 255));
	
	gfx_heart = sdl_read_xpm(gfx_heart_xpm, screen->format);
	SDL_SetColorKey(gfx_heart, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 0, 255));
}

int introtime;

void _introfn(int frameDelta)
{
	SDL_Rect blitrect;
	
	introtime += frameDelta;
	
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	
	blitrect.x = 18;
	blitrect.y = 32;
	
	SDL_BlitSurface(sprite_player, &player.spriterect, screen, &blitrect);
	
	if (introtime > 1000)
	{
		blitrect.x = 64;
		blitrect.y = 38;
		
		fontsurf = TTF_RenderText_Blended(font, "Unhappy with his insufficiently scary castle, famous villain Dracula had", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 2500)
	{
		blitrect.x = 64;
		blitrect.y = 60;
		
		fontsurf = TTF_RenderText_Blended(font, "ordered a million ghosts and was waiting for the delivery.", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 5000)
	{
		blitrect.x = 64;
		blitrect.y = 104;
		
		fontsurf = TTF_RenderText_Blended(font, "The delivery finally appeared, but there had been a mistake.", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 6500)
	{
		blitrect.x = 64;
		blitrect.y = 126;
		
		fontsurf = TTF_RenderText_Blended(font, "Instead of a million ghosts, the package contained ...", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 8000)
	{
		blitrect.x = 64;
		blitrect.y = 148;
		
		fontsurf = TTF_RenderText_Blended(font, "one", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 9000)
	{
		blitrect.x = 100;
		blitrect.y = 148;
		
		fontsurf = TTF_RenderText_Blended(font, "million", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 10000)
	{
		blitrect.x = 154;
		blitrect.y = 148;
		
		fontsurf = TTF_RenderText_Blended(font, "GOATS!", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 13000)
	{
		blitrect.x = 64;
		blitrect.y = 180;
		
		fontsurf = TTF_RenderText_Blended(font, "Since the goats somehow got loose, Dracula must now", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 14500)
	{
		blitrect.x = 64;
		blitrect.y = 202;
		
		fontsurf = TTF_RenderText_Blended(font, "collect them all. They are fainting goats, so the easiest", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 16000)
	{
		blitrect.x = 64;
		blitrect.y = 224;
		
		fontsurf = TTF_RenderText_Blended(font, "way to trap them is to give them a scare.", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 19000)
	{
		blitrect.x = 64;
		blitrect.y = 260;
		
		fontsurf = TTF_RenderText_Blended(font, "Dracula can move with the arrow keys, jump (Z) and scare goats (X).", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 20500)
	{
		blitrect.x = 64;
		blitrect.y = 282;
		
		fontsurf = TTF_RenderText_Blended(font, "Green slimes are dangerous. Dracula has 10 hit points.", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 23000)
	{
		blitrect.x = 64;
		blitrect.y = 320;
		
		fontsurf = TTF_RenderText_Blended(font, "Press (Z) to play.", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	if (introtime > 24000)
	{
		blitrect.x = 16;
		blitrect.y = 440;
		
		fontsurf = TTF_RenderText_Blended(font, "lur@LD#25", (SDL_Color){255, 255, 255});
		SDL_BlitSurface(fontsurf, NULL, screen, &blitrect);
		SDL_FreeSurface(fontsurf);
	}
	
	SDL_UpdateRect(screen, 0, 0, 0, 0);
	
	if (controls.button1)
	{
		if (introtime < 23000)
		{
			introtime += 1000;
		}
		else
		{
			sound_set_song(0);
			
			sound_synth_play(sfx_clear_goat1);
			sound_synth_play(sfx_clear_goat2);
			SDL_Delay(800);
			
			sound_set_song(3);
			framefn = _framefn;
		}
	}
}

int main(int argc, char** argv)
{
	SDL_Event ev;
	
	int quit = 0;
	int sleepCycle[] = {16, 17, 17};
	int sleepNth = 0;
	int sleepTotal = 0;
	int frameStart = 0;
	int frameEnd = 0;
	int frameDelta = 0;
	// int fps = 0, fpsreport = 0;
	
	nthlevel = 0;
	newlevelpause = 1;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	TTF_Init();
	
	sound_setup();
	sound_set_song(2);
	
	sfx_player_jump = sound_synth_get(0);
	sfx_player_jump->waveform = SYNTH_SQUARE;
	sfx_player_jump->length = 100;
	sfx_player_jump->amp_start = 0.3;
	sfx_player_jump->amp_end = 0.2;
	sfx_player_jump->freq_start = 300;
	sfx_player_jump->freq_end = 500;
	
	sfx_player_scare = sound_synth_get(1);
	sfx_player_scare->waveform = SYNTH_NOISE;
	sfx_player_scare->length = 320;
	sfx_player_scare->amp_start = 0.1;
	sfx_player_scare->amp_end = 0.1;
	sfx_player_scare->freq_start = 3200;
	sfx_player_scare->freq_end = 4800;
	
	sfx_player_scare2 = sound_synth_get(2);
	sfx_player_scare2->waveform = SYNTH_SQUARE;
	sfx_player_scare2->length = 320;
	sfx_player_scare2->amp_start = 0.15;
	sfx_player_scare2->amp_end = 0.0;
	sfx_player_scare2->freq_start = 1200;
	sfx_player_scare2->freq_end = 1550;
	
	sfx_clear_goat1 = sound_synth_get(3);
	sfx_clear_goat1->waveform = SYNTH_SQUARE;
	sfx_clear_goat1->length = 50;
	sfx_clear_goat1->amp_start = 0.3;
	sfx_clear_goat1->amp_end = 0.3;
	sfx_clear_goat1->freq_start = 450;
	sfx_clear_goat1->freq_end = 450;
	
	sfx_clear_goat2 = sound_synth_get(4);
	sfx_clear_goat2->waveform = SYNTH_SQUARE;
	sfx_clear_goat2->length = 50;
	sfx_clear_goat2->delay = 50;
	sfx_clear_goat2->amp_start = 0.3;
	sfx_clear_goat2->amp_end = 0.3;
	sfx_clear_goat2->freq_start = 1250;
	sfx_clear_goat2->freq_end = 1250;
	
	sfx_hurting = sound_synth_get(5);
	sfx_hurting->waveform = SYNTH_SQUARE;
	sfx_hurting->length = 180;
	sfx_hurting->delay = 0;
	sfx_hurting->amp_start = 0.3;
	sfx_hurting->amp_end = 0;
	sfx_hurting->freq_start = 1000;
	sfx_hurting->freq_end = 2;
	
	sfx_c4 = sound_synth_get(6);
	sfx_c4->waveform = SYNTH_SQUARE;
	sfx_c4->length = 180;
	sfx_c4->delay = 0;
	sfx_c4->amp_start = 0.3;
	sfx_c4->amp_end = 0.1;
	sfx_c4->freq_start = 261.626;
	sfx_c4->freq_end = 261.626;
	
	sfx_f4 = sound_synth_get(7);
	sfx_f4->waveform = SYNTH_SQUARE;
	sfx_f4->length = 180;
	sfx_f4->delay = 0;
	sfx_f4->amp_start = 0.3;
	sfx_f4->amp_end = 0.1;
	sfx_f4->freq_start = 349.228;
	sfx_f4->freq_end = 349.228;
	
	level = 0;
	gravity = 0.8;
	
	framefn = &_introfn;
	memset(&controls, 0, sizeof(Controls));
	memset(&player, 0, sizeof(Player));
	
	player.hearts = 10;
	
	player.rect.w = 48;
	player.rect.h = 64;
	
	// level_place_player();
	
	player.spriterect.x = 192;
	player.spriterect.y = 0;
	player.spriterect.w = 48;
	player.spriterect.h = 64;
	
	font = TTF_OpenFont("Vera.ttf", 16);
	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);
	
	load_sprites();
	
	mygoats = (Goat**) malloc(sizeof(Goat*) * 16);
	memset(mygoats, 0, sizeof(Goat*) * 16);
	
	// mygoats[0] = goat_create(100, 10);
	// mygoats[1] = goat_create(300, 10);
	// mygoats[2] = goat_create(500, 10);
	
	myslimes = (Slime**) malloc(sizeof(Slime*) * 16);
	memset(myslimes, 0, sizeof(Slime*) * 16);
	
	// myslimes[0] = slime_create(400, 10, 1);
	// myslimes[0]->controls.left = 1;
	
	// fpsreport = SDL_GetTicks() + 1000;
	
	while (!quit)
	{
		frameDelta = SDL_GetTicks() - frameStart;
		frameStart = SDL_GetTicks();
		
		framefn(frameDelta);
		
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_KEYDOWN:
					switch (ev.key.keysym.sym)
					{
						case SDLK_LEFT:
							controls.left = 1;
							break;
						
						case SDLK_RIGHT:
							controls.right = 1;
							break;
						
						case SDLK_UP:
							// controls.up = 1;
							controls.button1 = 1;
							break;
						
						case SDLK_DOWN:
							controls.down = 1;
							break;
						
						case SDLK_z:
						case SDLK_SPACE:
							controls.button1 = 1;
							break;
						
						case SDLK_x:
						case SDLK_LCTRL:
							controls.button2 = 1;
							break;
						
						default:
							break;
					}
					
					break;
				
				case SDL_KEYUP:
					switch (ev.key.keysym.sym)
					{
						case SDLK_LEFT:
							controls.left = 0;
							break;
						
						case SDLK_RIGHT:
							controls.right = 0;
							break;
						
						case SDLK_UP:
							// controls.up = 0;
							controls.button1 = 0;
							break;
						
						case SDLK_DOWN:
							controls.down = 0;
							break;
						
						case SDLK_z:
						case SDLK_SPACE:
							controls.button1 = 0;
							break;
						
						case SDLK_x:
						case SDLK_LCTRL:
							controls.button2 = 0;
							break;
						
						default:
							break;
					}
					
					break;
				
				case SDL_QUIT:
					quit = 1;
					break;
				
				default:
					break;
			}
		}
		
		// ++fps;
		
		frameEnd = SDL_GetTicks();
		
		// if (fpsreport <= frameEnd)
		// {
			// printf("%d fps\n", fps);
			// fps = 0;
			// fpsreport += 1000;
		// }
		
		if (sleepCycle[sleepNth] - (frameEnd - frameStart) > 0)
		{
			SDL_Delay(sleepCycle[sleepNth] - (frameEnd - frameStart));
		}
		
		if (++sleepNth > 2)
		{
			sleepNth = 0;
		}
	}
	
	sound_quit();
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
	
	return EXIT_SUCCESS;
}
