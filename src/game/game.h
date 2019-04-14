#ifndef GAME_H
#define GAME_H

#include "engine/platform/platform.h"

struct t_GAMESTATE;

class Game
{
public:
  bool initialize();
  void destroy();
  void update();
  void render();

  void set_state(t_GAMESTATE *s);

  Platform *platformPtr;
  t_GAMESTATE *state;
  t_GAMESTATE *nextState;
};

#endif // !GAME_H
