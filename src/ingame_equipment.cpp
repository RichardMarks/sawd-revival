
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

void display_item_detail(char* name)
{
	shop_item* itm = get_master_item(name);
	if (itm==0)return;
	itm->modifiers;

	int box[] = {20, 4, 40, 18};
	int x = box[0] + 2;
	int y = box[1] + 2;
	clibwindow* itm_wnd = cl->open_window(box[0], box[1], box[2], box[3], 1|2|4, 0);
	
	cl->outcharsf( x, y, "Name: %s", itm->name);y++;y++;
	
	cl->outcharsf(x, y, "Max HP: %c%3d",(itm->modifiers[ITEM_MODIFIER_MAX_HP]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_MAX_HP]);y++;
	cl->outcharsf(x, y, "Max MP: %c%3d",(itm->modifiers[ITEM_MODIFIER_MAX_MP]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_MAX_MP]);y++;
	cl->outcharsf(x, y, "Max AP: %c%3d",(itm->modifiers[ITEM_MODIFIER_MAX_AP]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_MAX_AP]);y++;
	cl->outcharsf(x, y, "   ATK: %c%3d",(itm->modifiers[ITEM_MODIFIER_ATTACK]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_ATTACK]);y++;
	cl->outcharsf(x, y, "   DEF: %c%3d",(itm->modifiers[ITEM_MODIFIER_DEFENSE]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_DEFENSE]);y++;
	cl->outcharsf(x, y, "   STR: %c%3d",(itm->modifiers[ITEM_MODIFIER_STRENGTH]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_STRENGTH]);y++;
	cl->outcharsf(x, y, "   MAG: %c%3d",(itm->modifiers[ITEM_MODIFIER_MAGIC]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_MAGIC]);y++;
	cl->outcharsf(x, y, "    HP: %c%3d",(itm->modifiers[ITEM_MODIFIER_HP]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_HP]);y++;
	cl->outcharsf(x, y, "    MP: %c%3d",(itm->modifiers[ITEM_MODIFIER_MP]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_MP]);y++;
	cl->outcharsf(x, y, "    AP: %c%3d",(itm->modifiers[ITEM_MODIFIER_AP]>0)?'+':'-',itm->modifiers[ITEM_MODIFIER_AP]);

	x = box[0] + 15;
	y = box[1] + 4;

	if (itm->can_equip)
	{
		cl->outchars(x, y, "Type: Equipment");y++;
		if (itm->chr_class == 9 || (itm->chr_class == player->chr_class))
		{
			cl->outchars(x, y, "Usable: Yes");y++;
		}
		else
		{
			cl->outchars(x, y, "Usable: No");y++;
		}
	}
	else
	{
		cl->outchars(x, y, "Type: Item");y++;
	}

	while(!cl->get_key());

	cl->close_window(itm_wnd);
}

void ingame_equipment()
{
	if ((int)player->items_equipped.size()==0)
	{
		cl->show_message(3,"","You have no equipment!","");
		return;
	}

	int box[] = {41, 3, 36, 19};
	int x = box[0] + 2;
	int y = box[1] + 2;
	
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1|2|4, 0);

	for (unsigned int i = 0; i < player->items_equipped.size(); i++)
	{
		cl->outchars(x + 1, (y + i), player->items_equipped.at(i));
	}

	int cursor_y = 0;

	bool close_box = false;
	
	while(!close_box)
	{
		cl->set_bgcolor(1|2|4);
		cl->set_fgcolor(0);
		{for(int i=0;i<16;i++){cl->outchar(x,y+i,' ');}}
		cl->outchar(x, y + cursor_y, (unsigned char)16);

		int k = cl->get_key();
		switch(k)
		{
		case VK_UP:
			{
				if (cursor_y > 0)
				{
					cursor_y--;
				}
			} break;
		case VK_DOWN:
			{
				if (cursor_y < (int)player->items_equipped.size()-1)
				{
					cursor_y++;
				}
			} break;
		case VK_ESCAPE:
		case VK_BACK:
			{
				close_box = true;
			} break;
		case VK_SPACE:
		case VK_RETURN:
			{
				//char m[80];
				//sprintf_s(m,80,"Item at %4d %s",cursor_y,player->items_equipped.at(cursor_y));
				//cl->show_message(3,"",m,"");
				display_item_detail(player->items_equipped.at(cursor_y));
			} break;
		}// end switch getkey
	} // end while not closebox

	cl->close_window(wnd);
}
