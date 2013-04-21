
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

void gstitle_screen()
{
	// display title
	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->cls();

	char* title[] =
	{
		"CCPS Solutions Presents",
		"  ",
		" SSSS  AAA  W   W DDDD",
		"S     A   A W   W D   D",
		" SSS  AAAAA W W W D   D",
		"    S A   A W W W D   D",
		"SSSS  A   A WW WW DDDD",
		"  ",
		"  ",
		"Simple ASCII Walk-around Demo",
		"  ",
		"RPGDX 2008 ASCII Mini-RPG Contest",
		"  ",
		"http://www.ccpssolutions.com"
	};

	for (int n = 0; n < 14; n++)
	{
		int x = 40 - (strlen(title[n]) / 2);
		cl->outchars(x, 4 + n, title[n]);
	}

	while(!cl->get_key())
	{
	}

	gamestate = gsmain_menu;
}

const int MAINMNU_OPTION_NEWGAME = 0;
const int MAINMNU_OPTION_LOADGAME = 1;
const int MAINMNU_OPTION_CREDITS = 2;
const int MAINMNU_OPTION_QUITGAME = 3;

void draw_main_menu_options(int x, int y, int active)
{
	// New  Game
	// Load Game
	//  Credits
	// Quit Game

	/*
	_____________ _____________ _____________ _____________
	* New  Game * * Load Game * *  Credits  * * Quit Game *
^^^^^^^^^^^^^ ^^^^^^^^^^^^^ ^^^^^^^^^^^^^ ^^^^^^^^^^^^^
===========================================
	*/
	if (active == MAINMNU_OPTION_NEWGAME)
	{
		cl->draw_button(x,y,13,3,"New Game",4,1|2|4|8);
	}
	else
	{
		cl->draw_button(x,y,13,3,"New Game",0,1|2|4);
	}

	if (active == MAINMNU_OPTION_LOADGAME)
	{
		cl->draw_button(x+14,y,13,3,"Load Game",4,1|2|4|8);
	}
	else
	{
		cl->draw_button(x+14,y,13,3,"Load Game",0,1|2|4);
	}

	if (active == MAINMNU_OPTION_CREDITS)
	{
		cl->draw_button(x+29,y,13,3," Credits ",4,1|2|4|8);
	}
	else
	{
		cl->draw_button(x+29,y,13,3," Credits ",0,1|2|4);
	}

	if (active == MAINMNU_OPTION_QUITGAME)
	{
		cl->draw_button(x+43,y,13,3,"Quit Game",4,1|2|4|8);
	}
	else
	{
		cl->draw_button(x+43,y,13,3,"Quit Game",0,1|2|4);
	}
}

void gsmain_menu()
{

	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->cls();

	char* title[] =
	{
		"CCPS Solutions Presents",
		"  ",
		" SSSS  AAA  W   W DDDD",
		"S     A   A W   W D   D",
		" SSS  AAAAA W W W D   D",
		"    S A   A W W W D   D",
		"SSSS  A   A WW WW DDDD",
		"  ",
		"  ",
		"Simple ASCII Walk-around Demo",
		"  ",
		"RPGDX 2008 ASCII Mini-RPG Contest",
		"  ",
		"http://www.ccpssolutions.com"
	};

	for (int n = 0; n < 14; n++)
	{
		int x = 40 - (strlen(title[n]) / 2);
		cl->outchars(x, 4 + n, title[n]);
	}

	int active_option = MAINMNU_OPTION_NEWGAME;

	int x = 40-56/2;
	int y = 20;

	draw_main_menu_options(x, y, active_option);

	bool close_box = false;
	while(!close_box)
	{
		int k = cl->get_key();
		switch (k)
		{
		case VK_LEFT:
			{
				if (active_option > MAINMNU_OPTION_NEWGAME)
				{
					active_option--;
				}
				else if (active_option == MAINMNU_OPTION_NEWGAME)
				{
					active_option = MAINMNU_OPTION_QUITGAME;
				}
				draw_main_menu_options(x, y, active_option);
			} break;
		case VK_RIGHT:
			{
				if (active_option < MAINMNU_OPTION_QUITGAME)
				{
					active_option++;
				}
				else if (active_option == MAINMNU_OPTION_QUITGAME)
				{
					active_option = MAINMNU_OPTION_NEWGAME;
				}
				draw_main_menu_options(x, y, active_option);
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				switch(active_option)
				{
				case MAINMNU_OPTION_NEWGAME:
					{
						reset_game_vars();
						gamestate = gsname_character;
						close_box = true;
					} break;

				case MAINMNU_OPTION_LOADGAME:
					{
						loadgame_screen();
						close_box = true;
						/*
						cl->show_message(3,
							"TODO: Loading Saved Games",
							"",
							"Sorry not implemented yet");
							*/
					} break;

				case MAINMNU_OPTION_CREDITS:
					{
						cl->show_message_centered(11,
							"All content and code created by Richard Marks.",
							"",
							"Many thanks to all my friends for their support.",
							"",
							"Thanks to my wonderful parents, without them I",
							"",
							"would not be here, nor be the man I am today.",
							"",
							"Last but not least, thanks to God, for without",
							"",
							"Him, I would be lost in all that I do.");
					} break;

				case MAINMNU_OPTION_QUITGAME:
					{
						gamestate = 0;
						close_box = true;
					} break;
				} // end switch option
			} break;
		} // end switch key
	} // end while

	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->cls();
}
