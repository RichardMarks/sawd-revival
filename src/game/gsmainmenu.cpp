#include "game/game.h"
#include "gamestates.h"

_GAMESTATE gsmainmenu;

const int MAINMENU_OPTION_NEWGAME = 0;
const int MAINMENU_OPTION_LOADGAME = 1;
const int MAINMENU_OPTION_CREDITS = 2;
const int MAINMENU_OPTION_QUITGAME = 3;

typedef struct t_MAINMENUDATA
{
  int ActiveOption;
  int X;
  int Y;
  bool Redraw;
} _MAINMENUDATA;
_MAINMENUDATA MainMenuData;

void gsmainmenu_setup()
{
  gsmainmenu.Create = &gsmainmenu_create;
  gsmainmenu.Destroy = &gsmainmenu_destroy;
  gsmainmenu.Enter = &gsmainmenu_enter;
  gsmainmenu.Exit = &gsmainmenu_exit;
  gsmainmenu.Update = &gsmainmenu_update;
  gsmainmenu.Render = &gsmainmenu_render;
}

void gsmainmenu_create(void *game, void *platform)
{
}

void gsmainmenu_destroy(void *game, void *platform)
{
}

void gsmainmenu_enter(void *game, void *platform)
{
  clib *cl = ((Platform *)platform)->cl;
  cl->set_bgcolor(0);
  cl->set_fgcolor(1 | 2 | 4 | 8);
  cl->cls();

  char *title[] =
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
          "http://www.ccpssolutions.com"};

  for (int n = 0; n < 14; n++)
  {
    int x = 40 - (strlen(title[n]) / 2);
    cl->outchars(x, 4 + n, title[n]);
  }

  MainMenuData.ActiveOption = MAINMENU_OPTION_NEWGAME;
  MainMenuData.X = 40 - 56 / 2;
  MainMenuData.Y = 20;
  MainMenuData.Redraw = true;
}

void gsmainmenu_exit(void *game, void *platform)
{
  clib *cl = ((Platform *)platform)->cl;
  cl->set_bgcolor(0);
  cl->set_fgcolor(1 | 2 | 4 | 8);
  cl->cls();
}

void gsmainmenu_update(void *game, void *platform)
{
  Game &g = *(Game *)game;
  Platform &p = *(Platform *)platform;

  if (clui.info_.Active) {
    if (controller_button_pressed(p.p1, CONTROLLER_BUTTON_A)) {
      clui.update(false, false, false, false, true, true, false);
    }
    return;
  }

  if (controller_direction_pressed(p.p1, CONTROLLER_DPAD_LEFT))
  {
    if (MainMenuData.ActiveOption > MAINMENU_OPTION_NEWGAME)
    {
      MainMenuData.ActiveOption -= 1;
    }
    else if (MainMenuData.ActiveOption == MAINMENU_OPTION_NEWGAME)
    {
      MainMenuData.ActiveOption = MAINMENU_OPTION_QUITGAME;
    }
    MainMenuData.Redraw = true;
  }
  else if (controller_direction_pressed(p.p1, CONTROLLER_DPAD_RIGHT))
  {
    if (MainMenuData.ActiveOption < MAINMENU_OPTION_QUITGAME)
    {
      MainMenuData.ActiveOption += 1;
    }
    else if (MainMenuData.ActiveOption == MAINMENU_OPTION_QUITGAME)
    {
      MainMenuData.ActiveOption = MAINMENU_OPTION_NEWGAME;
    }
    MainMenuData.Redraw = true;
  }
  else if (controller_button_pressed(p.p1, CONTROLLER_BUTTON_A))
  {
    switch (MainMenuData.ActiveOption)
    {
    case MAINMENU_OPTION_NEWGAME:
    {
      // TODO: reset game variables
      // g.reset_game_vars();
      printf("TODO: name character state\n");
      g.set_state(NULL);
    }
    break;
    case MAINMENU_OPTION_LOADGAME:
    {
      printf("TODO: load game screen state\n");
      g.set_state(NULL);
    }
    break;
    case MAINMENU_OPTION_CREDITS:
    {
      if (!clui.info_.Active)
      {
        printf("show credits\n");
        clui.show_message_centered(11,
                                   "All content and code created by Richard Marks.",
                                   "",
                                   "Many thanks to all my friends for their support.",
                                   "",
                                   "Thanks to my wonderful parents, without them I",
                                   "",
                                   "would not be here, nor be the man I am today.",
                                   "",
                                   "Last but not least, thanks to God, for without",
                                   "",
                                   "Him, I would be lost in all that I do.");
      }
    }
    break;
    case MAINMENU_OPTION_QUITGAME:
    {
      g.set_state(NULL);
    }
    break;
    default:
      break;
    }
  }
  else if (controller_button_pressed(p.p1, CONTROLLER_BUTTON_C))
  {
  }
}

void gsmainmenu_render(void *game, void *platform)
{
  // avoid unneccessary draw calls by only redrawing when something changes
  if (!MainMenuData.Redraw)
  {
    return;
  }

  MainMenuData.Redraw = false;

  // draw!
  clib *cl = ((Platform *)platform)->cl;

  char *options[] = {
      "New Game",
      "Load Game",
      " Credits ",
      "Quit Game"};
  int optionX[] = {0, 14, 29, 43};
  int optionW = 13;
  int optionH = 3;

  for (int i = 0; i < 4; i += 1)
  {
    unsigned short bgc = 0;
    unsigned short fgc = 1 | 2 | 4;
    if (MainMenuData.ActiveOption == i)
    {
      bgc = 4;
      fgc = 1 | 2 | 4 | 8;
    }
    cl->draw_button(
        MainMenuData.X + optionX[i],
        MainMenuData.Y,
        optionW,
        optionH,
        options[i],
        bgc,
        fgc);
  }
}
