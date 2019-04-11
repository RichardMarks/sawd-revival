
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
#ifndef SAWD_H
#define SAWD_H

#include <vector>

#define GAME_BUILD_NUMBER "build 3.17.2008"
const int SHOP_OPTION_EXIT		= 0;
const int SHOP_OPTION_BUY		= 1;
const int SHOP_OPTION_SELL		= 2;
const int SHOP_OPTION_ACCEPT	= 3;
const int SHOP_OPTION_CANCEL	= 4;

const int BATTLE_OPTION_FIGHT	= 5;
const int BATTLE_OPTION_RUN		= 6;
const int BATTLE_OPTION_ACCEPT	= 7;
const int BATTLE_OPTION_CANCEL	= 8;

const int BATTLE_RESULT_WIN		= 9;
const int BATTLE_RESULT_LOSE	= 10;

const int ITEM_MODIFIER_HP			= 0;
const int ITEM_MODIFIER_MP			= 1;
const int ITEM_MODIFIER_ATTACK		= 2;
const int ITEM_MODIFIER_DEFENSE		= 3;
const int ITEM_MODIFIER_STRENGTH	= 4;
const int ITEM_MODIFIER_MAGIC		= 5;
const int ITEM_MODIFIER_MAX_HP		= 6;
const int ITEM_MODIFIER_MAX_MP		= 7;
const int ITEM_MODIFIER_AP			= 8;
const int ITEM_MODIFIER_MAX_AP		= 9;

const int SPELL_MODIFIER_HP			= 0;
const int SPELL_MODIFIER_MP			= 1;
const int SPELL_MODIFIER_ATTACK		= 2;
const int SPELL_MODIFIER_DEFENSE	= 3;
const int SPELL_MODIFIER_STRENGTH	= 4;
const int SPELL_MODIFIER_MAGIC		= 5;

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

class item_target
{
public:
	item_target(void* self, void* target);
	~item_target();
	void* GetSelf();
	void* GetTarget();
private:
	void* self_;
	void* target_;
};

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

struct shop_item
{
	char* name;
	unsigned int cost;
	bool can_equip;
	int chr_class;
	void (*use_func)(void*);
	void (*drop_func)(void*);
	void (*equip_func)(void*);
	void (*unequip_func)(void*);
	int modifiers[10];
};

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

struct shop_spell
{
	char* name;
	unsigned int cost;
	int mp_cost;
	void (*cast_func)(void*);
	int modifiers[6];
};

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

struct shopkeeper
{
	char* name;
	bool is_magic_shop;
	std::vector<shop_item*> items;
	std::vector<shop_spell*> spells;
};

const unsigned int OUT_OF_RANGE_ID = 65535;
typedef unsigned int ITEM_ID;
typedef unsigned int MAGIC_ID;
extern std::vector<shop_item*> master_item_list;
extern std::vector<shop_spell*> master_magic_list;
extern void create_master_item_list();
extern void create_master_magic_list();
extern void add_master_item(shop_item* item);
extern void add_master_magic(shop_spell* spell);
extern ITEM_ID get_item_id(char* name);
extern MAGIC_ID get_magic_id(char* name);
extern void destroy_master_item_list();
extern void destroy_master_magic_list();
extern shop_item* get_master_item(char* name);
extern shop_spell* get_master_magic(char* name);
extern shop_item* create_item(char* name, unsigned int cost, void(*use_func)(void*), void(*drop_func)(void*), int* modifiers);
extern shop_item* create_equipment(int chrclass, char* name, unsigned int cost, void(*equip_func)(void*), void(*unequip_func)(void*), int* modifiers);
extern shop_spell* create_spell(char* name, unsigned int cost, int mp_cost, void(*cast_func)(void*), int* modifiers);

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

struct obj 
{
	int x, y; 
	unsigned char image; 
	char name[32];
	int chr_class;
	int max_ap, cur_ap;
	int max_hp, cur_hp;
	int max_mp, cur_mp;
	int attack, defense, strength, magic;
	int level, steps;
	unsigned long experience, gold;
	
	int battle_icon_w, battle_icon_h;
	char* battle_icon;
	int drinks;
	bool drunk;

	std::vector<shop_item*> items;
	std::vector<char*> items_equipped;
	std::vector<shop_spell*> spells;
};

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

typedef void (*statefunc)();

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

extern unsigned long xp_to_level_up();
extern void end_battle(void*);
extern void level_up();

extern bool  obj_has_item(obj* o, char* name);
extern bool  obj_has_equipped(obj* o, char* name);
extern bool  obj_has_spell(obj* o, char* name);

extern obj*  create_obj(int x, int y, unsigned char image);
extern obj*  create_enemy(char* name, int lvl, int mhp, int mmp, long gold, long xp, int atk, int dfn, int str);
extern void  draw_obj(obj* o);
extern bool  can_move(obj* o, char d);
extern void  move_obj(obj* o, char d);

extern char* load_map(char* file_name);
extern void  change_map(int new_map);
extern void  draw_map();
extern void  debug_draw_event_locations();
extern void  draw_game_display();

extern void  gsinit();
extern void  gsingame();

extern void gstitle_screen();
extern void gsmain_menu();

extern void gscreate_character();
extern void gsname_character();


extern bool save_game_ok(int id);
extern bool load_game_ok(int id);
extern void savegame_screen();
extern void loadgame_screen();

extern void display_item_detail(char* name);

// event functions
extern void warp_to(void*);

// DUSHAL EVENTS
extern void event_dushal_player_home(void*);
extern void event_dushal_inn(void*);
extern void event_dushal_pub(void*);
extern void event_dushal_magic_shop(void*);
extern void event_dushal_armor_shop(void*);
extern void event_dushal_weapon_shop(void*);
extern void event_dushal_item_shop(void*);

// KIRAR EVENTS
extern void event_kirar_thieves_hut(void* d);
extern void event_kirar_treasure_room(void* d);
extern void event_kirar_cave_entrance(void* d);
extern void event_kirar_inn(void*);
extern void event_kirar_pub(void*);
extern void event_kirar_magic_shop(void*);
extern void event_kirar_armor_shop(void*);
extern void event_kirar_weapon_shop(void*);
extern void event_kirar_item_shop(void*);

extern bool spawn_battle(void* enemy); // returns true if you win false if you lose

extern int shop_form_a(void* d); // choose to buy or sell
extern int shop_form_b(void* d); // select items to buy
extern int shop_form_c(void* d); // select items to sell
extern void draw_shop_ext();
extern void hide_shop_ext();

extern int battle_engine_a(void* d); // choose to fight or run
extern int battle_engine_b(void* d); // choose an item to use
extern int battle_engine_c(void* d); // choose a spell to cast
extern int battle_engine_d(void* d); // main battle screen


extern void ingame_menu(); // handles the ingame menu
extern void ingame_inventory(); // handle the inventory
extern void ingame_equipment();
extern void ingame_magic();
extern void ingame_status();
extern void ingame_system(); // save and load games
/*
extern void play_midi(char* filename, bool loop);
extern void loop_midi(); // call this in a loop to replay the midi
extern void stop_midi();
*/


///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

// globals
/*
typedef unsigned int MIDI_ID;

extern MIDI_ID			mapbgm[5];
extern MIDI_ID			dushal_bgm;
extern MIDI_ID			kirar_bgm;
extern MIDI_ID			world_bgm;
extern MIDI_ID			battlestart_bgm;
extern MIDI_ID			battle_bgm;
extern MIDI_ID			battlewin_bgm;
extern MIDI_ID			rest_bgm;
*/
extern clib*			cl;
extern obj*				player;
extern statefunc		gamestate;

extern int calc_atk_damage(void* a, void* b);

extern bool				entered_thieves_hut;
extern bool				have_treasure_room_key;
extern bool				entered_treasure_room;
extern bool				accepted_beer_quest;
extern bool				have_found_beer;
extern bool				entered_karns_pub;
extern bool				beggars_bar_open;
extern bool				boss_battle;

extern void reset_game_vars();

extern char				warrior_icon[13*11];
extern char				dwarf_icon[13*11];
extern char				thief_icon[13*11];
extern char				mage_icon[13*11];

// ITEMS
extern int leather_vest_modifiers[10];
extern int leather_boots_modifiers[10];
extern int leather_gauntlets_modifiers[10];
extern int short_sword_modifiers[10];
extern int heavy_axe_modifiers[10];
extern int wooden_bow_modifiers[10];
extern int wooden_staff_modifiers[10];
extern int silver_vest_modifiers[10];
extern int silver_boots_modifiers[10];
extern int silver_gauntlets_modifiers[10];
extern int silver_sword_modifiers[10];
extern int silver_axe_modifiers[10];
extern int silver_bow_modifiers[10];
extern int silver_staff_modifiers[10];
extern int healing_powder_modifiers[10];
extern int escape_roots_modifiers[10];
extern int burn_scroll_modifiers[10];
extern int break_scroll_modifiers[10];
extern int anti_venom_modifiers[10];
extern int inferno_scroll_modifiers[10];
extern int shatter_scroll_modifiers[10];
extern int drain_scroll_modifiers[10];

extern int beerpint_modifiers[10];
extern int bottledbeer_modifiers[10];
extern int liquor_modifiers[10];

extern void use_beerpint(void*);
extern void use_bottledbeer(void*);
extern void use_liquor(void*);

extern void drop_beerpint(void*);
extern void drop_bottledbeer(void*);
extern void drop_liquor(void*);

// ITEM FUNCTIONS
extern void equip_leather_vest(void*);
extern void equip_leather_boots(void*);
extern void equip_leather_gauntlets(void*);
extern void unequip_leather_vest(void*);
extern void unequip_leather_boots(void*);
extern void unequip_leather_gauntlets(void*);
extern void equip_short_sword(void*);
extern void equip_heavy_axe(void*);
extern void equip_wooden_bow(void*);
extern void equip_wooden_staff(void*);
extern void unequip_short_sword(void*);
extern void unequip_heavy_axe(void*);
extern void unequip_wooden_bow(void*);
extern void unequip_wooden_staff(void*);
extern void equip_silver_vest(void*);
extern void equip_silver_boots(void*);
extern void equip_silver_gauntlets(void*);
extern void unequip_silver_vest(void*);
extern void unequip_silver_boots(void*);
extern void unequip_silver_gauntlets(void*);
extern void equip_silver_sword(void*);
extern void equip_silver_axe(void*);
extern void equip_silver_bow(void*);
extern void equip_silver_staff(void*);
extern void unequip_silver_sword(void*);
extern void unequip_silver_axe(void*);
extern void unequip_silver_bow(void*);
extern void unequip_silver_staff(void*);
extern void use_healing_powder(void*);
extern void use_escape_roots(void*);
extern void use_burn_scroll(void*);
extern void use_break_scroll(void*);
extern void drop_healing_powder(void*);
extern void drop_escape_roots(void*);
extern void drop_burn_scroll(void*);
extern void drop_break_scroll(void*);
extern void use_anti_venom(void*);
extern void use_inferno_scroll(void*);
extern void use_shatter_scroll(void*);
extern void use_drain_scroll(void*);
extern void drop_anti_venom(void*);
extern void drop_inferno_scroll(void*);
extern void drop_shatter_scroll(void*);
extern void drop_drain_scroll(void*);



// SPELLS

extern int burn_spell_modifiers[6];
extern int break_spell_modifiers[6];
extern int inferno_spell_modifiers[6];
extern int shatter_spell_modifiers[6];
extern int drain_spell_modifiers[6];

// SPELL FUNCTIONS
extern void cast_burn_spell(void*);
extern void cast_break_spell(void*);
extern void cast_inferno_spell(void*);
extern void cast_shatter_spell(void*);
extern void cast_drain_spell(void*);



#define MAP_W 30
#define MAP_H 14
#define MAP_COUNT			5
#define MAPID_WORLD_MAP		0
#define MAPID_KIRAR_NORTH	1
#define MAPID_KIRAR_SOUTH	2
#define MAPID_DUSHAL_WEST	3
#define MAPID_DUSHAL_EAST	4
extern char* map[MAP_COUNT];

extern int previous_map;
extern int current_map;


#endif
