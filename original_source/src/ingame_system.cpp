
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

// save and load games
void ingame_system()
{
	// cl->show_message(3,"TODO: Ingame System Screen","","Sorry not implemented yet");

	int result = cl->show_choice_ex("",3,"Save Game","Load Game","Cancel");
	switch(result)
	{
	case 1: // SHOW SAVE SCREEN
		savegame_screen();
		break;
	case 2: // SHOW LOAD SCREEN
		loadgame_screen();
		break;
	case 3:
		return;
		break;
	}
}

// save and load screens are similar to the character creation screen
// you have 4 options, GAME 1 GAME 2 GAME 3 CANCEL
// which means you can have up to 3 different save games

const int SL_OPTION_GAME1	= 0;
const int SL_OPTION_GAME2	= 1;
const int SL_OPTION_GAME3	= 2;
const int SL_OPTION_CANCEL	= 3;

char* sl_game1_icon = "@G@A@M@E@@###@@@@@@@#@@@@@@@#@@@@@@@#@@@@@####@@";
char* sl_game2_icon = "@G@A@M@E@@###@@@@@@@@#@@@@@##@@@@@#@@@@@@@####@@";
char* sl_game3_icon = "@G@A@M@E@@##@@@@@@@@#@@@@@##@@@@@@@@#@@@@@##@@@@";
char* sl_cancel_icon = "@CANCEL@@@/@@@@@@/ -----<  =====@\\ #####@@\\@@@@@";

// BIG TEXT 48x5
char* savegame_bigtext = "@ssss @aaa@ v@@@v @eeee @gggg @aaa@ @m@m@ @eeees@@@@ a@@@a v@@@v e@@@@ g@@@@ a@@@a m@m@m e@@@@@sss@ aaa@a v@@@v eee@@ g@ggg aaa@a m@m@m eee@@@@@@s a@@@a @v@v@ e@@@@ g@@@g a@@@a m@m@m e@@@@ssss@ a@@@a @@v@@ @eeee @gggg a@@@a m@@@m @eeee";
char* loadgame_bigtext = "l@@@@ @ooo@ @aaa@ dddd@ @gggg @aaa@ @m@m@ @eeeel@@@@ o@@@o a@@@a d@@@d g@@@@ a@@@a m@m@m e@@@@l@@@@ o@@@o aaa@a d@@@d g@ggg aaa@a m@m@m eee@@l@@@@ o@@@o a@@@a d@@@d g@@@g a@@@a m@m@m e@@@@lllll @ooo@ a@@@a dddd@ @gggg a@@@a m@@@m @eeee";


void draw_sl_icon(int x, int y, char* icon, bool active, char mask='@');
void draw_sl_icon(int x, int y, char* icon, bool active, char mask)
{
	const int icon_w = 8;
	const int icon_h = 6;

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

void draw_sl_bigtext(int x, int y, char* icon, char mask='@');
void draw_sl_bigtext(int x, int y, char* icon, char mask)
{
	int icon_w = 47;
	int icon_h = 5;

	cl->set_bgcolor(0);
	
	for (int py = 0; py < icon_h; py++)
	{
		if (py >= 0 && py <= icon_h/3)
		{
			cl->set_fgcolor(1|2|4|8);
		}
		else if (py >= icon_h/3 && py <= (icon_h/3)*2)
		{
			cl->set_fgcolor(1|2|4);
		}
		else
		{
			cl->set_fgcolor(8);
		}

		for (int px = 0; px < icon_w; px++)
		{
			if (icon[px + py * icon_w] != mask)
			{
				if (icon[px + py * icon_w] != ' ')
				{
					cl->outchar(x + px, y + py, (unsigned char)219);
				}
				else
					cl->outchar(x + px, y + py, icon[px + py * icon_w]);
			}
		}
	}
}

void draw_sl_options(int x, int y, int active)
{
	if (active == SL_OPTION_GAME1)
	{
		draw_sl_icon(x, y, sl_game1_icon, true);	
		cl->draw_button(x,y,13,3," GAME #1 ",4,1|2|4|8);
	}
	else
	{
		draw_sl_icon(x, y, sl_game1_icon, false);	
		cl->draw_button(x,y,13,3," GAME #1 ",0,1|2|4);
	}

	if (active == SL_OPTION_GAME2)
	{
		draw_sl_icon(x+14, y, sl_game2_icon, true);
		cl->draw_button(x+14,y,13,3," GAME #2 ",4,1|2|4|8);
	}
	else
	{
		draw_sl_icon(x+14, y, sl_game2_icon, false);
		cl->draw_button(x+14,y,13,3," GAME #2 ",0,1|2|4);
	}

	if (active == SL_OPTION_GAME3)
	{
		draw_sl_icon(x+29, y, sl_game3_icon, true);
		cl->draw_button(x+29,y,13,3," GAME #3 ",4,1|2|4|8);
	}
	else
	{
		draw_sl_icon(x+29, y, sl_game3_icon, false);
		cl->draw_button(x+29,y,13,3," GAME #3 ",0,1|2|4);
	}

	if (active == SL_OPTION_CANCEL)
	{
		draw_sl_icon(x+43, y, sl_cancel_icon, true);
		cl->draw_button(x+43,y,13,3," CANCEL  ",4,1|2|4|8);
	}
	else
	{
		draw_sl_icon(x+43, y, sl_cancel_icon, false);
		cl->draw_button(x+43,y,13,3," CANCEL  ",0,1|2|4);
	}
}

bool save_game_ok(int id)
{
	char savegame_filename[1024];
	sprintf_s(savegame_filename,1024,"data\\GAME%03d.SAV", id);

	FILE* fp;
	fopen_s(&fp, savegame_filename, "wb");
	if(fp)
	{
		fwrite("SAWD",sizeof(char),4,fp);
		fwrite(&current_map, sizeof(int), 1, fp);
		fwrite(&previous_map, sizeof(int), 1, fp);
		// create story uchar
		unsigned int story_uchar = 0;
		if (entered_thieves_hut) story_uchar |= 1;
		if (have_treasure_room_key) story_uchar |= 2;
		if (entered_treasure_room) story_uchar |= 4;
		if (accepted_beer_quest) story_uchar |= 8;
		if (have_found_beer) story_uchar |= 16;
		if (entered_karns_pub) story_uchar |= 32;
		if (beggars_bar_open) story_uchar |= 64;

		fwrite(&story_uchar, sizeof(unsigned char), 1, fp);

		fwrite(&player->x, sizeof(int), 1, fp);
		fwrite(&player->y, sizeof(int), 1, fp);
		fwrite(player->name, sizeof(char), 32, fp);
		fwrite(&player->chr_class, sizeof(int), 1, fp);
		fwrite(&player->max_ap, sizeof(int), 1, fp);
		fwrite(&player->cur_ap, sizeof(int), 1, fp);
		fwrite(&player->max_hp, sizeof(int), 1, fp);
		fwrite(&player->cur_hp, sizeof(int), 1, fp);
		fwrite(&player->max_mp, sizeof(int), 1, fp);
		fwrite(&player->cur_mp, sizeof(int), 1, fp);
		fwrite(&player->attack, sizeof(int), 1, fp);
		fwrite(&player->defense, sizeof(int), 1, fp);
		fwrite(&player->strength, sizeof(int), 1, fp);
		fwrite(&player->magic, sizeof(int), 1, fp);
		fwrite(&player->level, sizeof(int), 1, fp);
		fwrite(&player->steps, sizeof(int), 1, fp);

		fwrite(&player->experience, sizeof(unsigned long), 1, fp);
		fwrite(&player->gold, sizeof(unsigned long), 1, fp);

		// store items
		unsigned int item_count = player->items.size();
		fwrite(&item_count, sizeof(unsigned int), 1, fp);
		{
			for (unsigned int i = 0; i < player->items.size(); i++)
			{
				ITEM_ID id = get_item_id(player->items.at(i)->name);
				fwrite(&id, sizeof(ITEM_ID), 1, fp);
			}
		}

		unsigned int equipped_item_count = player->items_equipped.size();
		fwrite(&equipped_item_count, sizeof(unsigned int), 1, fp);
		{
			for (unsigned int i = 0; i < player->items_equipped.size(); i++)
			{
				ITEM_ID id = get_item_id(player->items_equipped.at(i));
				fwrite(&id, sizeof(ITEM_ID), 1, fp);
			}
		}

		// store magic
		unsigned int spell_count = player->spells.size();
		fwrite(&spell_count, sizeof(unsigned int), 1, fp);
		{
			for (unsigned int i = 0; i < player->spells.size(); i++)
			{
				MAGIC_ID id = get_magic_id(player->spells.at(i)->name);
				fwrite(&id, sizeof(MAGIC_ID), 1, fp);
			}
		}

		fclose(fp);
	}
	else
	{
		return false;
	}
	/*
	SAVE FORMAT
	SAWD -- header
	(int) current_map
	(int) previous_map
	(unsigned char) story_uchar (see below)

	int x, y; 
	char name[32];
	int chr_class;
	int max_ap, cur_ap;
	int max_hp, cur_hp;
	int max_mp, cur_mp;

	(int) attack
	(int) defense
	(int) strength
	(int) magic
	(int) level
	(int) steps
	(unsigned long) xp
	(unsigned long) gold

	(int) num_items
	(int) item_id list
	(int) num_items_equipped
	(int) item_id list
	(int) num_spells
	(int) spell_id list
	
	*/

	return true; // debugging
}

bool load_game_ok(int id)
{
	char savegame_filename[1024];
	sprintf_s(savegame_filename,1024,"data\\GAME%03d.SAV", id);

	FILE* fp;
	fopen_s(&fp, savegame_filename, "rb");
	if(fp)
	{

		//cl->show_message(3,"","Reading Header...","");

		// read header
		char header[5] = {0};
		fread_s(&header, 4, sizeof(char), 4, fp);
		if (!(header[0] == 'S' &&
			header[1] == 'A' &&
			header[2] == 'W' &&
			header[3] == 'D'))
		{
			fclose(fp);
			return false;
		}

		//cl->show_message(3,"","Reading Map Positions...","");
		// read map positions
		fread_s(&current_map, sizeof(int), sizeof(int), 1, fp);
		fread_s(&previous_map, sizeof(int), sizeof(int), 1, fp);
		
		//cl->show_message(3,"","Reading Story Flags...","");
		// read story flags
		unsigned int story_uchar = 0;
		fread_s(&story_uchar, sizeof(unsigned char), sizeof(unsigned char), 1, fp);
		if (story_uchar & 1) entered_thieves_hut = true;
		if (story_uchar & 2) have_treasure_room_key = true;
		if (story_uchar & 4) entered_treasure_room = true;
		if (story_uchar & 8) accepted_beer_quest = true;
		if (story_uchar & 16) have_found_beer = true;
		if (story_uchar & 32) entered_karns_pub = true;
		if (story_uchar & 64) beggars_bar_open = true;


		// read player info
		//cl->show_message(3,"","Reading Player X/Y...","");

		fread_s(&player->x, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->y, sizeof(int), sizeof(int), 1, fp);

		//cl->show_message(3,"","Reading Player Name...","");

		fread_s(player->name, 32, sizeof(char), 32, fp);

		//cl->show_message(3,"","Reading Player Class...","");

		fread_s(&player->chr_class, sizeof(int), sizeof(int), 1, fp);

		//cl->show_message(3,"","Reading Player Stats...","");

		fread_s(&player->max_ap, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->cur_ap, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->max_hp, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->cur_hp, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->max_mp, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->cur_mp, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->attack, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->defense, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->strength, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->magic, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->level, sizeof(int), sizeof(int), 1, fp);
		fread_s(&player->steps, sizeof(int), sizeof(int), 1, fp);

		fread_s(&player->experience, sizeof(unsigned long), sizeof(unsigned long), 1, fp);
		fread_s(&player->gold, sizeof(unsigned long), sizeof(unsigned long), 1, fp);

		//cl->show_message(3,"","Reading Player Items...","");
		// load items
		player->items.clear();
		unsigned int item_count = 0;
		fread_s(&item_count, sizeof(unsigned int), sizeof(unsigned int), 1, fp);
		{
			for (unsigned int i = 0; i < item_count; i++)
			{
				ITEM_ID id = 0;
				fread_s(&id, sizeof(ITEM_ID), sizeof(ITEM_ID), 1, fp);
				player->items.push_back(master_item_list.at(id));
			}
		}

		//cl->show_message(3,"","Reading Player's Equipped Items...","");
		unsigned int equipped_item_count = 0;
		player->items_equipped.clear();
		fread_s(&equipped_item_count, sizeof(unsigned int), sizeof(unsigned int), 1, fp);
		{
			for (unsigned int i = 0; i < equipped_item_count; i++)
			{
				ITEM_ID id = 0;
				fread_s(&id, sizeof(ITEM_ID), sizeof(ITEM_ID), 1, fp);
				player->items_equipped.push_back(master_item_list.at(id)->name);
			}
		}

		//cl->show_message(3,"","Reading Player's Magic...","");
		// read magic
		unsigned int spell_count = 0;
		player->spells.clear();
		fread_s(&spell_count, sizeof(unsigned int), sizeof(unsigned int), 1, fp);
		{
			for (unsigned int i = 0; i < spell_count; i++)
			{
				MAGIC_ID id = 0;
				fread_s(&id, sizeof(MAGIC_ID), sizeof(MAGIC_ID), 1, fp);
				player->spells.push_back(master_magic_list.at(id));
			}
		}

		//cl->show_message(3,"","Finished Reading Savegame...","");
		fclose(fp);
	}
	else
	{
		return false;
	}
	/*
	SAVE FORMAT
	SAWD -- header
	(int) current_map
	(int) previous_map
	(unsigned char) story_uchar (see below)

	int x, y; 
	char name[32];
	int chr_class;
	int max_ap, cur_ap;
	int max_hp, cur_hp;
	int max_mp, cur_mp;

	(int) attack
	(int) defense
	(int) strength
	(int) magic
	(int) level
	(int) steps
	(unsigned long) xp
	(unsigned long) gold

	(int) num_items
	(int) item_id list
	(int) num_items_equipped
	(int) item_id list
	(int) num_spells
	(int) spell_id list
	
	*/

	return true; // debugging
}

void savegame_screen()
{
	int active_option = SL_OPTION_GAME1;

	int x = 40-56/2;
	int y = 20;

	clibwindow* black_panel = cl->open_window_noframe(0,0,79,24,0,1|2|4|8);
	
	// draw the big text for save game
	draw_sl_bigtext(17,2,savegame_bigtext);

	draw_sl_options(x, y, active_option);

	bool close_box = false;
	while(!close_box)
	{
		int k = cl->get_key();
		switch (k)
		{
		case VK_LEFT:
			{
				if (active_option > SL_OPTION_GAME1)
				{
					active_option--;
				}
				else if (active_option == SL_OPTION_GAME1)
				{
					active_option = SL_OPTION_CANCEL;
				}
				draw_sl_options(x, y, active_option);
			} break;
		case VK_RIGHT:
			{
				if (active_option < SL_OPTION_CANCEL)
				{
					active_option++;
				}
				else if (active_option == SL_OPTION_CANCEL)
				{
					active_option = SL_OPTION_GAME1;
				}
				draw_sl_options(x, y, active_option);
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				switch(active_option)
				{
				case SL_OPTION_GAME1:
					{
						if (save_game_ok(1))
						{
							cl->show_message(3,"","Game Saved!","");
						}
						close_box = true;
					} break;

				case SL_OPTION_GAME2:
					{
						if (save_game_ok(2))
						{
							cl->show_message(3,"","Game Saved!","");
						}
						close_box = true;
					} break;

				case SL_OPTION_GAME3:
					{
						if (save_game_ok(3))
						{
							cl->show_message(3,"","Game Saved!","");
						}
						close_box = true;
					} break;

				case SL_OPTION_CANCEL:
					{
						close_box = true;
					} break;
				}
			} break;
		} // end switch active option
	} // end while

	cl->close_window(black_panel);
	//draw_game_display();
}

void loadgame_screen()
{
	int active_option = SL_OPTION_GAME1;

	int x = 40-56/2;
	int y = 20;

	clibwindow* black_panel = cl->open_window_noframe(0,0,79,24,0,1|2|4|8);

	// draw the big text for load game
	draw_sl_bigtext(17,2,loadgame_bigtext);
	draw_sl_options(x, y, active_option);

	bool close_box = false;
	while(!close_box)
	{
		int k = cl->get_key();
		switch (k)
		{
		case VK_LEFT:
			{
				if (active_option > SL_OPTION_GAME1)
				{
					active_option--;
				}
				else if (active_option == SL_OPTION_GAME1)
				{
					active_option = SL_OPTION_CANCEL;
				}
				draw_sl_options(x, y, active_option);
			} break;
		case VK_RIGHT:
			{
				if (active_option < SL_OPTION_CANCEL)
				{
					active_option++;
				}
				else if (active_option == SL_OPTION_CANCEL)
				{
					active_option = SL_OPTION_GAME1;
				}
				draw_sl_options(x, y, active_option);
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				switch(active_option)
				{
				case SL_OPTION_GAME1:
					{
						if (load_game_ok(1))
						{
							cl->show_message(3,"","Game Load OK!","");
							gamestate = gsingame;
						}
						close_box = true;
					} break;

				case SL_OPTION_GAME2:
					{
						if (load_game_ok(2))
						{
							cl->show_message(3,"","Game Load OK!","");
							gamestate = gsingame;
						}
						close_box = true;
					} break;

				case SL_OPTION_GAME3:
					{
						if (load_game_ok(3))
						{
							cl->show_message(3,"","Game Load OK!","");
							gamestate = gsingame;
						}
						close_box = true;
					} break;

				case SL_OPTION_CANCEL:
					{
						close_box = true;
					} break;
				}
			} break;
		} // end switch active option
	} // end while

	cl->close_window(black_panel);

	
	//draw_game_display();
}
