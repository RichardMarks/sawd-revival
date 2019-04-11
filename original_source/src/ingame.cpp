
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

// handles the ingame menu
const int INGAME_MENU_ITEMS		= 0;
const int INGAME_MENU_EQUIP		= 1;
const int INGAME_MENU_MAGIC		= 2;
const int INGAME_MENU_STATUS	= 3;
const int INGAME_MENU_SYSTEM	= 4;
const int INGAME_MENU_QUIT		= 5;

void ingame_menu()
{
	const int num_options = 6;
	char* ingame_menu_options[num_options] =
	{
		"Items",
		"Equipment",
		"Magic",
		"Status",
		"System",
		"Quit Game"
	};

	char itm_cursor_icon_l = (char)16;

	int cursor_y = 0;

	int box[] = {31, 1, 24, 10};

	int x = box[0] + 2;
	int y = box[1] + 2;
	
	bool close_box = false;

	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1|2|4, 0);

	// draw the available items
	{for(int n = 0; n < num_options; n++)
	{
		if (n==2)
		{
			if (player->chr_class!=3)
			{
				cl->set_fgcolor(8);
				cl->outchars(x+2,y+n,ingame_menu_options[n]);
				cl->set_fgcolor(0);
			}
			else
			{
				cl->outchars(x+2,y+n,ingame_menu_options[n]);
			}
		}
		else
		{
		cl->outchars(x + 2, y + n, ingame_menu_options[n]);
		}
	}
	}

	while(!close_box)
	{
		cl->set_bgcolor(1|2|4);
		cl->set_fgcolor(0);
		{ // clear all cursor positions
			for(int n=0;n<num_options;n++)
			{
				cl->outchar(x, y + n, ' '); // left item cursor
			}
		} // end clear cursor positions

		// draw the cursor where it should be
		cl->outchar(x, y + cursor_y, itm_cursor_icon_l);

		int k = cl->get_key();

		switch (k)
		{
		case VK_UP:
			{
				if (cursor_y > 0)
				{
					cursor_y--;
				}
				else if (cursor_y <= 0)
				{
					cursor_y = num_options-1;
				}
			} break;
		case VK_DOWN:
			{
				if (cursor_y < num_options-1) 
				{
					cursor_y++;
				}
				else if (cursor_y >= num_options-1)
				{
					// move to the first item
					cursor_y = 0;
				}
			} break;

		case VK_BACK:
		case VK_ESCAPE:
			{
				close_box = true;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				switch (cursor_y)
				{
				case INGAME_MENU_ITEMS: 
					{
						ingame_inventory();
					} break;
				case INGAME_MENU_EQUIP: 
					{
						ingame_equipment();
					} break;
				case INGAME_MENU_MAGIC: 
					{
						if (player->chr_class!=3)
						{
							cl->show_message(3,"","You cannot use magic!","");
						}
						else
						{
							ingame_magic();
						}
					} break;
				case INGAME_MENU_STATUS: 
					{
						ingame_status();
					} break;
				case INGAME_MENU_SYSTEM: 
					{
						ingame_system();
					} break;
				case INGAME_MENU_QUIT: 
					{
						int yesno = cl->show_choice("Really quit the game?");
						if (yesno == 1)
						{
							cl->set_bgcolor(0);
							cl->cls();
							gamestate = gsmain_menu;
							close_box = true;
						}
					} break;
				} // end switch cursor_y
			} break;
		} // end switch getkey
	} // end while

	cl->close_window(wnd);
}


void ingame_magic()
{
	if ((int)player->spells.size()==0)
	{
		cl->show_message(3,"","You have no magic!","");
		return;
	}
	//cl->show_message(3,"TODO: Ingame Magic Screen","","Sorry not implemented yet");
	int box[] = {41, 3, 36, 19};
	int x = box[0] + 2;
	int y = box[1] + 2;
	
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1|2|4, 0);

	for (unsigned int i = 0; i < player->spells.size(); i++)
	{
		char msg[40];
		sprintf_s(msg,40,"%s MP Cost: %d", player->spells.at(i)->name, player->spells.at(i)->mp_cost);
		cl->outchars(x + 1, (y + i), msg);
	}

	bool close_box = false;
	
	while(!close_box)
	{
		int k = cl->get_key();
		switch(k)
		{
		case VK_ESCAPE:
		case VK_BACK:
		case VK_SPACE:
		case VK_RETURN:
			{
				close_box = true;
			} break;
		}// end switch getkey
	} // end while not closebox

	cl->close_window(wnd);
}

void ingame_status()
{
	// cl->show_message(3,"TODO: Ingame Status Screen","","Sorry not implemented yet");

	int box[] = {41, 3, 36, 19};

	int x = box[0] + 2;
	int y = box[1] + 2;
	
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1|2|4, 0);

	cl->outcharsf(x, y, "%s", player->name);y++;y++;
	cl->outcharsf(x, y, "HP: %04d / %04d", player->cur_hp, player->max_hp);y++;y++;
	cl->outcharsf(x, y, "MP: %04d / %04d", player->cur_mp, player->max_mp);y++;y++;
	cl->outcharsf(x, y, "XP: %08d ", player->experience);y++;y++;
	cl->outcharsf(x, y, "LV: %03d ", player->level);y++;y++;
	cl->outcharsf(x, y, "ATK: %03d   DEF: %03d", player->attack, player->defense);y++;y++;
	cl->outcharsf(x, y, "STR: %03d   MAG: %03d", player->strength, player->magic);y++;y++;
	cl->outcharsf(x, y, "GOLD: $ %d", player->gold);

	// draw the class icon
	char* class_icon = 0;
	char* class_label = 0;
	switch(player->chr_class)
	{
	case 0: class_icon = warrior_icon; class_label = "Warrior"; break;
	case 1: class_icon = dwarf_icon; class_label = "Dwarf"; break;
	case 2: class_icon = thief_icon; class_label = "Thief"; break;
	case 3: class_icon = mage_icon; class_label = "Mage"; break;
	}
	{
		
		const int icon_w = 13;
		const int icon_h = 11;
		const char mask = '@';
		unsigned short fgc = 0, bgc = 1|2|4;
		x = box[0] + (box[2] - (icon_w + 1));
		y = box[1] + 3;

		cl->set_bgcolor(bgc);
		cl->set_fgcolor(fgc);
		for (int py = 0; py < icon_h; py++)
		{
			for (int px = 0; px < icon_w; px++)
			{
				if (class_icon[px + py * icon_w] != mask)
				{
					cl->outchar(x + px, y + py, class_icon[px + py * icon_w]);
				}
			}
		}
		cl->outchars(x + 3, y + icon_h, class_label);
	}
	

	bool close_box = false;
	while(!cl->get_key()){}
	cl->close_window(wnd);
}
