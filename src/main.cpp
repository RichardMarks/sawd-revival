#include "engine/platform/platform.h"
#include "engine/clib/clib.h"
#include "game/game.h"

int main (int argc, char* argv[]) {
  Platform platform;
  Game game;

  game.platformPtr = &platform;

  if (!platform.initialize(80 * 8, 50 * 8)) {
    printf("Unable to initialize platform\n");

    platform.destroy();
    return 1;
  }

  platform.openWindow();

  if (!game.initialize()) {
    printf("Unable to initialize game\n");
    platform.destroy();
    return 1;
  }

  while (platform.running) {
    platform.update();

    if (platform.isReadyToUpdate()) {
      game.update();
    }

    if (platform.isReadyToDraw()) {
      game.render();
      platform.render();
    }
  }

  game.destroy();

  platform.destroy();

  return 0;
}
