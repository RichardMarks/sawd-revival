
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

// handle the inventory
void ingame_inventory()
{
	if ((int)player->items.size()==0)
	{
		cl->show_message_centered(3,"","You have no items!","");
		return;
	}
	
	int box[] = {41, 3, 36, 19};

	int x = box[0] + 2;
	int y = box[1] + 4;
	
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1|2|4, 0);

	char itm_cursor_icon_l = (char)16;
	int cursor_y = 0;
	int scroll_y = 0;
	int num_items = (int)player->items.size();
	int display_amount = 0;
	bool close_box = false;
	char nidisplay[17];
	sprintf_s(nidisplay, 17, "%d Items", num_items);

	cl->draw_button(box[0]+box[2]-19,y-4,19,3,nidisplay,1|2|4,0);

	while(!close_box)
	{
		if (num_items > 10)
		{
			display_amount = 10;
		}
		else
		{
			display_amount = num_items - 1;
		}

		for (int i = scroll_y; i < scroll_y + display_amount; i++)
		{
			cl->set_fgcolor(8);
			cl->outcharsf(x+2, (y + i) - scroll_y, "%03d", i+1);
			cl->set_fgcolor(0);
			cl->outchars(x + 6, (y + i) - scroll_y, "                           ");
			if (i-scroll_y == cursor_y)
			{
				cl->set_fgcolor(1|2|4|8);
				cl->set_bgcolor(4);
				cl->outchars(x + 6, (y + i) - scroll_y, player->items.at(i)->name);
				cl->set_bgcolor(1|2|4);
				cl->set_fgcolor(0);
			}
			else
			{
				cl->outchars(x + 6, (y + i) - scroll_y, player->items.at(i)->name);
			}
		}

		{
			for (int i = 0; i < display_amount; i++)
			{
				cl->outchar(x, y + i, ' ');
			}
		}

		cl->outchar(x, y + cursor_y, itm_cursor_icon_l);

		int k = cl->get_key();
		switch(k)
		{
		case VK_ESCAPE:
		case VK_BACK:
			{
				close_box = true;
			} break;

		case VK_UP:
			{
				if (cursor_y > 0)
				{
					cursor_y--;
				}
				else
				{
					if (scroll_y > 0)
					{
						scroll_y--;
					}
				}
			} break;

		case VK_DOWN:
			{
				if (cursor_y < display_amount-1)
				{
					cursor_y++;
				}
				else
				{
					if (scroll_y < num_items - display_amount)
					{
						scroll_y++;
					}
				}
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				int item_index = cursor_y + scroll_y;
				if (item_index > (int)player->items.size())
					break;

				// use or drop the item
				item_target* t = new item_target(
					player->items.at(item_index), 
					player);

				int result = cl->show_choice_ex("",3,"Use","Drop","Cancel");
				if (result == 1)
				{
					if (0!=player->items.at(item_index)->use_func)
					{
						player->items.at(item_index)->use_func(t);
						// remove the item from inventory
						player->items.erase(player->items.begin() + item_index);

						// update the inventory listing
						num_items--;
						sprintf_s(nidisplay, 17, "%d Items", num_items);
						cl->draw_button(box[0]+box[2]-19,y-4,19,3,nidisplay,1|2|4,0);
					}
				}
				else if (result == 2)
				{
					if (0!=player->items.at(item_index)->drop_func)
					{
						player->items.at(item_index)->drop_func(t);
						// remove the item from inventory
						player->items.erase(player->items.begin() + item_index);

						// update the inventory listing
						num_items--;
						sprintf_s(nidisplay, 17, "%d Items", num_items);
						cl->draw_button(box[0]+box[2]-19,y-4,19,3,nidisplay,1|2|4,0);
					}

				}
				else if (result == 3)
				{
					// do nothing
				}
				delete t;
				

			} break;
		}// end switch getkey
	} // end while not closebox

	cl->close_window(wnd);
}
