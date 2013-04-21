
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

int insert_pos = 0;

	const unsigned char cursor_icon_r = (unsigned char)17; // <
	const unsigned char cursor_icon_l = (unsigned char)16; // >
	const unsigned char cursor_icon_b = (unsigned char)30; // ^
	const unsigned char cursor_icon_t = (unsigned char)31; // v

void draw_name_entry_name_display(int x, int y, char* name)
{
	cl->draw_button(x, y, 24, 4, name);

	cl->set_fgcolor(1|2|8);
	cl->outchar(2+x + insert_pos, y + 2, cursor_icon_b);
	cl->set_fgcolor(1|2|4|8);
}

void gsname_character()
{
	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->cls();

	cl->show_message(7, 
		"Mysterious Voice:",
		"",
		"Before you may enter the World of Tiron,",
		"",
		"you must submit thy name.",
		"",
		"Your name may not be changed, so choose wisely.");

	char* chrmap =
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_! ";

	/* 13 x 5
	a b c d e f g h i j k l m

	n o p q r s t u v w x y z

	A B C D E F G H I J K L M

	N O P Q R S T U V W X Y Z

	0 1 2 3 4 5 6 7 8 9 _ !  

    | DELETE  | | DEFAULT | | ACCEPT  |
	*/

	int cursor_x = 1;
	int cursor_y = 6;

	clibwindow* wnd = cl->open_window(20, 4, 37, 18);

	// draw the character map
	{
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 13; x++)
			{
				int dx = x * 2;
				int dy = y * 2;
				cl->outchar(wnd->data_[0] + 6 + dx, wnd->data_[1] + 4 + dy, chrmap[x + y * 13]);
			}
		}
	}

	// draw the buttons
	char* btn_caption[3] = {"DELETE ","DEFAULT","ACCEPT "};
	int btn[3][4] =
	{
		{0,  12, 11, 3},
		{12, 12, 11, 3},
		{24, 12, 11, 3}
	};

	{int x = wnd->data_[0] + 1, y = wnd->data_[1] + 2; for(int n = 0; n < 3; n++){
	cl->draw_button(x + btn[n][0], y + btn[n][1], btn[n][2], btn[n][3], btn_caption[n]);
	}}
	
	// draw the 'button' that holds the entered name
	char temp_name[17]; sprintf_s(temp_name, 17, "Hero            ");
	draw_name_entry_name_display(wnd->data_[0] + 1, wnd->data_[1] - 1, temp_name);

	
	bool close_box = false;
	while(!close_box)
	{

		// clear cursors
		{
			int ox = wnd->data_[0] + 4, oy = wnd->data_[1] + 4;
			for (int y = 0; y < 5; y++)
			{
				for (int x = 0; x < 14; x++)
				{
					int dx = 1 + (x * 2);
					int dy = y * 2;
					cl->outchar(ox + dx, oy + dy, ' ');
				}
			}

			for (int n = 0; n < 3; n++)
			{
				cl->outchar(wnd->data_[0] + btn[n][0] + 5, 
					wnd->data_[1] + btn[n][1]+1, ' ');
			}
		}

		//draw cursors
		{
			if (cursor_y != 6)
			{
				int ox = wnd->data_[0] + 4, oy = wnd->data_[1] + 4;
				int dx = 1 + (cursor_x * 2);
				int dy = cursor_y * 2;
				cl->set_fgcolor(1|2|8);
				cl->outchar(ox + dx, oy + dy, '[');
				cl->outchar(ox + dx + 2, oy + dy, ']');
				cl->set_fgcolor(1|2|4|8);
			}
			else if (cursor_y == 6)
			{
				// cursor is at the buttons
				cl->set_fgcolor(1|2|8);
				cl->outchar(wnd->data_[0] + btn[cursor_x][0] + 5,
					wnd->data_[1] + btn[cursor_x][1]+1, cursor_icon_t);
				cl->set_fgcolor(1|2|4|8);
			}
		}


		int k = cl->get_key();
		switch (k)
		{
		case VK_UP:
			{
				if (cursor_y > 0 && cursor_y != 6)
				{
					cursor_y--;
				}
				else if (cursor_y == 6)
				{
					cursor_y = 4;
					cursor_x = 0;
				}
			} break;
		case VK_DOWN:
			{
				if (cursor_y < 4)
				{
					cursor_y++;
				}
				else if (cursor_y == 4)
				{
					cursor_y = 6;
					cursor_x = 0;
				}
			} break;
		case VK_LEFT:
			{
				if (cursor_y == 6)
				{
					// at buttons
					if (cursor_x > 0)
					{
						cursor_x--;
					}
					else if (cursor_x == 0)
					{
						cursor_x = 2;
					}
				}
				else
				{
					// at chrmap
					if (cursor_x > 0)
					{
						cursor_x--;
					}
				}
			} break;
		case VK_RIGHT:
			{
				if (cursor_y == 6)
				{
					// at buttons
					if (cursor_x < 2)
					{
						cursor_x++;
					}
					else if (cursor_x == 2)
					{
						cursor_x = 0;
					}
				}
				else
				{
					// at chrmap
					if (cursor_x < 12)
					{
						cursor_x++;
					}
				}
			} break;

		case VK_ESCAPE:
			{
				close_box = true;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				if (cursor_y == 6)
				{
					// at buttons
					switch(cursor_x)
					{
					case 0: // delete
						{
							if (insert_pos > 0)
							{
								temp_name[insert_pos] = ' ';
								insert_pos--;
								draw_name_entry_name_display(wnd->data_[0] + 1, wnd->data_[1] - 1, temp_name);
							}
						} break;
					case 1: // default
						{
							sprintf_s(temp_name, 17, "                ");
							sprintf_s(temp_name, 17, "Hero");
							draw_name_entry_name_display(wnd->data_[0] + 1, wnd->data_[1] - 1, temp_name);
						} break;
					case 2: // accept
						{
							sprintf_s(player->name, 32, "%s", temp_name);
							close_box = true;
						} break;
					}
				}
				else
				{
					// at chrmap
					if (insert_pos < 15)
					{
						temp_name[insert_pos] = chrmap[cursor_x+cursor_y*13];
						insert_pos++;
						draw_name_entry_name_display(wnd->data_[0] + 1, wnd->data_[1] - 1, temp_name);
					}
					else
					{
						cl->show_message(1, "Name length limit reached!");
					}
				}
			} break;
		}
	}

	cl->close_window(wnd);

	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->cls();

	gamestate = gscreate_character;
	// gamestate = 0;
}
