
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

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

std::vector<shop_item*> master_item_list;
std::vector<shop_spell*> master_magic_list;

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

shop_item* create_item(char* name, unsigned int cost, void(*use_func)(void*), 
					   void(*drop_func)(void*), int* modifiers)
{
	shop_item* itm		= new shop_item;
	itm->cost			= cost;
	itm->name			= name;
	itm->can_equip		= false;
	itm->use_func		= use_func;
	itm->drop_func		= drop_func;
	itm->equip_func		= 0;
	itm->unequip_func	= 0;

	for (unsigned int i = 0; i < 10; i++)
	{
		itm->modifiers[i] = modifiers[i];
	}

	return itm;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

shop_item* create_equipment(int chrclass, char* name, unsigned int cost, void(*equip_func)(void*),
					   void(*unequip_func)(void*), int* modifiers)
{
	shop_item* itm		= new shop_item;
	itm->chr_class		= chrclass;
	itm->cost			= cost;
	itm->name			= name;
	itm->can_equip		= true;
	itm->use_func		= 0;
	itm->drop_func		= 0;
	itm->equip_func		= equip_func;
	itm->unequip_func	= unequip_func;
	
	for (unsigned int i = 0; i < 10; i++)
	{
		itm->modifiers[i] = modifiers[i];
	}

	return itm;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

shop_spell* create_spell(char* name, unsigned int cost, int mp_cost, void(*cast_func)(void*), int* modifiers)
{
	shop_spell* spell	= new shop_spell;
	spell->cost			= cost;
	spell->mp_cost		= mp_cost;
	spell->name			= name;
	spell->cast_func	= cast_func;
	
	for (unsigned int i = 0; i < 6; i++)
	{
		spell->modifiers[i] = modifiers[i];
	}


	return spell;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////
int leather_vest_modifiers[10]		= {0,0,0,4,0,0,0,0,0,0};
int leather_boots_modifiers[10]		= {0,0,0,5,0,0,0,0,0,0};
int leather_gauntlets_modifiers[10]	= {0,0,0,4,0,0,0,0,0,0};

int short_sword_modifiers[10]		= {0,0,4,0,1,0,0,0,0,0};
int heavy_axe_modifiers[10]			= {0,0,4,0,1,0,0,0,0,0};
int wooden_bow_modifiers[10]		= {0,0,4,0,1,0,0,0,0,0};
int wooden_staff_modifiers[10]		= {0,0,4,0,1,0,0,0,0,0};

int silver_vest_modifiers[10]		= {0,0,0,8,1,0,0,0,0,0};
int silver_boots_modifiers[10]		= {0,0,0,9,1,0,0,0,0,0};
int silver_gauntlets_modifiers[10]	= {0,0,0,8,1,0,0,0,0,0};

int silver_sword_modifiers[10]		= {0,0,8,0,2,0,0,0,0,0};
int silver_axe_modifiers[10]		= {0,0,8,0,2,0,0,0,0,0};
int silver_bow_modifiers[10]		= {0,0,8,0,2,0,0,0,0,0};
int silver_staff_modifiers[10]		= {0,0,8,0,2,0,0,0,0,0};

int healing_powder_modifiers[10]	= {25,0,0,0,0,0,0,0,0,0};
int escape_roots_modifiers[10]		= {0,0,0,0,0,0,0,0,0,0};
int burn_scroll_modifiers[10]		= {-10,0,0,0,0,0,0,0,0,0};
int break_scroll_modifiers[10]		= {0,0,0,-1,0,0,0,0,0,0};
int anti_venom_modifiers[10]		= {0,0,0,0,0,0,0,0,0,0};
int inferno_scroll_modifiers[10]	= {-20,0,0,-2,0,0,0,0,0,0};
int shatter_scroll_modifiers[10]	= {0,0,0,-5,0,0,0,0,0,0};
int drain_scroll_modifiers[10]		= {-30,0,0,0,0,0,0,0,0,0};

int beerpint_modifiers[10]			= {0,0,0,0,0,0,0,0,0,0};
int bottledbeer_modifiers[10]		= {0,0,0,0,0,0,0,0,0,0};
int liquor_modifiers[10]			= {0,0,0,0,0,0,0,0,0,0};

void create_master_item_list()
{
	/*
	000 Healing Powder		$15
	001 Escape Roots		$800
	002 Burn Scroll			$50
	003 Break Scroll		$80
	004 Anti-Venom			$50
	005 Inferno Scroll		$250
	006 Shatter Scroll		$350
	007 Drain Scroll		$450

	008 Leather Vest		$30		defense + 4
	009 Leather Boots		$40		defense + 5
	010 Leather Gauntlets	$40		defense + 4
	011 Silver Vest			$250
	012 Silver Boots		$150
	013 Silver Gauntlets	$350

	014 Short Sword			$100 (warrior only)
	015 Heavy Axe			$100 (dwarf only)
	016 Wooden Bow			$100 (thief only)
	017 Wooden Staff		$100 (mage only)
	018 Silver Sword		$500 (warrior only)
	019 Silver Axe			$500 (dwarf only)
	020 Silver Bow			$500 (thief only)
	021 Silver Staff		$500 (mage only)

	*/


	{ shop_item* i = create_item("Healing Powder",15,use_healing_powder, drop_healing_powder, healing_powder_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Escape Roots",800,use_escape_roots, drop_escape_roots, escape_roots_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Burn Scroll",50,use_burn_scroll, drop_burn_scroll, burn_scroll_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Break Scroll",80,use_break_scroll, drop_break_scroll, break_scroll_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Anti-Venom",50,use_anti_venom, drop_anti_venom, anti_venom_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Inferno Scroll",250,use_inferno_scroll, drop_inferno_scroll, inferno_scroll_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Shatter Scroll",350,use_shatter_scroll, drop_shatter_scroll, shatter_scroll_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Drain Scroll",450,use_drain_scroll, drop_drain_scroll, drain_scroll_modifiers); add_master_item(i); }

	{ shop_item* i = create_item("Pint 'O Beer",7,use_beerpint, drop_beerpint, beerpint_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Bottled Draught",4,use_bottledbeer, drop_bottledbeer, bottledbeer_modifiers); add_master_item(i); }
	{ shop_item* i = create_item("Hard Liquor",10,use_liquor, drop_liquor, liquor_modifiers); add_master_item(i); }


	{ shop_item* i = create_equipment(9,"Leather Vest", 30, equip_leather_vest, unequip_leather_vest, leather_vest_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(9,"Leather Boots", 40, equip_leather_boots, unequip_leather_boots, leather_boots_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(9,"Leather Gauntlets", 40, equip_leather_gauntlets, unequip_leather_gauntlets, leather_gauntlets_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(9,"Silver Vest", 250, equip_silver_vest, unequip_silver_vest, silver_vest_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(9,"Silver Boots", 150, equip_silver_boots, unequip_silver_boots, silver_boots_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(9,"Silver Gauntlets", 350, equip_silver_gauntlets, unequip_silver_gauntlets, silver_gauntlets_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(0,"Short Sword", 100, equip_short_sword, unequip_short_sword, short_sword_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(1,"Heavy Axe", 100, equip_heavy_axe, unequip_heavy_axe, heavy_axe_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(2,"Wooden Bow", 100, equip_wooden_bow, unequip_wooden_bow, wooden_bow_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(3,"Wooden Staff", 100, equip_wooden_staff, unequip_wooden_staff, wooden_staff_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(0,"Silver Sword", 500, equip_silver_sword, unequip_silver_sword, silver_sword_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(1,"Silver Axe", 500, equip_silver_axe, unequip_silver_axe, silver_axe_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(2,"Silver Bow", 500, equip_silver_bow, unequip_silver_bow, silver_bow_modifiers); add_master_item(i); }
	{ shop_item* i = create_equipment(3,"Silver Staff", 500, equip_silver_staff, unequip_silver_staff, silver_staff_modifiers); add_master_item(i); }

}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

int burn_spell_modifiers[6]			= {-10,0,0,0,0,0};
int break_spell_modifiers[6]		= {0,0,0,-1,0,0};
int inferno_spell_modifiers[6]		= {-20,0,0,-2,0,0};
int shatter_spell_modifiers[6]		= {0,0,0,-5,0,0};
int drain_spell_modifiers[6]		= {-30,0,0,0,0,0};

void create_master_magic_list()
{
	/*
	Learn Burn			$100
	Learn Break			$200
	Learn Inferno		$1000
	Learn Shatter		$2000
	Learn Drain			$3000
	*/

	{ shop_spell* s = create_spell("Burn", 100, 8, cast_burn_spell, burn_spell_modifiers); add_master_magic(s); }
	{ shop_spell* s = create_spell("Break", 200, 10, cast_break_spell, break_spell_modifiers); add_master_magic(s); }
	{ shop_spell* s = create_spell("Inferno", 1000, 20, cast_inferno_spell, inferno_spell_modifiers); add_master_magic(s); }
	{ shop_spell* s = create_spell("Shatter", 2000, 30, cast_shatter_spell, shatter_spell_modifiers); add_master_magic(s); }
	{ shop_spell* s = create_spell("Drain", 3000, 20, cast_drain_spell, drain_spell_modifiers); add_master_magic(s); }

}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void add_master_item(shop_item* item)
{
	master_item_list.push_back(item);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void add_master_magic(shop_spell* spell)
{
	master_magic_list.push_back(spell);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

ITEM_ID get_item_id(char* name)
{
	ITEM_ID id = OUT_OF_RANGE_ID;
	unsigned int index = 0;
	while (index < master_item_list.size())
	{
		if (strcmp(master_item_list.at(index)->name, name)==0)
		{
			id = index;
			break;
		}
		index++;
	}
	return id;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

MAGIC_ID get_magic_id(char* name)
{
	MAGIC_ID id = OUT_OF_RANGE_ID;
	unsigned int index = 0;
	while (index < master_magic_list.size())
	{
		if (strcmp(master_magic_list.at(index)->name, name)==0)
		{
			id = index;
			break;
		}
		index++;
	}
	return id;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

shop_item* get_master_item(char* name)
{
	ITEM_ID id = get_item_id(name);
	if (OUT_OF_RANGE_ID != id)
	{
		return master_item_list.at(id);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

shop_spell* get_master_magic(char* name)
{
	MAGIC_ID id = get_magic_id(name);
	if (OUT_OF_RANGE_ID != id)
	{
		return master_magic_list.at(id);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void destroy_master_item_list()
{
	for (unsigned int index = 0; index < master_item_list.size(); index++)
	{
		delete master_item_list[index];
	}
	master_item_list.clear();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void destroy_master_magic_list()
{
	for (unsigned int index = 0; index < master_magic_list.size(); index++)
	{
		delete master_magic_list[index];
	}
	master_magic_list.clear();
}
