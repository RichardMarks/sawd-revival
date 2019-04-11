
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

#include "clib.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////
clibwindow::clibwindow(int x, int y, int w, int h)
{
	data_[0] = x;
	data_[1] = y;
	data_[2] = w;
	data_[3] = h;
	buffer_ = new _CHAR_INFO [w * h];
}

clibwindow::~clibwindow()
{
	delete [] buffer_;
	buffer_ = 0;
	data_[0] = 0;
	data_[1] = 0;
	data_[2] = 0;
	data_[3] = 0;
}
///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

clib::clib():
screen_(GetStdHandle(STD_OUTPUT_HANDLE)),
keyboard_(GetStdHandle(STD_INPUT_HANDLE)),
bgc_(0), fgc_(1|2|4)
{
	_CONSOLE_CURSOR_INFO csrinfo;
	csrinfo.bVisible = FALSE;
	csrinfo.dwSize   = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csrinfo);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

clib::~clib(){}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::initialize_high_res()
{
	_COORD bsz = {80, 50};
	_SMALL_RECT sz = {0, 0, 79, 49};
	SetConsoleWindowInfo(screen_, TRUE, &sz);
	SetConsoleScreenBufferSize(screen_, bsz);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::screen80x25()
{
	_COORD bsz = {80, 25};
	_SMALL_RECT sz = {0, 0, 79, 24};

	if (!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &sz))
	{
		char eb[1024];
		sprintf_s(eb,1024,"SetConsoleWindowInfo failed!\n\nError Code: %d",GetLastError());
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}

	if (!SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bsz))
	{
		char eb[1024];
		sprintf_s(eb,1024,"SetConsoleScreenBufferSize failed!\n\nError Code: %d",GetLastError());
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}

	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	HWND conWnd = GetConsoleWindow();

	tagRECT r;
	GetWindowRect(conWnd, &r);
	int ww = r.right - r.left;
	int wh = r.bottom - r.top;

	int newx = sw/2-ww/2;
	int newy = sh/2-wh/2;

	//		MoveWindow(conWnd, newx, newy, ww, wh, TRUE);
	if (!MoveWindow(conWnd, newx, newy, ww, wh, TRUE))
	{
		char eb[1024];
		sprintf_s(eb,1024,"MoveWindow failed!\n\nError Code: %d\n\nWindow Dimensions: %d %d %d %d",
			GetLastError(), newx, newy, ww, wh);
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::screen80x50()
{
	_COORD bsz = {80, 50};
	_SMALL_RECT sz = {0, 0, 79, 49};
	if (!SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), bsz))
	{
		char eb[1024];
		sprintf_s(eb,1024,"SetConsoleScreenBufferSize failed!\n\nError Code: %d",GetLastError());
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}

	if (!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &sz))
	{
		char eb[1024];
		sprintf_s(eb,1024,"SetConsoleWindowInfo failed!\n\nError Code: %d",GetLastError());
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}

	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	HWND conWnd = GetConsoleWindow();

	tagRECT r;
	GetWindowRect(conWnd, &r);
	int ww = r.right - r.left;
	int wh = r.bottom - r.top;

	int newx = sw/2-ww/2;
	int newy = sh/2-wh/2;

	if (!MoveWindow(conWnd, newx, newy, ww, wh, TRUE))
	{
		char eb[1024];
		sprintf_s(eb,1024,"MoveWindow failed!\n\nError Code: %d\n\nWindow Dimensions: %d %d %d %d",
			GetLastError(), newx, newy, ww, wh);
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::set_bgcolor(unsigned short color)
{
	bgc_=0;if(color & 1){bgc_|=BACKGROUND_RED;}
	if(color & 2){bgc_|=BACKGROUND_GREEN;}
	if(color & 4){bgc_|=BACKGROUND_BLUE;}
	if(color & 8){bgc_|=BACKGROUND_INTENSITY;}
	SetConsoleTextAttribute(screen_, fgc_ | bgc_);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::set_fgcolor(unsigned short color)
{
	fgc_=0;if(color & 1){fgc_|=FOREGROUND_RED;}
	if(color & 2){fgc_|=FOREGROUND_GREEN;}
	if(color & 4){fgc_|=FOREGROUND_BLUE;}
	if(color & 8){fgc_|=FOREGROUND_INTENSITY;}
	SetConsoleTextAttribute(screen_, fgc_ | bgc_);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::cls()
{
	_COORD st; unsigned long bw; st.X = 0; st.Y = 0;
	FillConsoleOutputAttribute(screen_,fgc_|bgc_,80*50,st,&bw);
	FillConsoleOutputCharacter(screen_,32,80*50,st,&bw);
	SetConsoleCursorPosition(screen_,st);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::set_xy(int x, int y)
{
	_COORD p; p.X = x; p.Y = y;
	SetConsoleCursorPosition(screen_, p);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::outchar(int x, int y, char c)
{
	unsigned long cw;char s[]={c,'\0'};
	_COORD p; p.X = x; p.Y = y;
	SetConsoleCursorPosition(screen_, p);
	WriteConsole(screen_,s,strlen(s),&cw,0);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::outchars(int x, int y, char *text)
{
	unsigned long cw;
	_COORD p; p.X = x; p.Y = y;
	SetConsoleCursorPosition(screen_, p);
	WriteConsole(screen_,text,strlen(text),&cw,0);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::outcharsf(int x, int y, char *text, ...)
{
	unsigned long cw;
	char stringbuffer [512];
	_COORD p; p.X = x; p.Y = y;
	SetConsoleCursorPosition(screen_, p);
	
	va_list args;
	va_start (args, text);
	vsprintf_s (stringbuffer, 512, text, args);
	WriteConsole(screen_, stringbuffer, strlen(stringbuffer), &cw, 0);
	va_end (args);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::read(char *buffer, unsigned long bs)
{
	unsigned long cw; ReadConsole(keyboard_,buffer,bs,&cw,0);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

int clib::get_key()
{
	unsigned long cw; _INPUT_RECORD e;
	
	ReadConsoleInput(keyboard_, &e, 1, &cw);
	if (e.EventType == KEY_EVENT)
	{
		if (e.Event.KeyEvent.bKeyDown)
		{
			return e.Event.KeyEvent.wVirtualKeyCode;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////
// new gui methods
clibwindow* clib::open_window(int x, int y, int w, int h, unsigned short bgc, unsigned short fgc)
{
	// before drawing the window, we save the contents of the screen
	// where the window will be drawn so that we can redraw the screen
	// when the window is closed.
	clibwindow* wnd = new clibwindow(x, y, w, h);

	_COORD		w_sz;
	_COORD		w_p;
	_SMALL_RECT w_b;
	w_p.X		= 0;
	w_p.Y		= 0;
	w_sz.X		= w;
	w_sz.Y		= h;
	w_b.Left	= x;
	w_b.Top		= y;
	w_b.Right	= x + w-1;
	w_b.Bottom	= y + h-1;

	//if(!ReadConsoleOutputA(screen_, (_CHAR_INFO*)_window_buffer_, w_sz, w_p, &w_b))
	if(!ReadConsoleOutputA(screen_, (_CHAR_INFO*)wnd->buffer_, w_sz, w_p, &w_b))
	{
		char eb[1024];
		sprintf_s(eb,1024,"ReadConsoleOutputA failed!\n\nError Code: %d",GetLastError());
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}


	// draw the window
	//this->set_bgcolor(4);		// blue background
	//this->set_fgcolor(1|2|4|8); // bright white foreground

	this->set_bgcolor(bgc);
	this->set_fgcolor(fgc);

	for (int cy = 1; cy < h - 1; cy++)
	{
		for (int cx = 1; cx < w - 1; cx++)
		{
			this->outchar(x+cx, y+cy, ' ');
		}
	}

	// draw top and bottom edge
	{for (int cx = 0; cx < w; cx++)
	{
		this->outchar(x+cx, y, (char)205);
		this->outchar(x+cx, y+(h-1), (char)205);
	}}
	
	// draw left and right edge
	{for (int cy = 0; cy < h; cy++)
	{
		this->outchar(x, y+cy, (char)186);
		this->outchar(x+(w-1), y+cy, (char)186);
	}}
	
	// draw corners
	this->outchar(x, y, (char)201);
	this->outchar(x, y+(h-1), (char)200);
	this->outchar(x+(w-1), y, (char)187);
	this->outchar(x+(w-1), y+(h-1), (char)188);

	// return the saved window
	return wnd;
}

clibwindow* clib::open_window_noframe(int x, int y, int w, int h, unsigned short bgc, unsigned short fgc)
{
	// before drawing the window, we save the contents of the screen
	// where the window will be drawn so that we can redraw the screen
	// when the window is closed.
	clibwindow* wnd = new clibwindow(x, y, w, h);

	_COORD		w_sz;
	_COORD		w_p;
	_SMALL_RECT w_b;
	w_p.X		= 0;
	w_p.Y		= 0;
	w_sz.X		= w;
	w_sz.Y		= h;
	w_b.Left	= x;
	w_b.Top		= y;
	w_b.Right	= x + w-1;
	w_b.Bottom	= y + h-1;

	//if(!ReadConsoleOutputA(screen_, (_CHAR_INFO*)_window_buffer_, w_sz, w_p, &w_b))
	if(!ReadConsoleOutputA(screen_, (_CHAR_INFO*)wnd->buffer_, w_sz, w_p, &w_b))
	{
		char eb[1024];
		sprintf_s(eb,1024,"ReadConsoleOutputA failed!\n\nError Code: %d",GetLastError());
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}

	this->set_bgcolor(bgc);
	this->set_fgcolor(fgc);

	for (int cy = 0; cy < h; cy++)
	{
		for (int cx = 0; cx < w; cx++)
		{
			this->outchar(x + cx, y + cy, ' ');
		}
	}

	// return the saved window
	return wnd;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::close_window(clibwindow* wnd)
{
	_COORD		w_sz;
	_COORD		w_p;
	_SMALL_RECT w_b;
	w_p.X		= 0;
	w_p.Y		= 0;
	w_sz.X		= wnd->data_[2];
	w_sz.Y		= wnd->data_[3];
	w_b.Left	= wnd->data_[0];
	w_b.Top		= wnd->data_[1];
	w_b.Right	= wnd->data_[0] + wnd->data_[2]-1;
	w_b.Bottom	= wnd->data_[1] + wnd->data_[3]-1;
	//if(!WriteConsoleOutputA(screen_, (_CHAR_INFO*)_window_buffer_, w_sz, w_p, &w_b))
	if(!WriteConsoleOutputA(screen_, (_CHAR_INFO*)wnd->buffer_, w_sz, w_p, &w_b))
	{
		char eb[1024];
		sprintf_s(eb,1024,"WriteConsoleOutputA failed!\n\nError Code: %d",GetLastError());
		::MessageBox(0,eb,"Error",MB_OK|MB_ICONEXCLAMATION);
	}
	
	delete wnd;
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::show_message_centered(int lines, ...)
{
	int box[] = {5, (25/2) - ((lines + 4) / 2), 70, lines + 4};
	int x = box[0] + 2;
	int y = box[1] + 2;

	bool close_box = false;

	clibwindow* wnd = this->open_window(box[0], box[1], box[2], box[3]);

	// write the message on the window
	va_list vl;
	va_start(vl, lines);
	for (int index = 0; index < lines; index++)
	{
		char* msg = va_arg(vl, char*);
		this->outchars(40-(strlen(msg)/2), y + index, msg);
	}
	va_end(vl);

	while(!close_box)
	{
		int k = this->get_key();	
		switch (k)
		{
		case VK_BACK:
		case VK_ESCAPE:
		case VK_RETURN: 
		case VK_SPACE: 
				close_box = true; 
			break;
		}
	}

	this->close_window(wnd);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

void clib::show_message(int lines, ...)
{
	int box[] = {5, (25/2) - ((lines + 4) / 2), 70, lines + 4};
	int x = box[0] + 2;
	int y = box[1] + 2;

	bool close_box = false;

	clibwindow* wnd = this->open_window(box[0], box[1], box[2], box[3]);

	// write the message on the window
	va_list vl;
	va_start(vl, lines);
	for (int index = 0; index < lines; index++)
	{
		char* msg = va_arg(vl, char*);
		this->outchars(x, y + index, msg);
	}
	va_end(vl);

	while(!close_box)
	{
		int k = this->get_key();	
		switch (k)
		{
		case VK_BACK:
		case VK_ESCAPE:
		case VK_RETURN: 
		case VK_SPACE: 
				close_box = true; 
			break;
		}
	}

	this->close_window(wnd);
}

///////////////////////////////////////////////////////////////////////////////
//###########################################################################//
///////////////////////////////////////////////////////////////////////////////

int clib::show_choice(char* message)
{
	int box[] = {40 - (34 / 2), (25/2) - (9 / 2), 34, 9};

	int x = box[0] + 2;
	int y = box[1] + 2;
	int cursor_y = 0;
	int opt_y = 2;
	int opt_x = x + (box[2] / 2) - 5;
	int msg_x = x-1 + (box[2] / 2) - strlen(message) / 2;
	bool close_box = false;

	// open the window
	clibwindow* wnd = this->open_window(box[0], box[1], box[2], box[3]);
	
	while(!close_box)
	{
		this->outchars(msg_x, y, message);
		this->outchars(opt_x, y+opt_y,   "  Yes");
		this->outchars(opt_x, y+opt_y+1, "  No ");
		this->outchar(opt_x, y+opt_y + cursor_y, (char)175);

		int k = this->get_key();
		
		switch (k)
		{
		case VK_UP:
			{
				if (cursor_y > 0) cursor_y = 0;
			} break;
		case VK_DOWN:
			{
				if (cursor_y < 1) cursor_y = 1;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				close_box = true;
			} break;
		}
	}
	// close the window
	this->close_window(wnd);

	if (cursor_y == 0)
		return 1;
	else if (cursor_y == 1)
		return 0;
	return 0;
}

int clib::show_choice_ex(char* message, int options, ...)
{
	int box[] = {40 - (34 / 2), (25/2) - ((options + 4) / 2), 34, options + 4};

	int x			= box[0] + 2;
	int y			= box[1] + 2;
	int cursor_y	= 0;
	int opt_y		= 0;	
	int msg_x		= x - 1 + (box[2] / 2) - strlen(message) / 2;
	
	clibwindow* wnd = this->open_window(box[0], box[1], box[2], box[3]);

	clibwindow* msgwnd = this->open_window(box[0], box[1]-2, box[2], 3);
	
	// write the message on the window
	
	this->outchars(msg_x, y-5, message);

	va_list vl;
	va_start(vl, options);
	for (int index = 0; index < options; index++)
	{
		char* msg = va_arg(vl, char*);
		this->outchars(x+2, y + opt_y + index, msg);
	}
	va_end(vl);
	
	bool close_box = false;
	while(!close_box)
	{
		{for (int i = 0; i<options;i++){this->outchar(x, y+opt_y+i,' ');}}
		this->outchar(x, y+opt_y + cursor_y, (unsigned char)175);

		int k = this->get_key();
		
		switch (k)
		{
		case VK_UP:
			{
				if (cursor_y > 0) cursor_y--;
			} break;
		case VK_DOWN:
			{
				if (cursor_y < options-1) cursor_y++;
			} break;

		case VK_SPACE:
		case VK_RETURN:
			{
				close_box = true;
			} break;
		}
	}
	// close the window
	this->close_window(msgwnd);
	this->close_window(wnd);

	return cursor_y+1;
}

void clib::draw_button(int x, int y, int w, int h, char* caption, unsigned short bgc, unsigned short fgc)
{
	this->set_bgcolor(bgc);
	this->set_fgcolor(fgc);

	for (int cy = 1; cy < h - 1; cy++)
	{
		for (int cx = 1; cx < w - 1; cx++)
		{
			this->outchar(x+cx, y+cy, ' ');
		}
	}

	// draw top and bottom edge
	{for (int cx = 0; cx < w; cx++)
	{
		this->outchar(x+cx, y, (char)205);
		this->outchar(x+cx, y+(h-1), (char)205);
	}}
	
	// draw left and right edge
	{for (int cy = 0; cy < h; cy++)
	{
		this->outchar(x, y+cy, (char)186);
		this->outchar(x+(w-1), y+cy, (char)186);
	}}
	
	// draw corners
	this->outchar(x, y, (char)201);
	this->outchar(x, y+(h-1), (char)200);
	this->outchar(x+(w-1), y, (char)187);
	this->outchar(x+(w-1), y+(h-1), (char)188);

	// draw the caption
	this->outchars(x + 2, y + 1, caption);
}
