#ifndef GAMESTATES_H
#define GAMESTATES_H

// a game state consists of 6 functions per state

// statename_create  -- allocate anything needed in the state. called once at start of program
// statename_destroy -- deallocate anything allocated in xx_create. called once at end of program
// statename_enter -- called when entering this state
// statename_exit -- called when exiting this state
// statename_update -- called once per timer tick
// statename_render -- called once per redraw

typedef struct t_GAMESTATE
{
  void (*Create)(void *, void *);
  void (*Destroy)(void *, void *);
  void (*Enter)(void *, void *);
  void (*Exit)(void *, void *);
  void (*Update)(void *, void *);
  void (*Render)(void *, void *);
} _GAMESTATE;

extern void gstitle_setup();
extern void gstitle_create(void *game, void *platform);
extern void gstitle_destroy(void *game, void *platform);
extern void gstitle_enter(void *game, void *platform);
extern void gstitle_exit(void *game, void *platform);
extern void gstitle_update(void *game, void *platform);
extern void gstitle_render(void *game, void *platform);
extern _GAMESTATE gstitle;

extern void gsmainmenu_setup();
extern void gsmainmenu_create(void *game, void *platform);
extern void gsmainmenu_destroy(void *game, void *platform);
extern void gsmainmenu_enter(void *game, void *platform);
extern void gsmainmenu_exit(void *game, void *platform);
extern void gsmainmenu_update(void *game, void *platform);
extern void gsmainmenu_render(void *game, void *platform);
extern _GAMESTATE gsmainmenu;

#endif // !GAMESTATES_H
