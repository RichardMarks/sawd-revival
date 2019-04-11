
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

#include "mapevents.h"

MapEventManager::MapEventManager()
{
}

MapEventManager::~MapEventManager()
{
	for (mapevent_list::iterator i = events_.begin(); i < events_.end(); i++)
	{
		delete (*i);
	}
	events_.clear();
}

int MapEventManager::AddWarp(int x, int y, int map_id, void (*func)(void*d), warptarget* target)
{
	int p = (int)events_.size() + 1;
	mapevent* e = new mapevent;
	e->enabled = true;
	e->onetime = false;
	e->x = x;
	e->y = y;
	e->data = target;
	e->func = func;
	e->map_id = map_id;
	events_.push_back(e);
	return p;
}

int MapEventManager::AddEvent(int x, int y, int map_id, void (*func)(void*d), void* data, bool onetime, bool enabled)
{
	int p = (int)events_.size() + 1;
	mapevent* e = new mapevent;
	e->enabled = enabled;
	e->onetime = onetime;
	e->x = x;
	e->y = y;
	e->data = data;
	e->func = func;
	e->map_id = map_id;
	events_.push_back(e);
	return p;
}

void MapEventManager::RemoveEvent(int index)
{
	if (index > (int)events_.size()) return;
	int n = 0;
	for (mapevent_list::iterator i = events_.begin(); i < events_.end(); i++)
	{
		if (n == index)
		{
			delete (*i);
			events_.erase(i);
		}
		n++;
	}
}

mapevent* MapEventManager::GetEvent(int index)
{
	if (index > (int)events_.size()) return 0;
	return events_.at(index);
}

int MapEventManager::EventCount()
{
	return (int)events_.size();
}
