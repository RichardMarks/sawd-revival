
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
#include <ctime>
#include "clib.h"
#include "maps.h"
#include "mapevents.h"

//#include "mymidi.h"
#include "sawd.h"


///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

// handle the externs
char*				map[MAP_COUNT];
int					previous_map;
int					current_map;
MapEventManager*	map_manager;
warptarget*			warp_target[1024];
clib*				cl;
obj*				player;
statefunc			gamestate;
/*
MidiManager*		midi_manager;

MIDI_ID				dushal_bgm;
MIDI_ID				kirar_bgm;
MIDI_ID				world_bgm;
MIDI_ID				battlestart_bgm;
MIDI_ID				battle_bgm;
MIDI_ID				battlewin_bgm;
MIDI_ID				rest_bgm;
MIDI_ID				mapbgm[5];
*/

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

int unit_test_battle()
{
	/*
	cl = new clib();
	player = create_obj(8, 9, 1);
	// setup player stats
	sprintf_s(player->name, 32, "Hero");
	
	player->max_hp = 100;
	player->cur_hp = player->max_hp;
	player->max_mp = 15;
	player->cur_mp = player->max_mp;
	player->gold = 100;
	player->items.push_back("HP Potion");
	player->items.push_back("MP Potion");
	player->items.push_back("Fire Scroll");
	player->items.push_back("Fire Scroll");
	player->items.push_back("Fire Scroll");
	player->max_ap = 3;
	player->cur_ap = 1;

	// int result = battle_engine_a(0);
	// int result = battle_engine_b(player);
	obj* enemy = create_obj(0,0,0);
	sprintf_s(enemy->name, 32, "Giant Snake");
	enemy->max_hp = 50; enemy->cur_hp = enemy->max_hp;
	enemy->max_mp = 0; enemy->cur_mp = enemy->max_mp;
	enemy->attack = 1;
	enemy->defense = 1;
	enemy->strength = 2;
	enemy->gold = 25;
	enemy->experience = 35;

	int result = battle_engine_d(enemy);
	delete enemy;
	delete player;
	delete cl;
	return result;
	*/
	return 0;
}

int real_game_main()
{
	cl				= new clib();
	
	map_manager		= new MapEventManager();
	// midi_manager	= new MidiManager();

	ZeroMemory(warp_target, sizeof(warp_target));

	create_master_item_list();
	create_master_magic_list();

	/*
	dushal_bgm		= midi_manager->LoadMidi("data\\music\\dushal.mid");
	kirar_bgm		= midi_manager->LoadMidi("data\\music\\kirar_1.mid");
	world_bgm		= midi_manager->LoadMidi("data\\music\\world.mid");

	battlestart_bgm	= midi_manager->LoadMidi("data\\music\\battles.mid");
	battle_bgm		= midi_manager->LoadMidi("data\\music\\battle.mid");
	battlewin_bgm	= midi_manager->LoadMidi("data\\music\\battlew.mid");
	rest_bgm		= midi_manager->LoadMidi("data\\music\\rpg_rest.mid");

	mapbgm[0]		= world_bgm;
	mapbgm[1]		= kirar_bgm;
	mapbgm[2]		= kirar_bgm;
	mapbgm[3]		= dushal_bgm;
	mapbgm[4]		= dushal_bgm;
	*/

	gamestate = gsinit;

	cl->screen80x25();

	while(gamestate!=0)
	{
		gamestate();
	}
	
	//#######################################
	//## clean up
	//#######################################

	destroy_master_item_list();
	destroy_master_magic_list();

	// delete midi_manager;

	{
		for (int i = 0; i < MAP_COUNT; i++)
		{
			delete map[i];
		}
	}
	delete player;
	delete cl;
	delete map_manager;

	{
		for (int i = 0; i < 1024; i++)
		{
			if (warp_target[i])
			{
				delete warp_target[i];
			}
		}
	}

	return 0;
}

int main(int argc, char* argv[])
{
	{char window_title[255];sprintf_s(window_title,255,"SAWD - %s", 
	GAME_BUILD_NUMBER); SetConsoleTitle(window_title);}

	srand((unsigned int)time(0));

	int result = real_game_main();
	// int result = unit_test_battle();

	return result;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////
void reset_game_vars()
{
	player->x = 8;
	player->y = 9;
	change_map(MAPID_DUSHAL_WEST);
	entered_thieves_hut			= false;
	have_treasure_room_key		= false;
	entered_treasure_room		= false;
	accepted_beer_quest			= false;
	have_found_beer				= false;
	entered_karns_pub			= false;
	player->items.clear();
	player->items_equipped.clear();
	player->spells.clear();
	player->drunk = false;
}

#define ADDEVENT(x,y,m,f) {map_manager->AddEvent(x, y, m, f, 0, false);}
void gsinit()
{
	player = create_obj(8, 9, 1);

	// setup player stats
	sprintf_s(player->name, 32, "Hero");
	

	char* map_names[] =
	{
		"data\\maps\\world.map",
		"data\\maps\\kirarnorth.map",
		"data\\maps\\kirarsouth.map",
		"data\\maps\\dushalwest.map",
		"data\\maps\\dushaleast.map"
	};

	{
		for (int i = 0; i < MAP_COUNT; i++)
		{
			map[i] = load_map(map_names[i]);
		}
	}



	// SETUP MAP WARPS
	{
		warp_target[0] = new warptarget(1, 3, MAPID_DUSHAL_EAST);
		warp_target[1] = new warptarget(28, 6, MAPID_DUSHAL_WEST);
		warp_target[2] = new warptarget(5, 11, MAPID_WORLD_MAP);
		warp_target[3] = new warptarget(28, 10, MAPID_DUSHAL_EAST);

		warp_target[4] = new warptarget(8, 12, MAPID_KIRAR_SOUTH);
		warp_target[5] = new warptarget(23, 3, MAPID_WORLD_MAP);
		warp_target[6] = new warptarget(15, 12, MAPID_KIRAR_NORTH);

		warp_target[7] = new warptarget(25, 1, MAPID_KIRAR_SOUTH);

		// DUSHAL WEST WARPS
		map_manager->AddWarp(29, 6, MAPID_DUSHAL_WEST, warp_to, warp_target[0]);
 
		// DUSHAL EAST WARPS
		map_manager->AddWarp(0, 3, MAPID_DUSHAL_EAST, warp_to, warp_target[1]);
		map_manager->AddWarp(29, 10, MAPID_DUSHAL_EAST, warp_to, warp_target[2]);

		// WORLD_MAP WARPS
		map_manager->AddWarp(4, 11, MAPID_WORLD_MAP, warp_to, warp_target[3]);
		map_manager->AddWarp(23, 2, MAPID_WORLD_MAP, warp_to, warp_target[4]);

		// KIRAR SOUTH WARPS
		map_manager->AddWarp(8, 13, MAPID_KIRAR_SOUTH, warp_to, warp_target[5]);
		map_manager->AddWarp(25, 0, MAPID_KIRAR_SOUTH, warp_to, warp_target[6]);

		// KIRAR NORTH WARPS
		map_manager->AddWarp(15, 13, MAPID_KIRAR_NORTH, warp_to, warp_target[7]);
	}

	// DUSHAL WEST EVENTS
	ADDEVENT(8, 7, MAPID_DUSHAL_WEST, event_dushal_player_home);
	ADDEVENT(16, 4, MAPID_DUSHAL_WEST, event_dushal_magic_shop);

	// DUSHAL EAST EVENTS
	ADDEVENT(18, 10, MAPID_DUSHAL_EAST, event_dushal_inn);
	ADDEVENT(10, 2, MAPID_DUSHAL_EAST, event_dushal_pub);
	ADDEVENT(4, 11, MAPID_DUSHAL_EAST, event_dushal_armor_shop);
	ADDEVENT(3, 11, MAPID_DUSHAL_EAST, event_dushal_weapon_shop);
	ADDEVENT(25, 2, MAPID_DUSHAL_EAST, event_dushal_item_shop);


	// KIRAR SOUTH EVENTS
	ADDEVENT(7, 8, MAPID_KIRAR_SOUTH, event_kirar_inn);
	ADDEVENT(11, 8, MAPID_KIRAR_SOUTH, event_kirar_pub);
	ADDEVENT(20, 8, MAPID_KIRAR_SOUTH, event_kirar_armor_shop);
	ADDEVENT(6, 4, MAPID_KIRAR_SOUTH, event_kirar_magic_shop);
	ADDEVENT(11, 3, MAPID_KIRAR_SOUTH, event_kirar_weapon_shop);
	ADDEVENT(24, 5, MAPID_KIRAR_SOUTH, event_kirar_item_shop);

	// KIRAR NORTH EVENTS
	ADDEVENT(6, 4, MAPID_KIRAR_NORTH, event_kirar_thieves_hut);
	ADDEVENT(8, 9, MAPID_KIRAR_NORTH, event_kirar_treasure_room);
	ADDEVENT(22, 3, MAPID_KIRAR_NORTH, event_kirar_cave_entrance);

	change_map(MAPID_DUSHAL_WEST);
	
	gamestate = gstitle_screen;
	// gamestate = gsname_character;
	// gamestate = gscreate_character;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void gsingame()
{
	draw_game_display();

	int k = cl->get_key();
	switch(k)
	{
	case VK_ESCAPE:
		{
			// gamestate = 0;return;
			ingame_menu();
		} break;
	case VK_UP:{{char d='u';if (can_move(player, d)){move_obj(player, d);}}} break;
	case VK_DOWN:{{char d='d';if (can_move(player, d)){move_obj(player, d);}}} break;
	case VK_LEFT:{{char d='l';if (can_move(player, d)){move_obj(player, d);}}} break;
	case VK_RIGHT:{{char d='r';if (can_move(player, d)){move_obj(player, d);}}} break;
	}

	// handle any events on the map if we moved
	if (k==VK_UP||k==VK_DOWN||k==VK_LEFT||k==VK_RIGHT)
	{
		//##
		//## get sober
		if (player->drunk)
			player->drinks--;
		if (player->drinks < 0) 
		{
			player->drinks = 0;
			player->drunk = false;
			cl->show_message_centered(3,"","Hooray! You are not drunk anymore! Go have a drink!","");
		}

		for (int i = 0; i < map_manager->EventCount(); i++)
		{
			mapevent* e = map_manager->GetEvent(i);
			if (e->enabled && e->map_id == current_map)
			{
				if (e->x == player->x && e->y == player->y)
				{

					e->func((void*)e->data);
					
					if (e->onetime)
					{
						e->enabled = false;
					}
				}
			}
		}

		player->steps++; 
	}

	// spawn random battle if we are on the world map
	if (current_map == MAPID_WORLD_MAP)
	{
		if (player->steps > 0 && (player->steps % 19) == 0)
		{
			//==========================================================
			//==
			// NAME				HP/MP/GOLD/XP		ATK/DFN/STR
			// Giant Snake		25/0/25/45			2/2/1
			// Grizzly Bear		120/0/75/100		4/6/2
			// Poisonous Plant	365/0/0/250			1/1/1
			obj** enemies = new obj* [3];

			enemies[0] = create_enemy("Giant Snake", 1, 25, 0, 25, 45, 2, 2, 1);
			enemies[1] = create_enemy("Grizzly Bear", 3, 120, 0, 75, 100, 4, 6, 2);
			enemies[2] = create_enemy("Poisonous Plant", 2, 365, 0, 0, 250, 1, 1, 1);

			enemies[0]->battle_icon_w = 25;
			enemies[0]->battle_icon_h = 15;
			enemies[0]->battle_icon = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@eeeee@@@@@@@@@@@@@@@@@@@e     ee@@@@@@@@@@@@@@@@e  w(!)  ee@@@@@@@@@@@@@@e e@e  eeev@@@@@@@@@@@@@e e@@@ee\\_V@@@@@@@@@@@@@@e e@@@@ee@\\_@@e@@@@@@@@@@e  e@@@@ee@\\@@@ee@@@@@@@@@e  eee@@@@@@@@ee@@@@@@@e  e    eeeee@@@ee@@@@@@e mm ee    e  eee e@@@@@@e      e    eeeeee@@@@@@@@wwwww  e   e@@@@@@@@@@@@@@@@@@e   ee@@@@@@@@@@@@@@@@@@@@eee@@@@@@@@@@";
			
			enemies[1]->battle_icon_w = 25;
			enemies[1]->battle_icon_h = 15;
			enemies[1]->battle_icon = "@@@@@@@@@@@@@@xxxx@@@@@@@@@@@@@@@@@xxxxxxxxxx@@@@@@@@@@@@@xxxxxxxxxxxxxx@@@@@@@@@@xxxxxxxxxxxxxxxx@@@@@@@xxxxxxxxxxxxxxxxxxx@@@@@xxxxxxxxxxxxxxxxxxxx@@@@xxxxxxxxxxxxxxxxxxxxx@@@@xxxxxxxxxxxxxxxxxxxxx@@@xxxxxxxxxxxxxxxxxxxxxx@@xxxxxxxxxxxxxxxxxxx@xxx@xxxxxxxxxxx@xxx@xxx@@xxx@@xxxx@@xxx@@xxx@xxx@xxx@@@@@@@@xxx@@@xxx@@@@@xxx@@@@@@@@@@@@@xxx@@@@@@@@@@@";

			enemies[2]->battle_icon_w = 31;
			enemies[2]->battle_icon_h = 8;
			enemies[2]->battle_icon = "@@@@@@@@@@@@@.--.@@@@@@@@@@@@@@@@@@@@@.-~-:'@@@@':-~-.@@@@@@@@@@@@.-/@@@@@'.@@.'@@@@@\\-.@@@@@@@@;__|@@@@@@_::_@@@@@@|__;@@@@@/'@@@'.@@@/' \\/ '\\@@@.'@@@'\\@@@|@@@@@@_@@\\.()()./@@_@@@@@@|@@@\\@@@@/' '.@'.().'@.' '\\@@@@/@@/@'-._'.@@_~@.__.@~_@@.'_.-'@\\@";


			// primitive zoning for enemies
			int e_id = 0;
			if (player->y > 0 && player->y <= (MAP_H / 3)) e_id = 2;
			if (player->y > (MAP_H / 3) && player->y <= (MAP_H/3)*2) e_id = 1;
			if (player->y > (MAP_H / 3)*2) e_id = 0;

			// item rewards!
			int item_reward = rand()%100;
			if (item_reward>=88)
			{
				for (int qty = 0 ; qty < 2+rand()%6-2; qty++)
				{
					if (rand()%8==3)
					{
						enemies[e_id]->items.push_back(get_master_item("Anti-Venom"));
					}
					else
					{
						enemies[e_id]->items.push_back(get_master_item("Healing Powder"));
					}
				}
			}

			int result = battle_engine_a(enemies[e_id]);
			if (result == BATTLE_OPTION_FIGHT)
			{
				int battle_result = battle_engine_d(enemies[e_id]);
				if (battle_result == BATTLE_RESULT_WIN)
				{
					// you won or ran away!
				}
				else if (battle_result == BATTLE_RESULT_LOSE)
				{
					cl->show_message(3, "","You were defeated!","");
					cl->show_message(3, "", "G A M E  O V E R","");
					gamestate = gsmain_menu;
				}
			}
			else if (result == BATTLE_OPTION_RUN)
			{
				// chance to run away
			}
			delete enemies[0];
			delete enemies[1];
			delete enemies[2];
			delete [] enemies;
			player->steps = 0;

			//==
			//==========================================================
		} // end if we have taken 19 steps
	} // end if on world map


	// audio disabled
	//if (!midi_manager->GetMidi(mapbgm[current_map])->IsPlaying())
	//{
	//	midi_manager->GetMidi(mapbgm[current_map])->Play(true);
	//}

}

obj* create_enemy(char* name, int lvl, int mhp, int mmp, long gold, long xp, int atk, int dfn, int str)
{
	obj* o = create_obj(0,0,0);
	//o->name				= name;
	sprintf_s(o->name, 16, "%s", name);

	o->max_hp			= mhp; o->cur_hp = mhp;
	o->max_mp			= mmp; o->cur_mp = mmp;
	o->max_ap			= 1;   o->cur_ap = 1;
	o->attack			= atk;
	o->defense			= dfn;
	o->strength			= str;
	o->gold				= gold;
	o->experience		= xp;
	o->level			= lvl;
	o->battle_icon		= "????????????????";
	o->battle_icon_w	= 4;
	o->battle_icon_h	= 4;
	return o;
}
///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

obj* create_obj(int x, int y, unsigned char image)
{
	obj* o = new obj;
	o->x = x;
	o->y = y;
	o->image = image;

	sprintf_s(o->name, 32, "                ");
	o->chr_class = 0;
	o->max_hp = 0;
	o->cur_hp = 0;
	o->max_mp = 0;
	o->cur_mp = 0;
	o->attack = 0;
	o->defense = 0;
	o->strength = 0;
	o->magic = 0;
	o->gold = 0;
	o->experience = 0;
	o->level = 1;
	o->steps = 0;


	return o;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void draw_obj(obj* o)
{
	cl->outchar(o->x, o->y, o->image);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

char* load_map(char* file_name)
{
	char* dest_map = new char[MAP_W * MAP_H];
	FILE* fp;
	fopen_s(&fp, file_name, "r");
	if (!fp){ delete dest_map; return 0; }
	fread_s(dest_map, MAP_W * MAP_H, sizeof(char), MAP_W * MAP_H, fp);
	fclose(fp);
	return dest_map;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void change_map(int new_map)
{
	previous_map = current_map;
	current_map = new_map;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void draw_map()
{
	char* mapnames[] =
	{
		"Tiron", "Kirar North", "Kirar South", "Dushal West", "Dushal East"
	};

	cl->set_bgcolor(0); cl->set_fgcolor(1|2|4|8);
	cl->outcharsf(1,MAP_H+1,"                                ");
	cl->outcharsf(1,MAP_H+1,"Current Location: %s", mapnames[current_map]);

	for (int y = 0; y < MAP_H; y++)
	{
		for (int x = 0; x < MAP_W; x++)
		{
			char tile = map[current_map][x + (y * MAP_W)];

			if (tile == 'T') // tree
			{
				cl->set_bgcolor(2);
				cl->set_fgcolor(2|8);
				cl->outchar(x,y,5);
			}
			else if (tile == 'W') // water
			{
				cl->set_bgcolor(4);
				cl->set_fgcolor(4|8);
				cl->outchar(x,y,(char)247);
			}
			else if (tile == 'H') // house or town
			{
				cl->set_bgcolor(2);
				cl->set_fgcolor(1|2);
				cl->outchar(x,y,127);
			}
			else if (tile == ' ') // grass
			{
				cl->set_bgcolor(2);
				cl->set_fgcolor(1|2|4|8);
				cl->outchar(x,y,32);
			}
			else if (tile == '.') // path
			{
				cl->set_bgcolor(1|2);
				cl->set_fgcolor(1|2|4);
				cl->outchar(x,y,'.');
			}
			else if (tile == '#') // wall
			{
				cl->set_bgcolor(1|2|4);
				cl->set_fgcolor(0);
				cl->outchar(x,y,'#');
			}
			else
			{
				cl->set_bgcolor(2);
				cl->set_fgcolor(1|2|4|8);
				cl->outchar(x,y,tile);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void debug_draw_event_locations()
{
	char* mapnames[] =
	{
		"World",
		"Kirar North",
		"Kirar South",
		"Dushal West",
		"Dushal East"
	};

	int x = 52;
	int y = 1;
	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
	cl->outcharsf(x,y,"Curr Map = %s", mapnames[current_map]);y++;
	cl->outcharsf(x,y,"Prev Map = %s", mapnames[previous_map]);y++;
	cl->outcharsf(x,y,"Pos %02d, %02d", player->x, player->y);y++;y++;

	cl->outchars(x,y,"Story Flags:");y++;y++;
	if (entered_thieves_hut)cl->outchars(x,y,"[X] entered_thieves_hut");else cl->outchars(x,y,"[ ] entered_thieves_hut");
	y++;
	if (have_treasure_room_key)cl->outchars(x,y,"[X] have_treasure_room_key");else cl->outchars(x,y,"[ ] have_treasure_room_key");
	y++;
	if (entered_treasure_room)cl->outchars(x,y,"[X] entered_treasure_room");else cl->outchars(x,y,"[ ] entered_treasure_room");
	y++;
	if (accepted_beer_quest)cl->outchars(x,y,"[X] accepted_beer_quest");else cl->outchars(x,y,"[ ] accepted_beer_quest");
	y++;
	if (have_found_beer)cl->outchars(x,y,"[X] have_found_beer");else cl->outchars(x,y,"[ ] have_found_beer");
	y++;
	if (entered_karns_pub)cl->outchars(x,y,"[X] entered_karns_pub");else cl->outchars(x,y,"[ ] entered_karns_pub");



	for (int i = 0; i < map_manager->EventCount(); i++)
	{
		mapevent* e = map_manager->GetEvent(i);
		if (e->enabled && e->map_id == current_map)
		{
			cl->set_bgcolor(1|2|8);
			cl->set_fgcolor(1|8);
			cl->outchar(e->x,e->y,(char)136);
		}
	}
	cl->set_bgcolor(0);
	cl->set_fgcolor(1|2|4|8);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

bool can_move(obj* o, char d)
{
	bool space_is_open = false;
	char tile = 'x';
	
	switch(d)
	{
	case 'u': tile = map[current_map][o->x + ((o->y-1) * MAP_W)]; break;
	case 'd': tile = map[current_map][o->x + ((o->y+1) * MAP_W)]; break;
	case 'l': tile = map[current_map][(o->x-1) + (o->y * MAP_W)]; break;
	case 'r': tile = map[current_map][(o->x+1) + (o->y * MAP_W)]; break;
	default: break;
	}

	if (tile == ' ' || tile == '.' || tile == 'H')
	{
		space_is_open = true;
	}
	return space_is_open;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void move_obj(obj* o, char d)
{
	switch(d)
	{
	case 'u': o->y--; break;
	case 'd': o->y++; break;
	case 'l': o->x--; break;
	case 'r': o->x++; break;
	default: break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void draw_game_display()
{
	draw_map();
	// debug_draw_event_locations();
	cl->set_bgcolor(0); cl->set_fgcolor(1|2|4|8);
	draw_obj(player);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

bool obj_has_item(obj* o, char* name)
{
	for (unsigned int i = 0; i < o->items.size(); i++)
	{
		if (strcmp(o->items.at(i)->name, name)==0)
		{
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

bool obj_has_equipped(obj* o, char* name)
{
	for (unsigned int i = 0; i < o->items_equipped.size(); i++)
	{
		if (strcmp(o->items_equipped.at(i), name)==0)
		{
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

bool obj_has_spell(obj* o, char* name)
{
	for (unsigned int i = 0; i < o->spells.size(); i++)
	{
		if (strcmp(o->spells.at(i)->name, name)==0)
		{
			return true;
		}
	}
	return false;
}
