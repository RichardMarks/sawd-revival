#include "engine/platform/platform.h"
#include "engine/clib/clib.h"
#include "game/game.h"

int main(int argc, char *argv[])
{
  Platform platform;
  Game game;

  game.platformPtr = &platform;

  if (!platform.initialize(80 * 8, 50 * 8))
  {
    printf("Unable to initialize platform\n");

    platform.destroy();
    return 1;
  }

  platform.openWindow();

  if (!game.initialize())
  {
    printf("Unable to initialize game\n");
    platform.destroy();
    return 1;
  }

  while (platform.running)
  {
    platform.update();

    if (platform.isReadyToUpdate())
    {

      // // temporary handling for keyboard controls for ui

      // if (platform.p1.c_p)
      // {
      //   // clui.show_message_centered(3, "This is a test", "of the clui library", "show_message_centered");
      //   clui.show_choice("Does this work?");
      // }

      // clui.update(
      //     platform.p1.up_p,
      //     platform.p1.down_p,
      //     platform.p1.left_p,
      //     platform.p1.right_p,
      //     platform.p1.a_p,
      //     platform.p1.b_p,
      //     platform.p1.c_p);

      // // does clui have a choice value for us?
      // if (clui.has_choice_value())
      // {
      //   int choice = clui.get_choice_value();

      //   if (choice)
      //   {
      //     printf("picked YES\n");
      //   }
      //   else
      //   {
      //     printf("picked NO\n");
      //   }
      // }

      game.update();
    }

    if (platform.isReadyToDraw())
    {
      game.render();
      clui.render();
      platform.render();
    }
  }

  game.destroy();

  platform.destroy();

  return 0;
}
