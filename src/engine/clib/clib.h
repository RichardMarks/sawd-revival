#ifndef CLIB_H
#define CLIB_H

#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_RED 0x0004
#define FOREGROUND_INTENSITY 0x0008
#define BACKGROUND_BLUE 0x0010
#define BACKGROUND_GREEN 0x0020
#define BACKGROUND_RED 0x0040
#define BACKGROUND_INTENSITY 0x0080

typedef struct t_CHAR_INFO
{
  unsigned char Char;
  unsigned short Attributes;
} _CHAR_INFO;

typedef struct t_COORD
{
  signed short X;
  signed short Y;
} _COORD;

typedef struct t_SMALL_RECT
{
  signed short Left;
  signed short Top;
  signed short Right;
  signed short Bottom;
} _SMALL_RECT;

typedef struct t_CONSOLE_WINDOW_BUFFER
{
  _CHAR_INFO *Data;
  unsigned int Size;
} _CONSOLE_WINDOW_BUFFER;

typedef struct t_CONSOLE_WINDOW_INFO
{
  _SMALL_RECT SizePos;
  _COORD BufferSize;
  _COORD Cursor;
  unsigned short TextAttribute;
} _CONSOLE_WINDOW_INFO;

extern _CONSOLE_WINDOW_BUFFER ConsoleWindowBuffer;
extern _CONSOLE_WINDOW_INFO ConsoleWindowInfo;

class clibwindow
{
public:
  clibwindow(int x, int y, int w, int h);
  ~clibwindow();
  int data_[4];
  _CHAR_INFO *buffer_;
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
  void outchars(int x, int y, char *text);
  void outcharsf(int x, int y, char *text, ...);
  void read(char *buffer, unsigned long bs);
  int get_key();
  clibwindow *open_window(int x, int y, int w, int h, unsigned short bgc = 4, unsigned short fgc = 1 | 2 | 4 | 8);
  clibwindow *open_window_noframe(int x, int y, int w, int h, unsigned short bgc = 4, unsigned short fgc = 1 | 2 | 4 | 8);
  void close_window(clibwindow *wnd);
  void draw_button(int x, int y, int w, int h, char *caption, unsigned short bgc = 4, unsigned short fgc = 1 | 2 | 4 | 8);

  void *screen_;
  void *keyboard_;
  unsigned short bgc_;
  unsigned short fgc_;
};

#define UITYPE_MESSAGE_CENTERED 1
#define UITYPE_MESSAGE 2
#define UITYPE_CHOICE 3
#define UITYPE_CHOICE_EX 4

typedef struct t_UI_INFO
{
  bool Active;
  bool Ready;
  int UIType;
  int Choice;
  int CursorX;
  int CursorY;
  int OptionX;
  int OptionY;
  int Options;
  int MessageX;
  int MessageY;
  int Box[4];
  int Box2[4];
  clibwindow *Window;
  clibwindow *Window2;
} _UI_INFO;

class clibui
{
public:
  clibui();
  ~clibui();

  bool has_choice_value();
  int get_choice_value();

  void show_message_centered(int lines, ...);
  void show_message(int lines, ...);
  void show_choice(char *message);
  void show_choice_ex(char *message, int options, ...);

  void update(bool up, bool down, bool left, bool right, bool a, bool b, bool c);
  void render();

  clib *cl_;
  _UI_INFO info_;

protected:
  void open_primary_window();
  void open_secondary_window();
  void close_primary_window();
  void close_secondary_window();
};

extern clibui clui;

#endif // !CLIB_H
