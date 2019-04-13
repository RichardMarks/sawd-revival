#include "platform.h"

bool Platform::initialize(int screenWidth, int screenHeight)
{
  printf("init platform\n");

  if (!al_init())
  {
    printf("Could not init allegro\n");
    return false;
  }

  if (!al_install_keyboard())
  {
    printf("Could not init keyboard\n");
    return false;
  }

  if (!al_install_mouse())
  {
    printf("Could not init mouse\n");
    return false;
  }

  if (!al_install_audio())
  {
    printf("Count not init audio\n");
    return false;
  }

  timer = al_create_timer(1.0 / 30.0);
  if (!timer)
  {
    printf("Could not init timer\n");
    return false;
  }

  queue = al_create_event_queue();
  if (!queue)
  {
    printf("Could not init event queue\n");
    return 1;
  }

  int windowWidth = 1920;
  int windowHeight = 1080;
  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

  disp = al_create_display(windowWidth, windowHeight);
  if (!disp)
  {
    printf("Could not init display %d x %d\n", windowWidth, windowHeight);
    return false;
  }
  windowWidth = al_get_display_width(disp);
  windowHeight = al_get_display_height(disp);
  printf("Created display %d x %d\n", windowWidth, windowHeight);

  displayBuffer = al_create_bitmap(screenWidth, screenHeight);
  if (!displayBuffer)
  {
    printf("Could not create display buffer %d x %d", screenWidth, screenHeight);
    return false;
  }

  if (!al_init_font_addon())
  {
    printf("Could not init font addon\n");
    return false;
  }

  if (!al_init_ttf_addon())
  {
    printf("Could not init ttf addon\n");
    return false;
  }

  if (!al_init_image_addon())
  {
    printf("Could not init image addon\n");
    return false;
  }

  if (!al_init_acodec_addon())
  {
    printf("Could not init audio codec addon\n");
    return false;
  }

  if (!al_reserve_samples(1))
  {
    printf("Could not reserve samples\n");
    return false;
  }

  debugFont = al_create_builtin_font();
  if (!debugFont)
  {
    printf("Could not create debug font\n");
    return false;
  }

  width = screenWidth;
  height = screenHeight;

  // float sx = windowWidth / screenWidth;
  // float sy = windowHeight / screenHeight;
  // float scale = sx < sy ? sx : sy;
  // float scaleWidth = screenWidth * scale;
  // float scaleHeight = screenHeight * scale;
  // float scaleX = (windowWidth - scaleWidth) * 0.5f;
  // float scaleY = (windowHeight - scaleHeight) * 0.5f;

  float aspectRatio = (float)screenWidth / (float)screenHeight;
  float scaleWidth = (float)windowHeight * aspectRatio;
  float scaleHeight = windowHeight;
  float scaleX = (windowWidth - scaleWidth) * 0.5f;
  float scaleY = (windowHeight - scaleHeight) * 0.5f;

  bufferX = scaleX;
  bufferY = scaleY;
  bufferW = scaleWidth;
  bufferH = scaleHeight;
  scale = scale;

  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  memset(key, 0, sizeof(key));

  // init palette
  // https://devblogs.microsoft.com/commandline/updating-the-windows-console-colors/
#ifdef LEGACY_COLORS
#define BLACK al_map_rgb(0, 0, 0)
#define DARK_BLUE al_map_rgb(0, 0, 128)
#define DARK_GREEN al_map_rgb(0, 128, 0)
#define DARK_CYAN al_map_rgb(0, 128, 128)
#define DARK_RED al_map_rgb(128, 0, 0)
#define DARK_MAGENTA al_map_rgb(128, 0, 128)
#define DARK_YELLOW al_map_rgb(128, 128, 0)
#define DARK_WHITE al_map_rgb(192, 192, 192)
#define BRIGHT_BLACK al_map_rgb(128, 128, 128)
#define BRIGHT_BLUE al_map_rgb(0, 0, 255)
#define BRIGHT_GREEN al_map_rgb(0, 255, 0)
#define BRIGHT_CYAN al_map_rgb(0, 255, 255)
#define BRIGHT_RED al_map_rgb(255, 0, 0)
#define BRIGHT_MAGENTA al_map_rgb(255, 0, 255)
#define BRIGHT_YELLOW al_map_rgb(255, 255, 0)
#define WHITE al_map_rgb(255, 255, 255)
#else
#define BLACK al_map_rgba(12, 12, 12, 128)
#define DARK_BLUE al_map_rgba(0, 55, 218, 128)
#define DARK_GREEN al_map_rgba(19, 161, 14, 128)
#define DARK_CYAN al_map_rgba(58, 150, 221, 128)
#define DARK_RED al_map_rgba(197, 15, 13, 128)
#define DARK_MAGENTA al_map_rgba(136, 23, 152, 128)
#define DARK_YELLOW al_map_rgba(193, 156, 0, 128)
#define DARK_WHITE al_map_rgba(204, 204, 204, 128)
#define BRIGHT_BLACK al_map_rgba(118, 118, 118, 128)
#define BRIGHT_BLUE al_map_rgba(59, 120, 255, 128)
#define BRIGHT_GREEN al_map_rgba(22, 198, 12, 128)
#define BRIGHT_CYAN al_map_rgba(97, 214, 214, 128)
#define BRIGHT_RED al_map_rgba(231, 72, 86, 128)
#define BRIGHT_MAGENTA al_map_rgba(180, 0, 158, 128)
#define BRIGHT_YELLOW al_map_rgba(249, 241, 165, 128)
#define WHITE al_map_rgba(242, 242, 242, 128)
#endif // LEGACY_COLORS

  palette[0] = BLACK;
  palette[1] = DARK_BLUE;
  palette[2] = DARK_GREEN;
  palette[3] = DARK_CYAN;
  palette[4] = DARK_RED;
  palette[5] = DARK_MAGENTA;
  palette[6] = DARK_YELLOW;
  palette[7] = DARK_WHITE;
  palette[8] = BRIGHT_BLACK;
  palette[9] = BRIGHT_BLUE;
  palette[10] = BRIGHT_GREEN;
  palette[11] = BRIGHT_CYAN;
  palette[12] = BRIGHT_RED;
  palette[13] = BRIGHT_MAGENTA;
  palette[14] = BRIGHT_YELLOW;
  palette[15] = WHITE;

  cl = new clib();
  clui.cl_ = cl;

  cl->screen80x50();

  cl->set_bgcolor(0);
  cl->set_fgcolor(1|2|8);
  cl->cls();

  char* title[] =
  {
    "CCPS Solutions Presents",
    "  ",
    " SSSS  AAA  W   W DDDD",
    "S     A   A W   W D   D",
    " SSS  AAAAA W W W D   D",
    "    S A   A W W W D   D",
    "SSSS  A   A WW WW DDDD",
    "  ",
    "  ",
    "Simple ASCII Walk-around Demo",
    "  ",
    "RPGDX 2008 ASCII Mini-RPG Contest",
    "  ",
    "http://www.ccpssolutions.com"
  };

  for (int n = 0; n < 14; n++)
  {
    // cl->set_bgcolor(n);
    cl->set_fgcolor(15 - n);
    int x = 40 - (strlen(title[n]) / 2);
    cl->outchars(x, 4 + n, title[n]);
  }

  // clibwindow* wnd = cl->open_window(0, 0, 40, 25, 4, 1|2|4|8);

  // // print the console buffer to stdout to test
  // for (int y = 0; y < 50; y += 1) {
  //   for (int x = 0; x < 80; x += 1) {
  //     printf("%c", ConsoleWindowBuffer.Data[x + y * 80].Char);
  //   }
  //   printf("\n");
  // }

  // cl->close_window(wnd);

  pcFontBitmap = al_load_bitmap("assets/font.png");
  if (!pcFontBitmap)
  {
    printf("Unable to load font.png\n");
    return false;
  }

  memset(&p1, 0, sizeof(p1));

  frames = 0;
  running = true;
  return true;
}

void Platform::destroy()
{
  printf("destroy platform\n");

  if (pcFontBitmap)
  {
    al_destroy_bitmap(pcFontBitmap);
    pcFontBitmap = NULL;
  }

  if (cl)
  {
    delete cl;
    cl = 0;
  }

  if (debugFont)
  {
    al_destroy_font(debugFont);
    debugFont = NULL;
  }

  if (displayBuffer)
  {
    al_destroy_bitmap(displayBuffer);
    displayBuffer = NULL;
  }

  if (disp)
  {
    al_destroy_display(disp);
    disp = NULL;
  }

  if (timer)
  {
    al_destroy_timer(timer);
    timer = NULL;
  }

  if (queue)
  {
    al_destroy_event_queue(queue);
    queue = NULL;
  }
}

void Platform::openWindow()
{
  al_start_timer(timer);
  redraw = false;
}

bool Platform::isReadyToUpdate()
{
  return ticked;
}

void Platform::update()
{
  // printf("platform update\n");

  al_wait_for_event(queue, &event);

  ticked = false;
  switch (event.type)
  {
  case ALLEGRO_EVENT_TIMER:
  {
    ticked = true;
    redraw = true;

    p1.up = key[ALLEGRO_KEY_UP];
    p1.down = key[ALLEGRO_KEY_DOWN];
    p1.left = key[ALLEGRO_KEY_LEFT];
    p1.right = key[ALLEGRO_KEY_RIGHT];
    p1.a = key[ALLEGRO_KEY_Z];
    p1.b = key[ALLEGRO_KEY_X];
    p1.c = key[ALLEGRO_KEY_C];

    // clear key buffer
    for (int i = 0; i < ALLEGRO_KEY_MAX; i += 1)
    {
      key[i] &= 1;
    }
  }
  break;

  case ALLEGRO_EVENT_MOUSE_AXES:
    mouseX = (event.mouse.x - bufferX) * 1 / scale;
    mouseY = (event.mouse.y - bufferY) * 1 / scale;
    break;

  case ALLEGRO_EVENT_KEY_DOWN: {
    switch (event.keyboard.keycode) {
      case ALLEGRO_KEY_UP: p1.up_p = true; break;
      case ALLEGRO_KEY_DOWN: p1.down_p = true; break;
      case ALLEGRO_KEY_LEFT: p1.left_p = true; break;
      case ALLEGRO_KEY_RIGHT: p1.right_p = true; break;
      case ALLEGRO_KEY_Z: p1.a_p = true; break;
      case ALLEGRO_KEY_X: p1.b_p = true; break;
      case ALLEGRO_KEY_C: p1.c_p = true; break;
      default: break;
    }
    key[event.keyboard.keycode] = 1 | 2;
  } break;

  case ALLEGRO_EVENT_KEY_UP: {
    switch (event.keyboard.keycode) {
      case ALLEGRO_KEY_UP: p1.up_p = false; break;
      case ALLEGRO_KEY_DOWN: p1.down_p = false; break;
      case ALLEGRO_KEY_LEFT: p1.left_p = false; break;
      case ALLEGRO_KEY_RIGHT: p1.right_p = false; break;
      case ALLEGRO_KEY_Z: p1.a_p = false; break;
      case ALLEGRO_KEY_X: p1.b_p = false; break;
      case ALLEGRO_KEY_C: p1.c_p = false; break;
      default: break;
    }
    key[event.keyboard.keycode] &= 2;
  } break;

  case ALLEGRO_EVENT_DISPLAY_CLOSE:
    running = false;
    break;
  }
}

bool Platform::isReadyToDraw()
{
  if (redraw && al_is_event_queue_empty(queue))
  {
    redraw = false;
    al_set_target_bitmap(displayBuffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    return true;
  }
  return false;
}

void Platform::render()
{
  // printf("platform render\n");

  copyConsoleBufferToDisplayBuffer();
  // for (int i = 0; i < 16; i += 1)
  // {
  //   al_draw_text(debugFont, palette[i], 0, 8 * i, 0, "Hello, World");
  // }

  al_set_target_backbuffer(disp);
  al_clear_to_color(al_map_rgb(0x30, 0x30, 0x30));
  al_draw_scaled_bitmap(
      displayBuffer,
      0,
      0,
      width,
      height,
      bufferX,
      bufferY,
      bufferW,
      bufferH,
      0);
  al_flip_display();
}

void Platform::drawPCFontCharacter(float x, float y, unsigned char character, unsigned short attributes)
{
  // find appropriate glyph in pc font bitmap
  int lettersPerRow = 16;
  int glyphWidth = 8;
  int glyphHeight = 8;
  int glyphX = (int)((int)character / lettersPerRow) * glyphWidth;
  int glyphY = (int)((int)character % lettersPerRow) * glyphHeight;

  // use a solid fill character for the background
  unsigned char solid = 219;
  int solidX = 104;
  int solidY = 88;

  int fgc = 0;

  // solve the foreground color
  if (attributes & FOREGROUND_RED) { fgc |= 4; }
  if (attributes & FOREGROUND_GREEN) { fgc |= 2; }
  if (attributes & FOREGROUND_BLUE) { fgc |= 1; }
  if (attributes & FOREGROUND_INTENSITY) { fgc |= 8; }

  // special case: if the character is 219
  // only draw the foreground colored one,
  // as the background will not be seen anyway
  if (solid == character) {
    al_draw_tinted_bitmap_region(
      pcFontBitmap,
      palette[fgc],
      solidX,
      solidY,
      glyphWidth,
      glyphHeight,
      x,
      y,
      0
    );
  } else {
    // common case: draw solid with bgc and character with fgc
    int bgc = 0;

    // solve the background color
    if (attributes & BACKGROUND_RED) { bgc |= 4; }
    if (attributes & BACKGROUND_GREEN) { bgc |= 2; }
    if (attributes & BACKGROUND_BLUE) { bgc |= 1; }
    if (attributes & BACKGROUND_INTENSITY) { bgc |= 8; }

    al_draw_tinted_bitmap_region(
      pcFontBitmap,
      palette[bgc],
      solidX,
      solidY,
      glyphWidth,
      glyphHeight,
      x,
      y,
      0
    );

    al_draw_tinted_bitmap_region(
      pcFontBitmap,
      palette[fgc],
      glyphX,
      glyphY,
      glyphWidth,
      glyphHeight,
      x,
      y,
      0
    );
  }
}

void Platform::copyConsoleBufferToDisplayBuffer()
{
  int consoleBufferWidth = ConsoleWindowInfo.BufferSize.X;
  int consoleBufferHeight = ConsoleWindowInfo.BufferSize.Y;
  for (int row = 0; row < consoleBufferHeight; row += 1)
  {
    float y = row * 8;
    for (int column = 0; column < consoleBufferWidth; column += 1)
    {
      int index = column + row * consoleBufferWidth;
      _CHAR_INFO &cell = ConsoleWindowBuffer.Data[index];
      float x = column * 8;
      drawPCFontCharacter(x, y, cell.Char, cell.Attributes);
    }
  }
}
