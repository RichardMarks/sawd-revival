
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
#ifndef MAPEVENTS_H
#define MAPEVENTS_H
#include <vector>

struct mapevent
{
	int x;
	int y;
	int map_id;
	void* data;
	void (*func)(void*);
	bool onetime;
	bool enabled;
};

struct warptarget
{
	warptarget(int x1, int y1, int map_id1) : x(x1), y(y1), map_id(map_id1) {}
	int x;
	int y;
	int map_id;
};

typedef std::vector<mapevent*> mapevent_list;
typedef std::vector<warptarget*> warptarget_list;

class MapEventManager
{
public:
	MapEventManager();
	~MapEventManager();

	int AddWarp(int x, int y, int map_id, void (*func)(void*d), warptarget* target);

	int AddEvent(int x, int y, int map_id, void (*func)(void*d), void* data = 0, bool onetime = true, bool enabled = true);
	void RemoveEvent(int index);
	mapevent* GetEvent(int index);
	int EventCount();
private:
	mapevent_list events_;
};

extern MapEventManager*	map_manager;
extern warptarget*		warp_target[1024];

#endif
