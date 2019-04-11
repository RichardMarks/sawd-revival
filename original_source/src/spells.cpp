
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

void cast_burn_spell(void* d)
{
	item_target* t	= (item_target*)d;
	shop_spell* self = (shop_spell*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	/*
		burns the enemy's flesh
	*/
	int dmg = calc_atk_damage(player, d);
	dmg += player->level * player->magic;
	dmg -= target->defense;

	char m1[255];char m2[255];
	sprintf_s(m1, 255, "%s is cast on the %s.", self->name, target->name);
	sprintf_s(m2, 255, "The %s takes %d damage.", target->name, dmg);
	cl->show_message(3, m1, "", m2);

	player->cur_mp -= self->mp_cost;
	if (player->cur_mp < 0)player->cur_mp=0;

	target->cur_hp -= dmg;
	if (target->cur_hp < 0)
	{
		target->cur_hp = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void cast_break_spell(void* d)
{
	item_target* t	= (item_target*)d;
	shop_spell* self = (shop_spell*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	/*
		breaks the enemy's defenses
	*/
	int dmg = calc_atk_damage(player, d);
	dmg += player->level * player->magic;
	dmg -= target->defense;

	char m1[255];char m2[255];
	sprintf_s(m1, 255, "%s is cast on the %s.", self->name, target->name);
	sprintf_s(m2, 255, "The %s's defense drops.", target->name, dmg);
	cl->show_message(3, m1, "", m2);

	player->cur_mp -= self->mp_cost;
	if (player->cur_mp < 0)player->cur_mp=0;

	target->defense -= dmg;
	if (target->defense < 0)
	{
		target->defense = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void cast_inferno_spell(void* d)
{
	item_target* t	= (item_target*)d;
	shop_spell* self = (shop_spell*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	/*
		the enemy is set ablaze
	*/
	int dmg = calc_atk_damage(player, d);
	dmg += player->level * player->magic * 2;
	dmg -= target->defense;

	char m1[255];char m2[255];
	sprintf_s(m1, 255, "%s is cast on the %s.", self->name, target->name);
	sprintf_s(m2, 255, "The %s takes %d damage.", target->name, dmg);
	cl->show_message(3, m1, "", m2);

	player->cur_mp -= self->mp_cost;
	if (player->cur_mp < 0)player->cur_mp=0;

	target->cur_hp -= dmg;
	if (target->cur_hp < 0)
	{
		target->cur_hp = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void cast_shatter_spell(void* d)
{
	item_target* t	= (item_target*)d;
	shop_spell* self = (shop_spell*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	/*
		breaks the enemy's defenses
	*/
	int dmg = calc_atk_damage(player, d);
	dmg += player->level * player->magic * 2;
	dmg -= target->defense;

	char m1[255];char m2[255];
	sprintf_s(m1, 255, "%s is cast on the %s.", self->name, target->name);
	sprintf_s(m2, 255, "The %s's defense drops.", target->name, dmg);
	cl->show_message(3, m1, "", m2);

	player->cur_mp -= self->mp_cost;
	if (player->cur_mp < 0)player->cur_mp=0;

	target->defense -= dmg;
	if (target->defense < 0)
	{
		target->defense = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void cast_drain_spell(void* d)
{
	item_target* t	= (item_target*)d;
	shop_spell* self = (shop_spell*)t->GetSelf();
	obj* target		= (obj*)t->GetTarget();

	int dmg = calc_atk_damage(player, d);
	dmg += self->modifiers[SPELL_MODIFIER_HP] * player->level;
	dmg += player->level * player->magic * 2;
	dmg -= target->defense;


	char m1[255];char m2[255];
	sprintf_s(m1, 255, "%s is cast on the %s.", self->name, target->name);
	sprintf_s(m2, 255, "The %s takes %d damage.", target->name, dmg);
	cl->show_message(3, m1, "", m2);

	target->cur_hp -= dmg;
	if (target->cur_hp < 0)
	{
		target->cur_hp = 0;
	}

	player->cur_hp += dmg;
	cl->show_message(3,"","You drained life from the enemy!","");
	if (player->cur_hp > player->max_hp)
	{
		player->cur_hp = player->max_hp;
	}

	player->cur_mp -= self->mp_cost;
	if (player->cur_mp < 0)player->cur_mp=0;
}
