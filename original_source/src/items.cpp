
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

// satisfy the item target class
item_target::item_target(void* self, void* target):self_(self),target_(target){}
item_target::~item_target(){}
void* item_target::GetSelf(){return self_;}
void* item_target::GetTarget(){return target_;}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////
void setmods(obj*o, int*mods)
{
	o->cur_hp += mods[ITEM_MODIFIER_HP];
	o->cur_mp += mods[ITEM_MODIFIER_MP];
	o->cur_ap += mods[ITEM_MODIFIER_AP];
	o->attack += mods[ITEM_MODIFIER_ATTACK];
	o->defense += mods[ITEM_MODIFIER_DEFENSE];
	o->strength += mods[ITEM_MODIFIER_STRENGTH];
	o->magic += mods[ITEM_MODIFIER_MAGIC];
	o->max_hp += mods[ITEM_MODIFIER_MAX_HP];
	o->max_mp += mods[ITEM_MODIFIER_MAX_MP];
	o->max_ap += mods[ITEM_MODIFIER_MAX_AP];
}

void unsetmods(obj*o, int*mods)
{
	o->cur_hp -= mods[ITEM_MODIFIER_HP];
	o->cur_mp -= mods[ITEM_MODIFIER_MP];
	o->cur_ap -= mods[ITEM_MODIFIER_AP];
	o->attack -= mods[ITEM_MODIFIER_ATTACK];
	o->defense -= mods[ITEM_MODIFIER_DEFENSE];
	o->strength -= mods[ITEM_MODIFIER_STRENGTH];
	o->magic -= mods[ITEM_MODIFIER_MAGIC];
	o->max_hp -= mods[ITEM_MODIFIER_MAX_HP];
	o->max_mp -= mods[ITEM_MODIFIER_MAX_MP];
	o->max_ap -= mods[ITEM_MODIFIER_MAX_AP];
}

void equip_leather_vest(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	setmods(target, self->modifiers);
	target->items_equipped.push_back(self->name);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_leather_boots(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	setmods(target, self->modifiers);
	target->items_equipped.push_back(self->name);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_leather_gauntlets(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	setmods(target, self->modifiers);
	target->items_equipped.push_back(self->name);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_leather_vest(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_leather_boots(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_leather_gauntlets(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_short_sword(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 0)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Short Sword!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_heavy_axe(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 1)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Heavy Axe!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_wooden_bow(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 2)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Wooden Bow!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_wooden_staff(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 3)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Wooden Staff!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_short_sword(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_heavy_axe(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_wooden_bow(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_wooden_staff(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_silver_vest(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	setmods(target, self->modifiers);
	target->items_equipped.push_back(self->name);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_silver_boots(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	setmods(target, self->modifiers);
	target->items_equipped.push_back(self->name);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_silver_gauntlets(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	setmods(target, self->modifiers);
	target->items_equipped.push_back(self->name);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_silver_vest(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_silver_boots(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_silver_gauntlets(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_silver_sword(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 0)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Silver Sword!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_silver_axe(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 1)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Silver Axe!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_silver_bow(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 2)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Silver Bow!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void equip_silver_staff(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	if (target->chr_class == 3)
	{
		setmods(target, self->modifiers);
		target->items_equipped.push_back(self->name);
	}
	else
	{
		cl->show_message(3,"","Cannot equip the Silver Staff!","");
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_silver_sword(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_silver_axe(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_silver_bow(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	unsetmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void unequip_silver_staff(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_healing_powder(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	char m[80];

	int hp = self->modifiers[ITEM_MODIFIER_HP];

	player->cur_hp += hp;

	if (player->cur_hp > player->max_hp)
	{
		player->cur_hp = player->max_hp;
	}

	sprintf_s(m,80,"%s: %d HP Restored!",target->name, hp);
	cl->show_message(3,"",m,"");
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_escape_roots(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","Nothing Happened...","");
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_burn_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","BURN SPELL SCROLL USED","");
	setmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_break_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","BREAK SPELL SCROLL USED","");
	setmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_healing_powder(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_escape_roots(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_burn_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_break_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_anti_venom(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","Nothing Happened...","");
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_inferno_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","INFERNO SPELL SCROLL USED","");
	setmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_shatter_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","SHATTER SPELL SCROLL USED","");
	setmods(target, self->modifiers);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_drain_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","DRAIN SPELL SCROLL USED","");
	setmods(target, self->modifiers);
	player->cur_hp += self->modifiers[ITEM_MODIFIER_HP];
	cl->show_message(3,"","You drained life from the enemy!","");
	if (player->cur_hp > player->max_hp)
		player->cur_hp = player->max_hp;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_anti_venom(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_inferno_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_shatter_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_drain_scroll(void* d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_beerpint(void*d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message(3,"","You down your Pint 'O Beer and feel a bit dizzy.","");
	player->drinks+=4;
	
	if (player->drinks > player->level*2)
	{
		cl->show_message_centered(3,"","You are drunk! Be careful out there!","");
		player->drunk = true;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_bottledbeer(void*d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message_centered(3,"","Ahh that sure does taste good...","");
	player->drinks++;
	
	if (player->drinks > player->level*2)
	{
		cl->show_message_centered(3,"","You are drunk! Be careful out there!","");
		player->drunk = true;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void use_liquor(void*d)
{
	item_target* t	= (item_target*)d;
	shop_item* self = (shop_item*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	cl->show_message_centered(3,"","Ahh that sure does taste good...","");
	player->drinks+=10;
	
	if (player->drinks > player->level*2)
	{
		cl->show_message_centered(3,"","You are drunk! Be careful out there!","");
		player->drunk = true;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_beerpint(void*d)
{
	cl->show_message_centered(3,"","You dropped a pint of beer! Oh the humanity!","");
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_bottledbeer(void*d)
{
	cl->show_message_centered(3,"","Dropping beer bottles is fun to you?","");
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void drop_liquor(void*d)
{
	cl->show_message_centered(3,"","What a waste of good liquor!","");
}
