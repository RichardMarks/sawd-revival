
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

#include "clib.h"
#include "sawd.h"

// icons 13x11
char warrior_icon[13*11] =
{
	'@','@','@','@','@','@','@','@','@','@','@','@','@',
	'@','@','@','@','@','@','@','@','@','|','@','@','@',
	'@','@','@','@','@','@','@','@','@','|','@','@','@',
	'@','@','@','@','@','/','\\','@','@','|','@','@','@',
	'@','@','@','@','@','\\','/','@','@','B','@','@','@',
	'@','@','@','/','R','>','<','R','/',';','@','@','@',
	'@','@','/','@','\\','q','p','/','@','@','@','@','@',
	'@','@','b','@','@','-','-','@','@','@','@','@','@',
	'@','@','@','@','(',')','(',')','@','@','@','@','@',
	'@','@','@','@','(',')','(',')','@','@','@','@','@',
	'@','@','@','~','~','@','@','~','~','@','@','@','@'
};

char dwarf_icon[13*11] =
{
	'@','@','@','@','@','@','@','@','@','@','@','@','@',
	'@','@','@','@','@','@','@','@','@','@','@','@','@',
	'@','@','@','@','@','@','@','@','@','@','@','@','@',
	'@','@','_','T','_','_','@','@','@','@','@','@','@',
	'@','D','@','@','@','/','@','@','@','@','@','@','@',
	'@','D','_','_','/','@','/','\\','@','@','@','@','@',
	'@','@','@','|','@','@','\\','/','@','@','@','@','@',
	'@','@','@','B','m','R','>','<','R','\\','@','@','@',
	'@','@','@','|','@','/','q','p','\\','d','@','@','@',
	'@','@','@','|','_','8','@','@','8','_','@','@','@',
	'@','@','@','@','~','~','@','@','~','~','@','@','@'
};

char thief_icon[13*11] =
{
	'@','@','@','@','@','@','@','@','@','@','@','@','@',
	'@','@','@','@','@','@','@','@','@','@','@','@','@',
	'@','@','@','@','@','@','@','|','\\','@','@','@','@',
	'@','@','@','@','@','@','@','|','@','\\','@','@','@',
	'@','@','@','@','@','/','\\','|','@','@','b','@','@',
	'@','@','@','-','-','\\','/','|','@','@','T','@','@',
	'@','@','@','@','\\','R','=','-','-','m','B','@','@',
	'@','@','@','@','@','q','p','|','@','/','@','@','@',
	'@','@','@','@','/','@','@','|','/','@','@','@','@',
	'@','@','@','y','@','@','@','@','\\','@','@','@','@',
	'@','@','@','~','~','@','@','@','~','~','@','@','@'
};

char mage_icon[13*11] =
{
	'@','@','@','@','@','@','@','\\','|','/','@','@','@',
	'@','@','@','@','@','@','-','-','0','-','-','@','@',
	'@','@','@','@','/','\\','@','/','|','\\','@','@','@',
	'@','@','@','/','@','@','\\','@','|','@','@','@','@',
	'@','@','@','~','!','!','~','@','|','@','@','@','@',
	'@','@','@','@','W','W','@','@','|','@','@','@','@',
	'@','@','/','R','@','@','R','m','B','@','@','@','@',
	'@','@','|','\\','_','_','/','@','|','@','@','@','@',
	'@','@','b','/','@','@','\\','@','|','@','@','@','@',
	'@','@','@','8','W','W','8','@','|','@','@','@','@',
	'@','@','@','~','~','~','~','@','@','@','@','@','@'
};
