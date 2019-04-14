#include "game/game.h"
#include "gamestates.h"

_GAMESTATE gstitle;

void gstitle_setup()
{
  gstitle.Create = &gstitle_create;
  gstitle.Destroy = &gstitle_destroy;
  gstitle.Enter = &gstitle_enter;
  gstitle.Exit = &gstitle_exit;
  gstitle.Update = &gstitle_update;
  gstitle.Render = &gstitle_render;
}

void gstitle_create(void *game, void *platform)
{
}

void gstitle_destroy(void *game, void *platform)
{
}

void gstitle_enter(void *game, void *platform)
{
  clib* cl = ((Platform*)platform)->cl;

  // display title
  cl->set_bgcolor(0);
  cl->set_fgcolor(1|2|4|8);
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
    int x = 40 - (strlen(title[n]) / 2);
    cl->outchars(x, 4 + n, title[n]);
  }
}

void gstitle_exit(void *game, void *platform)
{
  Platform& p = *(Platform*)platform;
  p.cl->cls();
}

void gstitle_update(void *game, void *platform)
{
  Game& g = *(Game*)game;
  Platform& p = *(Platform*)platform;

  if (controller_button_pressed(p.p1, CONTROLLER_BUTTON_A)) {
    g.set_state(&gsmainmenu);
  }
}

void gstitle_render(void *game, void *platform)
{
}
