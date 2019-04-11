
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
#include <cstdarg>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "clib.h"
#include "maps.h"
#include "mapevents.h"

#include "mymidi.h"
#include "sawd.h"

// satisfy externs for story flags
bool entered_thieves_hut = false;
bool have_treasure_room_key = false;
bool entered_treasure_room = false;
bool accepted_beer_quest = false;
bool have_found_beer = false;
bool entered_karns_pub = false;
bool beggars_bar_open = false;

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

// DUSHAL EVENTS
void event_dushal_player_home(void*d)
{
	cl->show_message(1, "You walk to your room...");
	
	draw_game_display();
	int yesno = cl->show_choice("Take a Rest?");
	if (yesno == 1) // yes
	{
		cl->set_bgcolor(0);cl->set_fgcolor(1|2|4|8);cl->cls();

		//midi_manager->GetMidi(mapbgm[current_map])->Pause();
		//midi_manager->GetMidi(rest_bgm)->Play(false);
		/*int tick = 0;
		while(midi_manager->GetMidi(rest_bgm)->IsPlaying())
		{
			cl->outcharsf(1, 1, "Midi Playing: (%d) %s",midi_manager->GetMidi(rest_bgm)->GetID(), midi_manager->GetMidi(rest_bgm)->GetAlias());
			cl->outcharsf(1,4,"%d", tick);
			tick++;
		}
		*/

		// rest
		player->cur_hp = player->max_hp;
		player->cur_mp = player->max_mp;
		
		cl->show_message(1, "HP / MP Restored!");
		//midi_manager->GetMidi(mapbgm[current_map])->Resume();

		// save game
	}
	// shove player to the right of his home
	player->x++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_dushal_inn(void*d)
{
	if (player->cur_hp <= player->max_hp / 2)
	{
		cl->show_message(4,"Inn Keeper:",
			"Welcome to Dushal Inn.", 
			"",
			"Oh! You look exhausted my dear!");
	}
	else
	{
		cl->show_message(2, "Inn Keeper:",
			"Welcome to Dushal Inn.");
	}
	draw_shop_ext();
	int yesno = cl->show_choice("Take a Rest for $20.00 ?");
	if (yesno == 1) // yes
	{
		if (player->gold >= 20)
		{
			player->gold -= 20;
			draw_shop_ext();
			// rest
			player->cur_hp = player->max_hp;
			player->cur_mp = player->max_mp;	
			cl->show_message(1, "HP / MP Restored!");
			cl->show_message(2, "Inn Keeper:",
				"I trust you slept well?");
		}
		else
		{
			cl->show_message(2,"Inn Keeper:", 
				"I'm sorry you don't have enough money!");
		}
		// save game
	}
	hide_shop_ext();
	// shove player north of the inn
	player->y--;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_dushal_pub(void* d)
{
	if (!have_found_beer)
	{
		cl->show_message(1,"You read the Sign on the door");
		cl->show_message(3,"BEGGAR'S BAR",
			"",
			"CLOSED");
		cl->show_message(3, 
			player->name, 
			"How odd...",
			"This ole bar never closes?...");
	}
	else
	{
		if (!beggars_bar_open)
		{
		beggars_bar_open = true;
		cl->show_message(1,"You read the Sign on the door");
		cl->show_message(3,"BEGGAR'S BAR",
			"",
			"NOW OPEN");
		cl->show_message(3, 
			player->name, 
			"Awesome!",
			"I guess ole Karn made his way down here.");
		}
		else
		{
			cl->show_message(1,"You read the Sign on the door");
		cl->show_message(3,"BEGGAR'S BAR",
			"",
			"NOW OPEN");
		}
		
		if (player->drunk)
		{
			cl->show_message(2,"Bar Tender:","A little too drunk! Come back when you're sober!");
			player->y++;
			return;
		}

		shopkeeper* keeper = new shopkeeper;
		keeper->is_magic_shop = false;
		keeper->name = "BEGGAR'S BAR";

		keeper->items.push_back(get_master_item("Pint 'O Beer"));
		keeper->items.push_back(get_master_item("Bottled Draught"));
		keeper->items.push_back(get_master_item("Hard Liquor"));

		int o = shop_form_a(keeper);
		if (o == SHOP_OPTION_SELL)
		{
			cl->show_message(2,"Bar Tender:",
				"I'm not interested in buying anything!");
		}
		else if (o == SHOP_OPTION_BUY)
		{
			int result = shop_form_b(keeper);
			if (result == SHOP_OPTION_CANCEL)
			{
				cl->show_message(2,"Bar Tender:","What? You don't drink?");
			}
			else if (result == SHOP_OPTION_ACCEPT)
			{
				cl->show_message(2, "Bar Tender:","Thank you! Come again!");
			}
		}

		delete keeper;
	}
	// shove player south of the pub
	player->y++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_dushal_magic_shop(void*d)
{
	/*
	Learn Burn		$100
	Learn Break		$200
	*/

	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = true;
	keeper->name = "DUSHAL MAGIC SHOP";

	keeper->spells.push_back(get_master_magic("Burn"));
	keeper->spells.push_back(get_master_magic("Break"));
	
	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}
	
	delete keeper;

	// shove player south of the shop
	player->y++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_dushal_armor_shop(void*d)
{
	/*
	Leather Vest		$30		defense + 4
	Leather Boots		$40		defense + 5
	Leather Gauntlets	$40		defense + 4
	*/

	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = false;
	keeper->name = "DUSHAL ARMOR SHOP";

	keeper->items.push_back(get_master_item("Leather Vest"));
	keeper->items.push_back(get_master_item("Leather Boots"));
	keeper->items.push_back(get_master_item("Leather Gauntlets"));

	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}

	delete keeper;

	// shove player north of the shop
	player->y--;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_dushal_weapon_shop(void*d)
{
	/*
	Short Sword			$100 (warrior only)
	Heavy Axe			$100 (dwarf only)
	Wooden Bow			$100 (thief only)
	Wooden Staff		$100 (mage only)
	*/
	
	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = false;
	keeper->name = "DUSHAL WEAPON SHOP";

	keeper->items.push_back(get_master_item("Short Sword"));
	keeper->items.push_back(get_master_item("Heavy Axe"));
	keeper->items.push_back(get_master_item("Wooden Bow"));
	keeper->items.push_back(get_master_item("Wooden Staff"));

	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}

	delete keeper;

	// shove player north of the shop
	player->y--;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_dushal_item_shop(void*d)
{
	/*
	Healing Powder		$15
	Escape Roots		$800
	Burn Scroll			$50
	Break Scroll		$80
	*/
	
	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = false;
	keeper->name = "DUSHAL ITEM SHOP";

	keeper->items.push_back(get_master_item("Healing Powder"));
	keeper->items.push_back(get_master_item("Escape Roots"));
	keeper->items.push_back(get_master_item("Burn Scroll"));
	keeper->items.push_back(get_master_item("Break Scroll"));

	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}

	delete keeper;

	// shove player south of the shop
	player->y++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////
bool boss_battle = false;
// KIRAR EVENTS
void event_kirar_thieves_hut(void* d)
{
	cl->show_message(2, "You walk up to the rickety old shack",
		"and push the door open.");
	if (!entered_thieves_hut) // first time
	{
		entered_thieves_hut = true;
		cl->show_message(3,"Thief:","Hey you!",
			"What the hell do you think you're doing here!?");
		cl->show_message(3,"Thief:","You just walked into a whole lot of trouble!",
			"Get em boys!");
		int battle_index = 1;
		while (battle_index < 11)
		{
			// spawn a battle against a Thief with a level 8*(battle_index/5)
			// NAME				HP/MP/GOLD/XP		ATK/DFN/STR
			obj* thief;
			if (battle_index == 10)
			{
				thief = create_enemy("Master Thief", (battle_index / 3) * 8, 
					2400, 
					0, 
					125000, 
					35000, 
					18, 
					15, 
					8);
			}
			else
			{
				int bi3 = battle_index / 3;
				thief = create_enemy("Thief", bi3 * 8, 
					bi3 * 65, 
					0, 
					750, 
					900, 
					4 * bi3, 
					4 * bi3, 
					4 * bi3);
			}

			thief->battle_icon_w = 13;
			thief->battle_icon_h = 11;
			thief->battle_icon = "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@|\\@@@@@@@@@@@|@\\@@@@@@@@/\\|@@b@@@@@--\\/|@@T@@@@@@\\R=--mB@@@@@@@qp|@/@@@@@@@/@@|/@@@@@@@y@@@@\\@@@@@@@~~@@@~~@@@";

			boss_battle = true;
			int battle_result = battle_engine_d(thief);
			if (battle_result == BATTLE_RESULT_WIN)
			{
				// you won or ran away!
			}
			else if (battle_result == BATTLE_RESULT_LOSE)
			{
				battle_index = 9999;
			}
			battle_index++;
		}
		boss_battle = false;
		cl->show_message(3,"The thieves lay at your feet broken and bleeding.",
			"They are not going to get up. Most are dead.",
			"Those that still draw breath will die as well.");

		// side quest
		if (accepted_beer_quest)
		{
			cl->show_message(2,player->name,"What is this?...");
			cl->show_message(1, "Found KARN'S BEER!");
			have_found_beer = true;
		}
	}
	else
	{
		cl->show_message(3,"There is nothing of interest here..",
			"just a bunch of corpses.",
			"You should know that. You kicked their asses!");
	}
	// shove player east of the hut
	player->x++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_treasure_room(void* d)
{
	if (have_treasure_room_key)
	{
		if (!entered_treasure_room)
		{
			cl->show_message(2, "You unlock the door with Karn's Key",
				"and push open the door...");
			entered_treasure_room = true;
			cl->show_message(1,"You have found the treasure room!");
			cl->show_message(1,"You fill your pockets with all you can carry.");
			cl->show_message(1,"You gained 100,000,000 gold!");
			player->gold += 100000000;
		}
		else
		{
			cl->show_message(2, "You walk up to the small building",
				"and notice the door is wide open!");
			cl->show_message(3,player->name, 
				"Damnit!",
				"I guess I didn't lock the door..");
			cl->show_message(2,player->name,
				"Someone cleaned the place out!");
		}
	}
	else
	{
		cl->show_message(2,"The door is locked...",
			"there must be a key somewhere.");
	}
	// shove player east of the room
	player->x++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_cave_entrance(void* d)
{
	cl->show_message(1,"Before you is a dark cave entrance.");
	cl->show_message(1,"If you continue the demo will end.");
	cl->show_message(2,"If you choose not to continue,",
		"you will return to the world map.");
	int yesno = cl->show_choice("Continue?");
	if (yesno == 1)
	{
		int sidequests_complete = 0;
		if (entered_treasure_room)sidequests_complete++;
		if (beggars_bar_open)sidequests_complete++;

		char sqmsg[80];
		sprintf_s(sqmsg,80,"%d / 2", sidequests_complete);
		if (sidequests_complete<2)
			cl->show_message_centered(4,"SIDE QUESTS COMPLETED:","",sqmsg,"Better luck next time!");
		else
			cl->show_message_centered(4,"SIDE QUESTS COMPLETED:","",sqmsg,"Very nice!");

		
		cl->show_message(3,"Thank you for playing!","",
			"END OF THE SAWD GAME DEMO.");
		gamestate = gsmain_menu;
		return;
	}
	else
	{
		change_map(MAPID_WORLD_MAP);
		player->x = 22;
		player->y = 3;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_inn(void*d)
{
	cl->show_message(1, "You walk into KIRAR INN.");
	if (player->cur_hp <= player->max_hp / 2)
	{
		cl->show_message(4,"Inn Keeper:",
			"Welcome to Kirar Inn.", 
			"",
			"Lord! You look like you've seen quite a few battles!");
	}
	else
	{
		cl->show_message(2, "Inn Keeper:",
			"Welcome to Kirar Inn.");
	}
	draw_shop_ext();
	int yesno = cl->show_choice("Take a Rest for $50.00 ?");
	if (yesno == 1) // yes
	{
		if (player->gold >= 50)
		{
			player->gold -= 50;
			draw_shop_ext();
			// rest
			player->cur_hp = player->max_hp;
			player->cur_mp = player->max_mp;	
			cl->show_message(1, "HP / MP Restored!");
			cl->show_message(2, "Inn Keeper:",
				"I take it you enjoyed your rest?");
		}
		else
		{
			cl->show_message(2,"Inn Keeper:", 
				"You don't have enough money!");
		}
		// save game
	}
	hide_shop_ext();
	// shove player east of the inn
	player->x++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_pub(void*d)
{
	cl->show_message(1, "You walk into KARN'S PUB.");

	if (!accepted_beer_quest)
	{
		// we have not accepted the beer quest
		if (!entered_karns_pub)
		{
			entered_karns_pub = true;
			cl->show_message(3,"Old man:", 
				"HEY!",
				"You there!");
			cl->show_message(2,player->name,
				"Me?");
			cl->show_message(2,"Old man:", 
				"Yeah you- Get over ere!");
			cl->show_message(4,"Old man:", 
				"Names Karn.",
				"You look like a fighter.",
				"I need to ask you a favor.");
			cl->show_message(4,"Karn:",
				"Some bloke stole me beer an I need to",
				"get it back or my business will surely",
				"be done for.");
		}
		else // been here before
		{
			cl->show_message(2,"Karn:","Changed your mind eh?");
		}

		int yesno = cl->show_choice("Would you get me beer?");
		if (yesno == 1)
		{
			// accept quest
			cl->show_message(3,"Karn:",
				"Oi! The wanker who done stole me beer ran off",
				"to the north of here!");
			cl->show_message(2,"Karn:","Heres $1000. Go get me beer!!");
			cl->show_message(1,"You gained 1,000 gold!");
			player->gold += 1000;
			accepted_beer_quest = true;
		}
		else
		{
			cl->show_message(2,"Karn:", "GET OUT OF HERE!");
			// deny quest
		}
	}
	else
	{
		// accepted beer quest
		if (!have_found_beer)
		{
			cl->show_message(3,"Karn:",
				"Still haven't found me beer?",
				"You're not looking hard enough!");
		}
		else
		{
			// found beer
			if (!have_treasure_room_key)
			{
				cl->show_message(3,"Karn:",
					"Aye tis me beer! Thank ye!",
					"Take this KEY.");
				have_treasure_room_key = true;
			}
			else
			{
				cl->show_message(2,"Karn:","I'm busy now me boy.");
			}
		}
	}
	// shove player west of the pub
	player->x--;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_magic_shop(void*d)
{
	/*
	Learn Inferno		$1000
	Learn Shatter		$2000
	Learn Drain			$3000
	*/

	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = true;
	keeper->name = "KIRAR MAGIC SHOP";
	
	keeper->spells.push_back(get_master_magic("Inferno"));
	keeper->spells.push_back(get_master_magic("Shatter"));
	keeper->spells.push_back(get_master_magic("Drain"));

	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}

	delete keeper;

	// shove player south of the shop
	player->y++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_armor_shop(void*d)
{
	/*
	Silver Vest			$250
	Silver Boots		$150
	Silver Gauntlets	$350
	*/
	
	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = false;
	keeper->name = "KIRAR ARMOR SHOP";

	keeper->items.push_back(get_master_item("Silver Vest"));
	keeper->items.push_back(get_master_item("Silver Boots"));
	keeper->items.push_back(get_master_item("Silver Gauntlets"));
	
	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}

	delete keeper;
	
	// shove player west of the shop
	player->x--;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_weapon_shop(void*d)
{
	/*
	Silver Sword		$500 (warrior only)
	Silver Axe			$500 (dwarf only)
	Silver Bow			$500 (thief only)
	Silver Staff		$500 (mage only)
	*/
	
	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = false;
	keeper->name = "KIRAR WEAPON SHOP";
	
	keeper->items.push_back(get_master_item("Silver Sword"));
	keeper->items.push_back(get_master_item("Silver Axe"));
	keeper->items.push_back(get_master_item("Silver Bow"));
	keeper->items.push_back(get_master_item("Silver Staff"));


	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}

	delete keeper;

	// shove player south of the shop
	player->y++;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void event_kirar_item_shop(void*d)
{
	/*
	Healing Powder		$25
	Anti-Venom			$50
	Escape Roots		$1500
	Inferno Scroll		$250
	Shatter Scroll		$350
	Drain Scroll		$450
	*/
	
	shopkeeper* keeper = new shopkeeper;
	keeper->is_magic_shop = false;
	keeper->name = "KIRAR ITEM SHOP";

	keeper->items.push_back(get_master_item("Healing Powder"));
	keeper->items.push_back(get_master_item("Anti-Venom"));
	keeper->items.push_back(get_master_item("Escape Roots"));
	keeper->items.push_back(get_master_item("Inferno Scroll"));
	keeper->items.push_back(get_master_item("Shatter Scroll"));
	keeper->items.push_back(get_master_item("Drain Scroll"));
	
	int o = shop_form_a(keeper);
	if (o == SHOP_OPTION_SELL)
	{
		cl->show_message(4,"Richard Marks:",
			"Sorry, selling did not make it into the",
			"game in time for the contest. If you like",
			"this game, tell me and I'll finish it.");
	}
	else if (o == SHOP_OPTION_BUY)
	{
		int result = shop_form_b(keeper);
		if (result == SHOP_OPTION_CANCEL)
		{
			cl->show_message(2,"Shop Keeper:","Changed your mind?");
		}
		else if (result == SHOP_OPTION_ACCEPT)
		{
			cl->show_message(2, "Shop Keeper:","Thanks for your business!");
		}
	}

	delete keeper;

	// shove player west of the shop
	player->x--;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

// GLOBAL EVENTS
void warp_to(void* d)
{
	warptarget* t = (warptarget*)d;

	change_map(t->map_id);
	player->x = t->x;
	player->y = t->y;
}

// end event functions
