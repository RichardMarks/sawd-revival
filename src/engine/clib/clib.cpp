#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <memory>

#include "clib.h"

clibui clui;

_CONSOLE_WINDOW_BUFFER ConsoleWindowBuffer;
_CONSOLE_WINDOW_INFO ConsoleWindowInfo;

#define STD_OUTPUT_HANDLE 0xDEADBEEF

unsigned int GetStdHandle(unsigned int id)
{
  return STD_OUTPUT_HANDLE;
}

int SetConsoleWindowInfo(unsigned int handle, int absolute, _SMALL_RECT *window)
{
  if (!window)
  {
    return 0;
  }
  if (handle == STD_OUTPUT_HANDLE)
  {
    if (absolute)
    {
      ConsoleWindowInfo.SizePos.Left = window->Left;
      ConsoleWindowInfo.SizePos.Top = window->Top;
      ConsoleWindowInfo.SizePos.Right = window->Right;
      ConsoleWindowInfo.SizePos.Bottom = window->Bottom;
    }
    else
    {
      ConsoleWindowInfo.SizePos.Left += window->Left;
      ConsoleWindowInfo.SizePos.Top += window->Top;
      ConsoleWindowInfo.SizePos.Right += window->Right;
      ConsoleWindowInfo.SizePos.Bottom += window->Bottom;
    }
  }
  else
  {
    return 0;
  }
  return 1;
}

int SetConsoleScreenBufferSize(unsigned int handle, _COORD *size)
{
  if (!size)
  {
    return 0;
  }
  if (handle == STD_OUTPUT_HANDLE)
  {
    ConsoleWindowInfo.BufferSize.X = size->X;
    ConsoleWindowInfo.BufferSize.Y = size->Y;

    if (ConsoleWindowBuffer.Data != 0)
    {
      delete[] ConsoleWindowBuffer.Data;
      ConsoleWindowBuffer.Data = 0;
      ConsoleWindowBuffer.Size = ConsoleWindowInfo.BufferSize.X * ConsoleWindowInfo.BufferSize.Y;
      ConsoleWindowBuffer.Data = new _CHAR_INFO[ConsoleWindowBuffer.Size];
      memset(ConsoleWindowBuffer.Data, 0, sizeof(_CHAR_INFO) * ConsoleWindowBuffer.Size);
    }
    else
    {
      ConsoleWindowBuffer.Size = ConsoleWindowInfo.BufferSize.X * ConsoleWindowInfo.BufferSize.Y;
      ConsoleWindowBuffer.Data = new _CHAR_INFO[ConsoleWindowBuffer.Size];
      memset(ConsoleWindowBuffer.Data, 0, sizeof(_CHAR_INFO) * ConsoleWindowBuffer.Size);
    }
  }
  else
  {
    return 0;
  }
  return 1;
}

int SetConsoleTextAttribute(void *handle, unsigned short attributes)
{
  ConsoleWindowInfo.TextAttribute = attributes;
  return 1;
}

int FillConsoleOutputAttribute(void *handle, unsigned short attribute, unsigned int length, _COORD &coord, unsigned long *written)
{
  int startIndex = coord.X + coord.Y * ConsoleWindowInfo.BufferSize.X;
  int endIndex = startIndex + length;
  if (endIndex >= ConsoleWindowBuffer.Size)
  {
    length -= ConsoleWindowBuffer.Size - endIndex;
  }
  *written = 0;
  for (int i = 0; i < length; i += 1)
  {
    ConsoleWindowBuffer.Data[startIndex + i].Attributes = attribute;
    *written += 1;
  }
  return 1;
}

int FillConsoleOutputCharacter(void *handle, unsigned char character, unsigned int length, _COORD &coord, unsigned long *written)
{
  int startIndex = coord.X + coord.Y * ConsoleWindowInfo.BufferSize.X;
  int endIndex = startIndex + length;
  if (endIndex >= ConsoleWindowBuffer.Size)
  {
    length -= ConsoleWindowBuffer.Size - endIndex;
  }
  *written = 0;
  for (int i = 0; i < length; i += 1)
  {
    ConsoleWindowBuffer.Data[startIndex + i].Char = character;
    *written += 1;
  }
  return 1;
}

int SetConsoleCursorPosition(void *handle, _COORD &coord)
{
  if (coord.X < 0 || coord.Y < 0 || coord.X >= ConsoleWindowInfo.BufferSize.X || coord.Y >= ConsoleWindowInfo.BufferSize.Y)
  {
    return 0;
  }
  ConsoleWindowInfo.Cursor.X = coord.X;
  ConsoleWindowInfo.Cursor.Y = coord.Y;
  return 1;
}

int WriteConsole(void *handle, const char *s, unsigned int length, unsigned long *written, void *reserved)
{
  int startIndex = ConsoleWindowInfo.Cursor.X + ConsoleWindowInfo.Cursor.Y * ConsoleWindowInfo.BufferSize.X;
  int endIndex = startIndex + length;
  if (endIndex >= ConsoleWindowBuffer.Size)
  {
    length -= ConsoleWindowBuffer.Size - endIndex;
  }
  char *p = (char *)s;
  int n = 0;
  *written = 0;
  while (*p != '\0' || n != length)
  {
    char character = *p;
    ConsoleWindowBuffer.Data[startIndex + n].Char = character;
    ConsoleWindowBuffer.Data[startIndex + n].Attributes = ConsoleWindowInfo.TextAttribute;
    ConsoleWindowInfo.Cursor.X += 1;
    if (ConsoleWindowInfo.Cursor.X >= ConsoleWindowInfo.BufferSize.X)
    {
      ConsoleWindowInfo.Cursor.X = 0;
      ConsoleWindowInfo.Cursor.Y += 1;
    }
    p += 1;
    n += 1;
    *written += 1;
  }
  return 1;
}

int ReadConsoleOutput(void *handle, _CHAR_INFO *buffer, _COORD &size, _COORD &coord, _SMALL_RECT *region)
{
  if (!buffer || !region)
  {
    return 0;
  }
  for (int y = 0; y < size.Y; y += 1)
  {
    for (int x = 0; x < size.X; x += 1)
    {
      int srcIndex = x + region->Left + ((y + region->Top) * ConsoleWindowInfo.BufferSize.X);
      if (srcIndex >= ConsoleWindowBuffer.Size)
      {
        return 0;
      }
      int dstIndex = x + coord.X + ((y + coord.Y) * size.X);
      if (dstIndex >= size.X * size.Y)
      {
        return 0;
      }
      _CHAR_INFO &source = ConsoleWindowBuffer.Data[srcIndex];
      _CHAR_INFO &dest = buffer[dstIndex];
      dest.Char = source.Char;
      dest.Attributes = source.Attributes;
    }
  }
  return 1;
}

int WriteConsoleOutput(void *handle, _CHAR_INFO *buffer, _COORD &size, _COORD &coord, _SMALL_RECT *region)
{
  if (!buffer || !region)
  {
    return 0;
  }
  for (int y = 0; y < size.Y; y += 1)
  {
    for (int x = 0; x < size.X; x += 1)
    {
      int dstIndex = x + region->Left + ((y + region->Top) * ConsoleWindowInfo.BufferSize.X);
      if (dstIndex >= ConsoleWindowBuffer.Size)
      {
        return 0;
      }
      int srcIndex = x + coord.X + ((y + coord.Y) * size.X);
      if (srcIndex >= size.X * size.Y)
      {
        return 0;
      }
      _CHAR_INFO &source = buffer[srcIndex];
      _CHAR_INFO &dest = ConsoleWindowBuffer.Data[dstIndex];
      dest.Char = source.Char;
      dest.Attributes = source.Attributes;
    }
  }
  return 1;
}

clibwindow::clibwindow(int x, int y, int w, int h)
{
  data_[0] = x;
  data_[1] = y;
  data_[2] = w;
  data_[3] = h;
  buffer_ = new _CHAR_INFO[w * h];
}

clibwindow::~clibwindow()
{
  delete[] buffer_;
  buffer_ = 0;
  data_[0] = 0;
  data_[1] = 0;
  data_[2] = 0;
  data_[3] = 0;
}

clib::clib() : screen_(0), keyboard_(0), bgc_(0), fgc_(1 | 2 | 4)
{
  ConsoleWindowBuffer.Data = 0;
  memset(&ConsoleWindowInfo, 0, sizeof(ConsoleWindowInfo));
}

clib::~clib()
{
}

void clib::initialize_high_res()
{
}

void clib::screen80x25()
{
  _COORD bsz = {80, 25};
  _SMALL_RECT sz = {0, 0, 79, 24};
  if (!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), 1, &sz))
  {
    printf("Unable to set console window info\n");
  }
  if (!SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), &bsz))
  {
    printf("Unable to set console screen buffer size\n");
  }
}

void clib::screen80x50()
{
  _COORD bsz = {80, 50};
  _SMALL_RECT sz = {0, 0, 79, 49};
  if (!SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), 1, &sz))
  {
    printf("Unable to set console window info\n");
  }
  if (!SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), &bsz))
  {
    printf("Unable to set console screen buffer size\n");
  }
}

void clib::set_bgcolor(unsigned short color)
{
  bgc_ = 0;
  if (color & 1)
  {
    bgc_ |= BACKGROUND_RED;
  }
  if (color & 2)
  {
    bgc_ |= BACKGROUND_GREEN;
  }
  if (color & 4)
  {
    bgc_ |= BACKGROUND_BLUE;
  }
  if (color & 8)
  {
    bgc_ |= BACKGROUND_INTENSITY;
  }
  SetConsoleTextAttribute(screen_, fgc_ | bgc_);
}

void clib::set_fgcolor(unsigned short color)
{
  fgc_ = 0;
  if (color & 1)
  {
    fgc_ |= FOREGROUND_RED;
  }
  if (color & 2)
  {
    fgc_ |= FOREGROUND_GREEN;
  }
  if (color & 4)
  {
    fgc_ |= FOREGROUND_BLUE;
  }
  if (color & 8)
  {
    fgc_ |= FOREGROUND_INTENSITY;
  }
  SetConsoleTextAttribute(screen_, fgc_ | bgc_);
}

void clib::set_xy(int x, int y)
{
  _COORD p;
  p.X = x;
  p.Y = y;
  SetConsoleCursorPosition(screen_, p);
}

void clib::cls()
{
  _COORD st;
  unsigned long bw;
  st.X = 0;
  st.Y = 0;
  FillConsoleOutputAttribute(screen_, fgc_ | bgc_, 80 * 50, st, &bw);
  FillConsoleOutputCharacter(screen_, 32, 80 * 50, st, &bw);
  SetConsoleCursorPosition(screen_, st);
}

void clib::outchar(int x, int y, char c)
{
  unsigned long cw;
  char s[] = {c, '\0'};
  _COORD p;
  p.X = x;
  p.Y = y;
  SetConsoleCursorPosition(screen_, p);
  WriteConsole(screen_, s, strlen(s), &cw, 0);
}

void clib::outchars(int x, int y, char *text)
{
  unsigned long cw;
  _COORD p;
  p.X = x;
  p.Y = y;
  SetConsoleCursorPosition(screen_, p);
  WriteConsole(screen_, text, strlen(text), &cw, 0);
}

void clib::outcharsf(int x, int y, char *text, ...)
{
  unsigned long cw;
  char stringbuffer[512];
  _COORD p;
  p.X = x;
  p.Y = y;
  SetConsoleCursorPosition(screen_, p);

  va_list args;
  va_start(args, text);
  vsprintf(stringbuffer, text, args);
  WriteConsole(screen_, stringbuffer, strlen(stringbuffer), &cw, 0);
  va_end(args);
}

void clib::read(char *buffer, unsigned long bs)
{
  // TODO: character input
}

int clib::get_key()
{
  // TODO: key input
  return 0;
}

clibwindow *clib::open_window(int x, int y, int w, int h, unsigned short bgc, unsigned short fgc)
{
  // before drawing the window, we save the contents of the screen
  // where the window will be drawn so that we can redraw the screen
  // when the window is closed.
  clibwindow *wnd = new clibwindow(x, y, w, h);

  _COORD w_sz;
  _COORD w_p;
  _SMALL_RECT w_b;
  w_p.X = 0;
  w_p.Y = 0;
  w_sz.X = w;
  w_sz.Y = h;
  w_b.Left = x;
  w_b.Top = y;
  w_b.Right = x + w - 1;
  w_b.Bottom = y + h - 1;

  if (!ReadConsoleOutput(screen_, (_CHAR_INFO *)wnd->buffer_, w_sz, w_p, &w_b))
  {
    printf("Unable to read console output\n");
  }

  // draw the window
  this->set_bgcolor(bgc);
  this->set_fgcolor(fgc);

  for (int cy = 1; cy < h - 1; cy += 1)
  {
    for (int cx = 1; cx < w - 1; cx += 1)
    {
      this->outchar(x + cx, y + cy, ' ');
    }
  }

  // draw top and bottom edge
  {
    for (int cx = 0; cx < w; cx += 1)
    {
      this->outchar(x + cx, y, (char)205);
      this->outchar(x + cx, y + (h - 1), (char)205);
    }
  }

  // draw left and right edge
  {
    for (int cy = 0; cy < h; cy += 1)
    {
      this->outchar(x, y + cy, (char)186);
      this->outchar(x + (w - 1), y + cy, (char)186);
    }
  }

  // draw corners
  this->outchar(x, y, (char)201);
  this->outchar(x, y + (h - 1), (char)200);
  this->outchar(x + (w - 1), y, (char)187);
  this->outchar(x + (w - 1), y + (h - 1), (char)188);

  // return the saved window
  return wnd;
}

clibwindow *clib::open_window_noframe(int x, int y, int w, int h, unsigned short bgc, unsigned short fgc)
{
  // before drawing the window, we save the contents of the screen
  // where the window will be drawn so that we can redraw the screen
  // when the window is closed.
  clibwindow *wnd = new clibwindow(x, y, w, h);

  _COORD w_sz;
  _COORD w_p;
  _SMALL_RECT w_b;
  w_p.X = 0;
  w_p.Y = 0;
  w_sz.X = w;
  w_sz.Y = h;
  w_b.Left = x;
  w_b.Top = y;
  w_b.Right = x + w - 1;
  w_b.Bottom = y + h - 1;

  if (!ReadConsoleOutput(screen_, (_CHAR_INFO *)wnd->buffer_, w_sz, w_p, &w_b))
  {
    printf("Unable to read console output\n");
  }

  this->set_bgcolor(bgc);
  this->set_fgcolor(fgc);

  for (int cy = 0; cy < h; cy += 1)
  {
    for (int cx = 0; cx < w; cx += 1)
    {
      this->outchar(x + cx, y + cy, ' ');
    }
  }

  // return the saved window
  return wnd;
}

void clib::close_window(clibwindow *wnd)
{
  _COORD w_sz;
  _COORD w_p;
  _SMALL_RECT w_b;
  w_p.X = 0;
  w_p.Y = 0;
  w_sz.X = wnd->data_[2];
  w_sz.Y = wnd->data_[3];
  w_b.Left = wnd->data_[0];
  w_b.Top = wnd->data_[1];
  w_b.Right = wnd->data_[0] + wnd->data_[2] - 1;
  w_b.Bottom = wnd->data_[1] + wnd->data_[3] - 1;

  if (!WriteConsoleOutput(screen_, (_CHAR_INFO *)wnd->buffer_, w_sz, w_p, &w_b))
  {
    printf("Unable to write console output\n");
  }

  delete wnd;
}

void clib::draw_button(int x, int y, int w, int h, char *caption, unsigned short bgc, unsigned short fgc)
{
  this->set_bgcolor(bgc);
  this->set_fgcolor(fgc);

  for (int cy = 1; cy < h - 1; cy += 1)
  {
    for (int cx = 1; cx < w - 1; cx += 1)
    {
      this->outchar(x + cx, y + cy, ' ');
    }
  }

  // draw top and bottom edge
  {
    for (int cx = 0; cx < w; cx += 1)
    {
      this->outchar(x + cx, y, (char)205);
      this->outchar(x + cx, y + (h - 1), (char)205);
    }
  }

  // draw left and right edge
  {
    for (int cy = 0; cy < h; cy += 1)
    {
      this->outchar(x, y + cy, (char)186);
      this->outchar(x + (w - 1), y + cy, (char)186);
    }
  }

  // draw corners
  this->outchar(x, y, (char)201);
  this->outchar(x, y + (h - 1), (char)200);
  this->outchar(x + (w - 1), y, (char)187);
  this->outchar(x + (w - 1), y + (h - 1), (char)188);

  // draw the caption
  this->outchars(x + 2, y + 1, caption);
}

clibui::clibui()
{
  memset(&info_, 0, sizeof(info_));
}

clibui::~clibui()
{
  close_secondary_window();
  close_primary_window();
}

bool clibui::has_choice_value()
{
  return !info_.Active && info_.Ready;
}

int clibui::get_choice_value()
{
  int choice = info_.Choice;
  info_.Ready = false;
  return choice;
}

void clibui::show_message_centered(int lines, ...)
{
  close_primary_window();

  info_.UIType = UITYPE_MESSAGE_CENTERED;
  info_.Box[0] = 5;
  info_.Box[1] = (25 / 2) - ((lines + 4) / 2);
  info_.Box[2] = 70;
  info_.Box[3] = lines + 4;

  info_.MessageX = info_.Box[0] + 2;
  info_.MessageY = info_.Box[1] + 2;

  open_primary_window();

  va_list vl;
  va_start(vl, lines);
  for (int index = 0; index < lines; index += 1)
  {
    char *message = va_arg(vl, char *);
    cl_->outchars(40 - (strlen(message) / 2), info_.MessageY + index, message);
  }
  va_end(vl);
}

void clibui::show_message(int lines, ...)
{
  close_primary_window();

  info_.UIType = UITYPE_MESSAGE;
  info_.Box[0] = 5;
  info_.Box[1] = (25 / 2) - ((lines + 4) / 2);
  info_.Box[2] = 70;
  info_.Box[3] = lines + 4;

  info_.MessageX = info_.Box[0] + 2;
  info_.MessageY = info_.Box[1] + 2;

  open_primary_window();

  va_list vl;
  va_start(vl, lines);
  for (int index = 0; index < lines; index += 1)
  {
    char *message = va_arg(vl, char *);
    cl_->outchars(info_.MessageX, info_.MessageY + index, message);
  }
  va_end(vl);
}

void clibui::show_choice(char *message)
{
  close_primary_window();

  info_.Choice = 0;
  info_.Ready = false;
  info_.UIType = UITYPE_CHOICE;
  info_.Box[0] = 40 - (34 / 2);
  info_.Box[1] = (25 / 2) - (9 / 2);
  info_.Box[2] = 34;
  info_.Box[3] = 9;

  info_.MessageX = info_.Box[0] + 2;
  info_.MessageY = info_.Box[1] + 2;
  info_.CursorY = 0;
  info_.OptionX = info_.MessageX + (info_.Box[2] / 2) - 5;
  info_.OptionY = 2;
  info_.MessageX -= 1;
  info_.MessageX += (info_.Box[2] / 2) - strlen(message) / 2;

  open_primary_window();

  cl_->outchars(info_.MessageX, info_.MessageY, message);
}

void clibui::show_choice_ex(char *message, int options, ...)
{
  close_secondary_window();
  close_primary_window();

  info_.Choice = 0;
  info_.Ready = false;
  info_.UIType = UITYPE_CHOICE_EX;
  info_.Box[0] = 40 - (34 / 2);
  info_.Box[1] = (25 / 2) - ((options + 4) / 2);
  info_.Box[2] = 34;
  info_.Box[3] = options + 4;

  info_.Box2[0] = info_.Box[0];
  info_.Box2[1] = info_.Box[1] - 2;
  info_.Box2[2] = info_.Box[2];
  info_.Box2[3] = 3;

  info_.MessageX = info_.Box[0] + 2;
  info_.MessageY = info_.Box[1] + 2;
  info_.CursorY = 0;
  info_.OptionY = 0;
  info_.OptionX = info_.MessageX + (info_.Box[2] / 2) - 5;
  info_.Options = options;

  info_.MessageX -= 1;
  info_.MessageX += (info_.Box[2] / 2) - strlen(message) / 2;

  open_primary_window();
  open_secondary_window();

  cl_->outchars(info_.MessageX, info_.MessageY - 5, message);

  va_list vl;
  va_start(vl, options);
  for (int index = 0; index < options; index += 1)
  {
    char *text = va_arg(vl, char *);
    cl_->outchars(info_.MessageX, info_.MessageY + index, text);
  }
  va_end(vl);
}

void clibui::update(bool up, bool down, bool left, bool right, bool a, bool b, bool c)
{
  if (!info_.Active)
  {
    return;
  }

  switch (info_.UIType)
  {
  case UITYPE_MESSAGE_CENTERED:
  {
    if (a || b)
    {
      close_primary_window();
    }
  }
  break;
  case UITYPE_MESSAGE:
  {
    if (a || b)
    {
      close_primary_window();
    }
  }
  break;
  case UITYPE_CHOICE:
  {
    if (up)
    {
      if (info_.CursorY > 0)
      {
        info_.CursorY = 0;
      }
    }
    else if (down)
    {
      if (info_.CursorY < 1)
      {
        info_.CursorY = 1;
      }
    }
    else if (a || b)
    {
      // 1 = yes 0 = no
      info_.Choice = info_.CursorY > 0 ? 0 : 1;
      info_.Ready = true;
      close_primary_window();
    }
  }
  break;
  case UITYPE_CHOICE_EX:
  {
    if (up)
    {
      if (info_.CursorY > 0)
      {
        info_.CursorY -= 1;
      }
    }
    else if (down)
    {
      if (info_.CursorY < info_.Options - 1)
      {
        info_.CursorY += 1;
      }
    }
    else if (a || b)
    {
      // choice is cursor y + 1
      info_.Choice = info_.CursorY + 1;
      info_.Ready = true;
      close_secondary_window();
      close_primary_window();
    }
  }
  break;
  default:
    break;
  }
}

void clibui::render()
{
  if (!info_.Active)
  {
    return;
  }

  switch (info_.UIType)
  {
  case UITYPE_MESSAGE_CENTERED:
  {
  }
  break;
  case UITYPE_MESSAGE:
  {
  }
  break;
  case UITYPE_CHOICE:
  {
    cl_->outchars(info_.OptionX, info_.MessageY + info_.OptionY, (char *)"  Yes");
    cl_->outchars(info_.OptionX, info_.MessageY + info_.OptionY + 1, (char *)"  No ");
    cl_->outchar(info_.OptionX, info_.MessageY + info_.OptionY + info_.CursorY, (char)175);
  }
  break;
  case UITYPE_CHOICE_EX:
  {
    {
      for (int i = 0; i < info_.Options; i += 1)
      {
        cl_->outchar(info_.Box[0] + 2, info_.Box[1] + 2 + info_.OptionY + i, ' ');
      }
      cl_->outchar(info_.Box[0] + 2, info_.Box[1] + 2 + info_.OptionY + info_.CursorY, (char)175);
    }
  }
  break;
  default:
    break;
  }
}

void clibui::open_primary_window()
{
  info_.Window = cl_->open_window(info_.Box[0], info_.Box[1], info_.Box[2], info_.Box[3]);
  info_.Active = true;
  printf("opening primary window\n");
}

void clibui::open_secondary_window()
{
  info_.Window2 = cl_->open_window(info_.Box2[0], info_.Box2[1], info_.Box2[2], info_.Box2[3]);
}

void clibui::close_primary_window()
{
  if (info_.Window != NULL)
  {
    cl_->close_window(info_.Window);
    info_.Window = NULL;
    info_.Active = false;
    printf("closing primary window\n");
  }
}

void clibui::close_secondary_window()
{
  if (info_.Window2 != NULL)
  {
    cl_->close_window(info_.Window2);
    info_.Window2 = NULL;
  }
}
