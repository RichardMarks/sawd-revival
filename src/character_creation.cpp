
/*
	SAWD Small ASCII Walk-around Demo (C) Copyright 2008, CCPS Solutions
	
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "clib.h"

#include "sawd.h"

//=========================================================================
//== CHARACTER CREATION
//=========================================================================
const int CC_OPTION_WARRIOR = 0;
const int CC_OPTION_DWARF	= 1;
const int CC_OPTION_THIEF	= 2;
const int CC_OPTION_MAGE	= 3;

void draw_cc_icon(int x, int y, char* icon, bool active, char mask='@');
void draw_cc_icon(int x, int y, char* icon, bool active, char mask)
{
	const int icon_w = 13;
	const int icon_h = 11;

	unsigned short fgc, bgc;
	bgc = 0;
	if (active)
	{
		fgc = 1|2|8;
	}
	else
	{
		fgc = 1|2|4;
	}
	cl->set_bgcolor(bgc);
	cl->set_fgcolor(fgc);

	for (int py = 0; py < icon_h; py++)
	{
		for (int px = 0; px < icon_w; px++)
		{
			if (icon[px + py * icon_w] != mask)
			{
				cl->outchar(x + px, y - ((icon_h + 2) - py), icon[px + py * icon_w]);
			}
		}
	}
}

void draw_cc_options(int x, int y, int active)
{
	if (active == CC_OPTION_WARRIOR)
	{
		draw_cc_icon(x, y, warrior_icon, true);	
		cl->draw_button(x,y,13,3," Warrior ",4,1|2|4|8);
	}
	else
	{
		draw_cc_icon(x, y, warrior_icon, false);	
		cl->draw_button(x,y,13,3," Warrior ",0,1|2|4);
	}

	if (active == CC_OPTION_DWARF)
	{
		draw_cc_icon(x+14, y, dwarf_icon, true);
		cl->draw_button(x+14,y,13,3,"  Dwarf  ",4,1|2|4|8);
	}
	else
	{
		draw_cc_icon(x+14, y, dwarf_icon, false);
		cl->draw_button(x+14,y,13,3,"  Dwarf  ",0,1|2|4);
	}

	if (active == CC_OPTION_THIEF)
	{
		draw_cc_icon(x+29, y, thief_icon, true);
		cl->draw_button(x+29,y,13,3,"  Thief  ",4,1|2|4|8);
	}
	else
	{
		draw_cc_icon(x+29, y, thief_icon, false);
		cl->draw_button(x+29,y,13,3,"  Thief  ",0,1|2|4);
	}

	if (active == CC_OPTION_MAGE)
	{
		draw_cc_icon(x+43, y, mage_icon, true);
		cl->draw_button(x+43,y,13,3,"   Mage  ",4,1|2|4|8);
	}
	else
	{
		draw_cc_icon(x+43, y, mage_icon, false);
		cl->draw_button(x+43,y,13,3,"   Mage  ",0,1|2|4);
	}
}

void gscreate_character()
{
	int active_option = CC_OPTION_WARRIOR;

	int x = 40-56/2;
	int y = 20;

	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->cls();

	//cl->show_message(1, "Select a Character Class");

	draw_cc_options(x, y, active_option);

	bool close_box = false;
	while(!close_box)
	{
		int k = cl->get_key();
		switch (k)
		{
		case VK_LEFT:
			{
				if (active_option > CC_OPTION_WARRIOR)
				{
					active_option--;
				}
				else if (active_option == CC_OPTION_WARRIOR)
				{
					active_option = CC_OPTION_MAGE;
				}
				draw_cc_options(x, y, active_option);
			} break;
		case VK_RIGHT:
			{
				if (active_option < CC_OPTION_MAGE)
				{
					active_option++;
				}
				else if (active_option == CC_OPTION_MAGE)
				{
					active_option = CC_OPTION_WARRIOR;
				}
				draw_cc_options(x, y, active_option);
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				switch(active_option)
				{
				case CC_OPTION_WARRIOR:
					{
						player->chr_class	= 0;
						player->max_hp		= 150 + rand()% (275-150);
						player->max_mp		= 0;
						player->attack		= 4 + rand()% (7 - 4);
						player->defense		= 3 + rand()% (6 - 3);
						player->strength	= 2 + rand()% (4 - 2);
						player->magic		= 0;
						
					} break;

				case CC_OPTION_DWARF:
					{
						player->chr_class	= 1;
						player->max_hp		= 100 + rand()% (175-100);
						player->max_mp		= 0;
						player->attack		= 4 + rand()% (7 - 4);
						player->defense		= 5 + rand()% (8 - 5);
						player->strength	= 2 + rand()% (4 - 2);
						player->magic		= 0;
						
					} break;

				case CC_OPTION_THIEF:
					{
						player->chr_class	= 2;
						player->max_hp		= 100 + rand()% (195-100);
						player->max_mp		= 0;
						player->attack		= 4 + rand()% (7 - 4);
						player->defense		= 3 + rand()% (6 - 3);
						player->strength	= 2 + rand()% (4 - 2);
						player->magic		= 0;
						
					} break;

				case CC_OPTION_MAGE:
					{
						player->chr_class	= 3;
						player->max_hp		= 100 + rand()% (175-100);
						player->max_mp		= 15;
						player->attack		= 2 + rand()% (5 - 2);
						player->defense		= 3 + rand()% (6 - 3);
						player->strength	= 1 + rand()% (3 - 1);
						player->magic		= 4 + rand()% (9 - 4);
						
					} break;
				}

				player->level		= 1;
				player->experience	= 0;
				player->gold		= 100;
				player->max_ap		= 1;
				player->cur_ap		= 1;
				player->cur_hp		= player->max_hp;
				player->cur_mp		= player->max_mp;
				

				close_box = true;
			} break;
		}
	}

	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->cls();

	gamestate = gsingame;
}
