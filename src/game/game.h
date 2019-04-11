#ifndef GAME_H
#define GAME_H

#include "engine/platform/platform.h"

class Game {
  public:
    bool initialize ();
    void destroy ();
    void update ();
    void render ();
    Platform* platformPtr;
};

#endif // !GAME_H
