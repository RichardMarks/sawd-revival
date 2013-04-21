
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
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include "clib.h"
#include "sawd.h"

// choose to fight or run
int battle_engine_a(void* d)
{
	char cursor_icon = (char)31;
	int cursor_x = 0;

	// the two buttons
	int btn[2][4] =
	{
		{4, 6, 9, 3},
		{20, 6, 9, 3},
	};
	char* btn_caption[2] = {"FIGHT"," RUN "};

	char* enemy_name = ((obj*)d)->name;

	int box[] = {40 - (38 / 2), (25 / 2) - (14 / 2), 38, 14};
	int x = box[0] + 2;
	int y = box[1] + 2;
	int caption_x = x - 2 + (box[2] / 2) - strlen(enemy_name) / 2;

	char* msg = 0;
	char c = enemy_name[0];
	if (c=='A'||c=='E'||c=='I'||c=='O'||c=='U')
	{
		msg = "You are being attacked by an";
	}
	else
	{
		msg = "You are being attacked by a";
	}

	int msg_x = x - 2 + (box[2] / 2) - strlen(msg) / 2;

	bool close_box = false;

	// draw the form
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1);

	// draw the caption and the message
	cl->outchars(caption_x, y + 2, enemy_name);
	cl->outchars(msg_x, y, msg);

	// draw the buttons
	{for(int n = 0; n < 2; n++)
	{
		cl->draw_button(x + btn[n][0], y + btn[n][1], btn[n][2], btn[n][3], btn_caption[n], 1);
		cl->outchar(x + btn[n][0] + 4, y + 4, ' ');

	}
	}

	int form_result = BATTLE_OPTION_RUN;
	while(!close_box)
	{
		// clear the previous cursors
		{for(int n = 0; n < 2; n++){cl->outchar(x + btn[n][0] + 4, y + 4, ' ');}}

		// draw the cursors in the right location
		if (cursor_x == 0) // FIGHT
		{
			cl->outchar(x + btn[0][0] + 4, y + 4, cursor_icon);
		}
		else if (cursor_x == 1) // RUN
		{
			cl->outchar(x + btn[1][0] + 4, y + 4, cursor_icon);
		}

		int k = cl->get_key();

		switch (k)
		{
		case VK_LEFT:
		case VK_RIGHT:
			{
				if (cursor_x == 0) 
				{
					cursor_x = 1;
				}
				else if (cursor_x == 1)
				{
					cursor_x = 0;
				}
			} break;

		case VK_BACK:
		case VK_ESCAPE:
			{
				form_result = BATTLE_OPTION_RUN;
				close_box = true;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				if (cursor_x == 0) // FIGHT
				{
					form_result = BATTLE_OPTION_FIGHT;
				}
				else if (cursor_x == 1) // RUN
				{
					form_result = BATTLE_OPTION_RUN;
				}
				close_box = true;
			} break;
		}
	}

	cl->close_window(wnd);
	return form_result;
}

// choose an item to use
int battle_engine_b(void* d)
{
	obj* e = (obj*)d;
	int num_items = (int)player->items.size();
	if (num_items == 0)
	{
		cl->show_message_centered(3,"","You have no items!","");
		return BATTLE_OPTION_CANCEL;
	}
	
	char itm_cursor_icon_l = (char)16;
	char itm_cursor_icon_r = (char)17;

	int cursor_y = 0;

	
	int box[] = {40 - (25 / 2), (25 / 2) - ((num_items+4) / 2), 25, (num_items+4)};
	int x = box[0] + 2;
	int y = box[1] + 2;
	
	bool close_box = false;

	// draw the form
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1);

	// draw the available items
	{for(int n = 0; n < num_items; n++)
	{
		cl->outchars(x + 2, y + n, player->items.at(n)->name);
	}
	}

	int form_result = BATTLE_OPTION_CANCEL;
	while(!close_box)
	{
		{ // clear all cursor positions
			for(int n=0;n<num_items;n++)
			{
				cl->outchar(x, y + n, ' '); // left item cursor
				cl->outchar(x + 19, y + n, ' '); // right item cursor
			}
		} // end clear cursor positions

		// draw the cursor where it should be
		cl->outchar(x, y + cursor_y, itm_cursor_icon_l);
		cl->outchar(x + 19, y + cursor_y, itm_cursor_icon_r);

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
					cursor_y = num_items-1;
				}
			} break;
		case VK_DOWN:
			{
				if (cursor_y < num_items-1) 
				{
					cursor_y++;
				}
				else if (cursor_y >= num_items-1)
				{
					// move to the first item
					cursor_y = 0;
				}
			} break;

		case VK_BACK:
		case VK_ESCAPE:
			{
				form_result = BATTLE_OPTION_CANCEL;
				close_box = true;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				form_result = BATTLE_OPTION_ACCEPT;

				int pick_target = cl->show_choice_ex("Select a target:",3,player->name,e->name,"Cancel");

				item_target* t = 0;
				switch(pick_target)
				{
				case 1:
					t = new item_target(player->items.at(cursor_y), player);
					break;
				case 2:
					t = new item_target(player->items.at(cursor_y), e);
					break;
				case 3:
					form_result = BATTLE_OPTION_CANCEL;
					close_box = true;
					cl->close_window(wnd);
					return form_result;
					break;
				}

				// cl->show_message(2, "Selected:", player->items.at(cursor_y)->name);
				if (0!=player->items.at(cursor_y)->use_func)
				{
					player->items.at(cursor_y)->use_func(t);
					// remove the item from inventory
					player->items.erase(player->items.begin() + cursor_y);
				}
				delete t;

				close_box = true;
			} break;
		}
	}

	cl->close_window(wnd);
	return form_result;
}

// choose a spell to cast
int battle_engine_c(void* d)
{
	obj* e = (obj*)d;
	int num_items = (int)player->spells.size();

	if (num_items == 0)
	{
		cl->show_message_centered(3,"","You have no magic!","");
		return BATTLE_OPTION_CANCEL;
	}
	
	char itm_cursor_icon_l = (char)16;
	char itm_cursor_icon_r = (char)17;

	int cursor_y = 0;

	
	int box[] = {40 - (25 / 2), (25 / 2) - ((num_items+4) / 2), 25, (num_items+4)};
	int x = box[0] + 2;
	int y = box[1] + 2;
	
	bool close_box = false;

	// draw the form
	clibwindow* wnd = cl->open_window(box[0], box[1], box[2], box[3], 1);

	// draw the available items
	{for(int n = 0; n < num_items; n++)
	{
		cl->outchars(x + 2, y + n, player->spells.at(n)->name);
	}
	}

	int form_result = BATTLE_OPTION_CANCEL;
	while(!close_box)
	{
		{ // clear all cursor positions
			for(int n=0;n<num_items;n++)
			{
				cl->outchar(x, y + n, ' '); // left item cursor
				cl->outchar(x + 19, y + n, ' '); // right item cursor
			}
		} // end clear cursor positions

		// draw the cursor where it should be
		cl->outchar(x, y + cursor_y, itm_cursor_icon_l);
		cl->outchar(x + 19, y + cursor_y, itm_cursor_icon_r);

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
					cursor_y = num_items-1;
				}
			} break;
		case VK_DOWN:
			{
				if (cursor_y < num_items-1) 
				{
					cursor_y++;
				}
				else if (cursor_y >= num_items-1)
				{
					// move to the first item
					cursor_y = 0;
				}
			} break;

		case VK_BACK:
		case VK_ESCAPE:
			{
				form_result = BATTLE_OPTION_CANCEL;
				close_box = true;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				form_result = BATTLE_OPTION_ACCEPT;

				int pick_target = cl->show_choice_ex("Select a target:",3,player->name,e->name,"Cancel");

				item_target* t = 0;
				switch(pick_target)
				{
				case 1:
					t = new item_target(player->spells.at(cursor_y), player);
					break;
				case 2:
					t = new item_target(player->spells.at(cursor_y), e);
					break;
				case 3:
					form_result = BATTLE_OPTION_CANCEL;
					close_box = true;
					cl->close_window(wnd);
					return form_result;
					break;
				}

				// cl->show_message(2, "Selected:", player->items.at(cursor_y)->name);
				if (0!=player->spells.at(cursor_y)->cast_func)
				{
					player->spells.at(cursor_y)->cast_func(t);
				}
				delete t;

				close_box = true;
			} break;
		}
	}

	cl->close_window(wnd);
	return form_result;
}


// helper routine to draw the battle menu widget
const int BTLMNU_ATTACK = 0;
const int BTLMNU_DEFEND = 1;
const int BTLMNU_ITEM   = 2;
const int BTLMNU_CAST   = 3;

void draw_battle_menu_widget(int x, int y, int active)
{
	cl->set_bgcolor(1); cl->set_fgcolor(1|2|4|8);
	cl->outchar(x + 13, y + 5, (unsigned char)17); // <
	cl->outchar(x + 15, y + 5, (unsigned char)16); // >
	cl->outchar(x + 14, y + 4, (unsigned char)30); // ^
	cl->outchar(x + 14, y + 6, (unsigned char)31); // v

	if (active==BTLMNU_ATTACK)
	{
		cl->draw_button(x, y + 4, 10, 3, "ATTACK", 1|2|8, 0);
	}
	else
	{
		cl->draw_button(x, y + 4, 10, 3, "ATTACK", 1);
	}

	if (active==BTLMNU_DEFEND)
	{
		cl->draw_button(x+18, y + 4, 10, 3, "DEFEND", 1|2|8, 0);
	}
	else
	{
		cl->draw_button(x+18, y + 4, 10, 3, "DEFEND", 1);
	}

	if (active==BTLMNU_ITEM)
	{
		cl->draw_button(x+9, y, 10, 3, " ITEM ", 1|2|8, 0);
	}
	else
	{
		cl->draw_button(x+9, y, 10, 3, " ITEM ", 1);
	}

	if (active==BTLMNU_CAST)
	{
		cl->draw_button(x+9, y + 8, 10, 3, " CAST ", 1|2|8, 0);
	}
	else
	{
		cl->draw_button(x+9, y + 8, 10, 3, " CAST ", 1);
	}
}

void draw_battle_player_widget(int x, int y)
{
	cl->set_bgcolor(1); cl->set_fgcolor(1|2|4|8);
	cl->outchars(x, y, player->name);
	cl->outcharsf(x, y + 2, "HP %4d / %4d", player->cur_hp, player->max_hp);
	cl->outcharsf(x, y + 4, "MP %4d / %4d", player->cur_mp, player->max_mp);

	// draw the AP gauge
	if (player->max_ap < 8)
	{
	cl->outchars(x, y + 6, "AP");
	int apx = x+3;
	for (int n = 0; n < player->max_ap; n++)
	{
		cl->outchars(apx, y + 6, "[ ]");
		apx += 3;
	}

	apx = x + 4;
	for (int n = 0; n < player->cur_ap; n++)
	{
		cl->outchars(apx, y + 6, "X");
		apx += 3;
	}
	}
	else
	{
		// display numerical gauge
		cl->outcharsf(x,y+6, "AP %3d/%3d", player->cur_ap, player->max_ap);
	}

}



//######################################################################
//## BATTLE SYSTEM
//######################################################################
clibwindow* main_wnd	= 0;
clibwindow* msg_wnd		= 0;
clibwindow* scene_wnd	= 0;
clibwindow* player_wnd	= 0;

int attacks				= 0;
int active_option		= BTLMNU_ATTACK;
int battle_result		= BATTLE_RESULT_LOSE;
bool battle_finished	= false;

int calc_atk_damage(void* a, void* b)
{
	obj* p = (obj*)a;obj* e = (obj*)b;
	int dmg = 0;
	int l_atk = p->level * (p->attack + p->strength);
	int l_def = e->level * (e->defense + e->strength);
	dmg = l_def - l_atk;
	int r1 = rand()%5;int r2 = rand()%5;int r3 = rand()%5;
	dmg +=-10+r1+r2-r3+rand()%(10- -10);
	if (dmg < 0) dmg = -dmg;
	return dmg;
}

void handle_player_turn(void* d)
{
	obj* e = (obj*)d;

	// the player turn loop
	bool close_box = false;
	while(!close_box)
	{
		int k = cl->get_key();

		switch (k)
		{
		case VK_UP:
			{
				active_option = BTLMNU_ITEM;
				draw_battle_menu_widget(main_wnd->data_[0] + 2, main_wnd->data_[1] + 24, active_option);
			} break;
		case VK_DOWN:
			{
				active_option = BTLMNU_CAST;
				draw_battle_menu_widget(main_wnd->data_[0] + 2, main_wnd->data_[1] + 24, active_option);
			} break;
		case VK_LEFT:
			{
				active_option = BTLMNU_ATTACK;
				draw_battle_menu_widget(main_wnd->data_[0] + 2, main_wnd->data_[1] + 24, active_option);
			} break;
		case VK_RIGHT:
			{
				active_option = BTLMNU_DEFEND;
				draw_battle_menu_widget(main_wnd->data_[0] + 2, main_wnd->data_[1] + 24, active_option);
			} break;

		case VK_BACK:
		case VK_ESCAPE:
			{
				if (!boss_battle)
				{
					if (rand()%100<=25)
					{
						cl->show_message_centered(3, "Escape Succeeded.", "", "You ran away from the enemy!");
						close_box = true;
						battle_result = BATTLE_RESULT_LOSE;
						battle_finished = true;
					}
					else
					{
						cl->show_message_centered(3, "Escape Failed.", "", "You were too slow!");
						close_box = true;
					}
				}
				else
				{
					cl->show_message_centered(3,"","You cannot escape from this battle!","");
				}
			} break;

		case VK_RETURN:
		case VK_SPACE:
			{
				switch(active_option)
				{
				case BTLMNU_ATTACK:
					{
						if (player->cur_ap > 1)
						{
							player->cur_ap--;
							attacks++;
							draw_battle_player_widget(player_wnd->data_[0] + 2, player_wnd->data_[1] + 2);

							if (player->cur_ap == 1)
							{
								int dmg = calc_atk_damage(player, d);
								dmg *= attacks;
								e->cur_hp -= dmg;

								char atkbuf[40];char atkbuf2[40];
								sprintf_s(atkbuf, 64, "You brutally attack the %s", e->name);
								sprintf_s(atkbuf2, 64, "%d times for %d damage!", attacks, dmg);
								cl->show_message_centered(2, atkbuf, atkbuf2);

								
								close_box = true;
								attacks = 0;
							}
						}
						else if (player->cur_ap == 1)
						{
							int dmg = calc_atk_damage(player, d);
							e->cur_hp -= dmg;
							char atkbuf[64];
							sprintf_s(atkbuf, 64, "You attack the %s for %d damage!", e->name, dmg);
							cl->show_message_centered(1, atkbuf);
							close_box = true;
							attacks = 0;
						}

					} break;

				case BTLMNU_DEFEND:
					{
						cl->show_message_centered(1, "You Defend.");
						if (player->cur_ap < player->max_ap)
						{
							cl->show_message_centered(1, "You gained an Action Point!");
							player->cur_ap++;
							draw_battle_player_widget(player_wnd->data_[0] + 2, player_wnd->data_[1] + 2);
						}						
					} break;

				case BTLMNU_ITEM:
					{
						// cl->show_message(1, "Items in battle not implemented.");
						battle_engine_b(d);
					} break;

				case BTLMNU_CAST:
					{
						if (player->chr_class!=3)
						{
							cl->show_message_centered(3,"","You don't know how to use magic!","");
						}
						else
						{
							battle_engine_c(d);
						}
						// cl->show_message(1, "Casting in battle not implemented.");
					} break;
				} // end switch case selected battle menu item
			} break; // end key is enter or space

		} // end switch case getkey
	} // end the player turn loop

}

void handle_enemy_turn(void* d)
{
	obj* e = (obj*)d;

	int dmg			= calc_atk_damage(player, d);
	int crit_dmg	= (dmg * (2 + rand() % 5 - 2));
	char m1[255]; 
	// new enemy AI

	// if my current hp is > 50% (max hp) then I'll attack
	// otherwise I'll defend or try a critical attack

	if (e->cur_hp >= (e->max_hp / 2))
	{
		// I'm attacking but I might miss or I may get bored
		int r = rand() % 100;
		if (r<=75)
		{
			// HAH! I hit yo ass!
			sprintf_s(m1, 255, "The %s attacks you for %d damage!",e->name,dmg);
			player->cur_hp -= dmg;
		}
		else if (r>75&&r<=98)
		{
			// whoops! I'm not too accurate
			sprintf_s(m1, 255, "The %s attacks you and misses!", e->name);
		}
		else if (r>98)
		{
			// ahh boredom..going to do nothing
			int r2=rand()%8;
			switch(r2)
			{
			case 0:
				sprintf_s(m1, 255, "The %s looks bored...", e->name);
				break;
			case 1:
				sprintf_s(m1, 255, "The %s is not paying attention...", e->name);
				break;
			case 2:
				sprintf_s(m1, 255, "The %s is singing???", e->name);
				e->defense--;
				break;
			case 3:
				sprintf_s(m1, 255, "The %s does not seem to want to fight...", e->name);
				break;
			default:
				sprintf_s(m1, 255, "The %s does nothing...", e->name);
				break;
			}
		}
	}
	else
	{
		// If my current hp is less than 1/4 of my max hp
		// then I'm going to try a critical attack
		// otherwise I'll defend
		if (e->cur_hp <= (e->max_hp/4))
		{
			cl->show_message_centered(3,"","CRITICAL ATTACK!!!","");
			// try a critical
			// 1 in 20 chance of critical attack connecting
			if (rand()%20==5)
			{
				// critical success
				int r=rand()%8;
				switch(r)
				{
				case 0:
					sprintf_s(m1, 255, "The %s catches you off-guard! You take %d damage", e->name, crit_dmg);
					break;
				case 1:
					sprintf_s(m1, 255, "The %s mauls you for %d damage", e->name, crit_dmg);
					break;
				case 2:
					sprintf_s(m1, 255, "The %s charges you! You suffer %d damage", e->name, crit_dmg);
					break;
				default:
					sprintf_s(m1, 255, "The %s sneaks behind you! You take %d damage", e->name, crit_dmg);
					break;
				}	

				player->cur_hp -= crit_dmg;
			}
			else
			{
				// critical failed
				sprintf_s(m1, 255, "The %s sneaks behind you! You block the attack!", e->name);
				e->defense--;
			}
		}
		else
		{
			// defend
			int r = rand()%8;
			switch(r)
			{
			case 0: 
				sprintf_s(m1, 255, "The %s defends!", e->name);
				break;
			case 1: 
				sprintf_s(m1, 255, "The %s is blocking attacks!", e->name);
				break;
			case 2: 
				sprintf_s(m1, 255, "Careful! The %s is defending!", e->name);
				break;
			case 3: 
				sprintf_s(m1, 255, "The %s takes a defensive stance!", e->name);
				break;
			case 4: 
				sprintf_s(m1, 255, "Not good! The %s is glowing!", e->name);
				e->attack++;
				e->strength++;
				break;
			default:
				sprintf_s(m1, 255, "The %s takes a defensive stance!", e->name);
				break;
			}
			
			e->defense++;
		}
	}

	cl->show_message_centered(3, "",m1,"");

	if (player->cur_hp < 0) player->cur_hp = 0;
	draw_battle_player_widget(player_wnd->data_[0] + 2, player_wnd->data_[1] + 2);

}

void old_handle_enemy_turn(void* d)
{
	obj* e = (obj*)d;

	int e_action = rand()%100;
	if (e_action<=25)
	{
		// attack
		int dmg = calc_atk_damage(player, d);
		char m1[255];
		if ((rand()%100)<=35)
		{
			sprintf_s(m1, 255, "The %s attacks you for %d damage!",e->name,dmg);
			player->cur_hp -= dmg;
		}
		else
		{
			sprintf_s(m1, 255, "The %s attacks you and misses!", e->name);
		}
		cl->show_message(1, m1);
	}
	else if (e_action > 25 && e_action<=50)
	{
		// defend
		char m1[255];
		if ((rand()%100)<=35)
		{
			sprintf_s(m1, 255, "The %s takes a defensive stance!", e->name);
			e->defense++;
		}
		else
		{
			sprintf_s(m1, 255, "The %s does nothing...", e->name);
		}
		cl->show_message(1, m1);
	}
	else
	{
		char m1[255];
		sprintf_s(m1, 255, "The %s is waiting...", e->name);
		cl->show_message(1, m1);
	}

	if (player->cur_hp < 0) player->cur_hp = 0;
	draw_battle_player_widget(player_wnd->data_[0] + 2, player_wnd->data_[1] + 2);
}



// main battle screen
int battle_engine_d(void* d)
{
	cl->screen80x50();
	battle_result = BATTLE_RESULT_LOSE;
	battle_finished = false;
	{char window_title[255];sprintf_s(window_title,255,"SAWD [IN BATTLE] - %s", 
	GAME_BUILD_NUMBER); SetConsoleTitle(window_title);}
	cl->set_bgcolor(0);cl->set_fgcolor(0);cl->cls();

	// create the battle windows
	cl->set_bgcolor(0);cl->set_fgcolor(0);cl->cls();
	cl->set_bgcolor(0);cl->set_fgcolor(0);cl->cls();

	// main window (64x40)
	main_wnd = cl->open_window(8, 5, 64, 40, 1);
	
	// msg window (60x5)
	msg_wnd = cl->open_window(10, 7, 60, 5, 1);

	// scene window (60x15)
	scene_wnd = cl->open_window(10, 12, 60, 17, 1);

	// player widget window (37x8)
	player_wnd = cl->open_window(39, 29, 31, 14, 1);

	// get a pointer to the enemy
	obj* e = (obj*)d;

	// write the message on the message window
	char* msg = 0;
	char c = e->name[0];
	if (c=='A'||c=='E'||c=='I'||c=='O'||c=='U')
	{
		msg = "You are being attacked by an";
	}
	else
	{
		msg = "You are being attacked by a";
	}

	char message_buffer[80];
	sprintf_s(message_buffer, 80, "%s %s!", msg, e->name);

	cl->outchars(40 - strlen(message_buffer) / 2, 9, message_buffer);

	

	
	// draw the battle-menu widget
	
	draw_battle_menu_widget(main_wnd->data_[0] + 2, main_wnd->data_[1] + 24, active_option);

	// draw the player widget
	draw_battle_player_widget(player_wnd->data_[0] + 2, player_wnd->data_[1] + 2);



	// draw the enemy icon
	{
		{
			int icon_w = e->battle_icon_w;
			int icon_h = e->battle_icon_h;
			const char mask = '@';
			unsigned short fgc = 0, bgc = 1|2|4;
			int x = 40 - (icon_w / 2);
			int y = scene_wnd->data_[1] + 1;

			for (int py = 0; py < icon_h; py++)
			{
				for (int px = 0; px < icon_w; px++)
				{
					if (e->battle_icon[px + py * icon_w] != mask)
					{
						cl->outchar(x + px, y + py, e->battle_icon[px + py * icon_w]);
					}
				}
			}
		}
	}


	
	int turn = rand() % 2;

	if (turn != 0) 
	{
		cl->show_message_centered(3, "","The enemy has surprised you!","");
	}

	while(!battle_finished)
	{
		if (turn == 0)
		{
			// player
			if (player->cur_hp>0)
				handle_player_turn(d);
			// enemy
			if (e->cur_hp > 0)
				handle_enemy_turn(d);
		}
		else
		{
			// enemy
			if (e->cur_hp > 0)
				handle_enemy_turn(d);
			// player
			if (player->cur_hp>0)
				handle_player_turn(d);
		} // end turns

		if (player->cur_hp <= 0)
		{
			battle_finished = true;
			battle_result = BATTLE_RESULT_LOSE;
		}
		else
		{
			if (e->cur_hp <= 0)
			{
				cl->show_message_centered(3, "","You were victorious!","");

				end_battle(e);

				battle_finished = true;
				battle_result = BATTLE_RESULT_WIN;
			}
		} // end hp checks

	} // end main battle loop


	// close the battle windows
	cl->close_window(player_wnd);
	cl->close_window(scene_wnd);
	cl->close_window(msg_wnd);
	cl->close_window(main_wnd);


	// reset everything..LOL was an instant win after first battle hahaha!
	{char window_title[255];sprintf_s(window_title,255,"SAWD - %s", 
	GAME_BUILD_NUMBER); SetConsoleTitle(window_title);}
	attacks				= 0;
	active_option		= BTLMNU_ATTACK;
	battle_finished		= false;
	
	cl->screen80x25();

	return battle_result;
}

unsigned long xp_to_level_up()
{
	unsigned long l = player->level + 1;
	return (unsigned long)(10 * l + 5 * (l * l) + 10 * (l * l * l) + 80);
}

void level_up()
{
	if (player->level==100)return; // maximum level

	cl->show_message_centered(3, "", "Gained an Experience Level!", "");
	player->level++;
	player->max_hp += 10 + player->level;
	if (player->level%3==0)
	{
		cl->show_message_centered(3,"","Gained an Action Point Slot!","");
		player->max_ap++;
	}

	player->attack		+= 2;
	player->defense		+= 2;
	player->strength	+= 2;
	
	if (player->chr_class==3)
	{
		player->max_mp	+= 4 + (3 * player->level);
		player->magic++;
	}
	
	player->cur_hp = player->max_hp;
	player->cur_mp = player->max_mp;
}

// ### 
// a nice end of battle screen with 'animated' level up
// ###
void draw_player_stats_end_battle(int x, int y_)
{
	int y = y_;
	
	cl->outcharsf(x, y, "%s", player->name);y++;y++;
	cl->outcharsf(x, y, "HP: %4d / %4d", player->cur_hp, player->max_hp);y++;y++;
	cl->outcharsf(x, y, "MP: %4d / %4d", player->cur_mp, player->max_mp);y++;y++;
	cl->outcharsf(x, y, "Level: %3d ", player->level);y++;y++;
	cl->outcharsf(x, y, "ATK: %3d   DEF: %3d   STR: %3d   MAG: %3d", player->attack, player->defense,player->strength, player->magic);y++;y++;
	
	cl->outcharsf(x, y + 6, "Action Point Slots: %d", player->max_ap);
	int apx = x;
	for (int n = 0; n < player->max_ap; n++)
	{
		cl->outchars(apx, y + 7, "[ ]");
		apx += 3;
		if (n % 8 == 0 && n != 0)
		{
			y++;
			apx = x;
		}
	}
}

void display_items_gained(void* d)
{
	obj* e = (obj*)d;

	int box[] = {20, 4, 40, 16};
	int x = box[0] + 1;
	int y = box[1] + 1;

	clibwindow* dig_wnd = cl->open_window(box[0], box[1], box[2], box[3],1|2|4,0);

	for (unsigned int i = 0; i < e->items.size(); i++)
	{
		cl->set_fgcolor(8);
		cl->outcharsf(x + 1, (y + i), "%3d", i+1);
		cl->set_fgcolor(0);
		cl->outchars(x + 5, (y + i), "                           ");
		cl->outchars(x + 5, (y + i), e->items.at(i)->name);
	}

	while(!cl->get_key()){}
	cl->close_window(dig_wnd);
}

void end_battle(void*d)
{
	{char window_title[255];sprintf_s(window_title,255,"SAWD [SPOILS OF WAR] - %s", 
	GAME_BUILD_NUMBER); SetConsoleTitle(window_title);}

	clibwindow* black_panel = cl->open_window_noframe(0,0,79,49,0,0);
	// the enemy that was defeated is passed to this function
	obj* e = (obj*)d;

	int box[] = {16, 5, 48, 30};
	int x = box[0] + 2;
	int y = box[1] + 2;

	clibwindow* upg_wnd = cl->open_window(box[0], box[1], box[2], box[3]);

	unsigned long xp_gained		= e->experience;
	unsigned long nxt_xp		= xp_to_level_up();
	unsigned long gold_gained	= e->gold;

	cl->outcharsf(x, 32, "$%8d", player->gold);
	draw_player_stats_end_battle(x, y);
	// update the experience
	bool next_upg = false; unsigned int counter = 0;
	while(!next_upg)
	{
		cl->outcharsf(x, y, "Battle Experience: %8d / %8d", player->experience, nxt_xp);
		player->experience++;
		if (player->experience >= nxt_xp)
		{
			level_up();
			nxt_xp = xp_to_level_up();
			draw_player_stats_end_battle(x, y); // update stats
		}

		counter++;
		if (counter >= xp_gained)
		{
			next_upg = true;
		}

		unsigned long start_time = timeGetTime();
		while((timeGetTime() - start_time) < 60);
	}

	// update the gold
	next_upg = false;
	counter = 0;
	while(!next_upg)
	{
		cl->outcharsf(x, 32, "$%8d", player->gold);
		player->gold++;
		counter++;
		if (counter >= gold_gained)
		{
			next_upg = true;
		}

		unsigned long start_time = timeGetTime();
		while((timeGetTime() - start_time) < 60);
	}

	// show items gained if any
	if (e->items.size()>0)
	{
		display_items_gained(e);

		for (unsigned int i = 0; i < e->items.size(); i++)
		{
			player->items.push_back(get_master_item(e->items.at(i)->name));
		}
	}

	cl->show_message_centered(3,"","Another Victory!","");
	cl->close_window(upg_wnd);
	cl->close_window(black_panel);
}
