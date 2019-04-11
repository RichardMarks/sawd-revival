
/*
	clib - A Simple Windows API Console Library (C) Copyright 2008, CCPS Solutions
	
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
#ifndef CLIB_H
#define CLIB_H

struct _CHAR_INFO;

class clibwindow
{
public:
	clibwindow(int x, int y, int w, int h);
	~clibwindow();
	int data_[4];
	_CHAR_INFO* buffer_;
};

class clib
{
public:
	clib();
	~clib();
	void initialize_high_res();
	void screen80x25();
	void screen80x50();
	void set_bgcolor(unsigned short color);
	void set_fgcolor(unsigned short color);
	void set_xy(int x, int y);
	void cls();
	void outchar(int x, int y, char c);
	void outchars(int x, int y, char* text);
	void outcharsf(int x, int y, char* text, ...);
	void read(char* buffer, unsigned long bs);
	int get_key();

	// gui functions moved to the cl lib
	// this will affect a large part of the game
	// but is worth the change to prevent need to
	// redraw things because the method will handle it
	// itself
	clibwindow* open_window(int x, int y, int w, int h, unsigned short bgc=4, unsigned short fgc=1|2|4|8);
	void close_window(clibwindow* wnd);

	clibwindow* open_window_noframe(int x, int y, int w, int h, unsigned short bgc=4, unsigned short fgc=1|2|4|8);

	void show_message_centered(int lines, ...);
	void show_message(int lines, ...);
	int  show_choice(char* message);
	int show_choice_ex(char* message, int options, ...);
    void draw_button(int x, int y, int w, int h, char* caption, unsigned short bgc=4, unsigned short fgc=1|2|4|8);

private:
	void* screen_;
	void* keyboard_;
	unsigned short bgc_;
	unsigned short fgc_;
};

#endif
