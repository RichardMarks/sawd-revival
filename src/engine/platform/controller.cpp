#include "platform.h"
#include "controller.h"

void reset_controller(_CONTROLLER &controller)
{
  memset(&controller.Buttons, 0, sizeof(controller.Buttons));
  memset(&controller.DPad, 0, sizeof(controller.DPad));
}

void update_controller(_CONTROLLER &controller, unsigned char *key)
{
  reset_controller(controller);

  // update the controller state
  controller.DPad.State[CONTROLLER_DPAD_UP] = (bool)key[ALLEGRO_KEY_UP] || (bool)key[ALLEGRO_KEY_W];
  controller.DPad.State[CONTROLLER_DPAD_DOWN] = (bool)key[ALLEGRO_KEY_DOWN] || (bool)key[ALLEGRO_KEY_S];
  controller.DPad.State[CONTROLLER_DPAD_LEFT] = (bool)key[ALLEGRO_KEY_LEFT] || (bool)key[ALLEGRO_KEY_A];
  controller.DPad.State[CONTROLLER_DPAD_RIGHT] = (bool)key[ALLEGRO_KEY_RIGHT] || (bool)key[ALLEGRO_KEY_D];

  controller.Buttons.State[CONTROLLER_BUTTON_A] = (bool)key[ALLEGRO_KEY_Z] || (bool)key[ALLEGRO_KEY_SPACE];
  controller.Buttons.State[CONTROLLER_BUTTON_B] = (bool)key[ALLEGRO_KEY_X] || (bool)key[ALLEGRO_KEY_ENTER];
  controller.Buttons.State[CONTROLLER_BUTTON_C] = (bool)key[ALLEGRO_KEY_C];
  controller.Buttons.State[CONTROLLER_BUTTON_D] = (bool)key[ALLEGRO_KEY_ESCAPE];
}

bool controller_direction_pressed(_CONTROLLER &controller, int direction)
{
  if (controller.DPad.State[direction] && !controller.DPadPressed.State[direction])
  {
    controller.DPadPressed.State[direction] = true;
    return true;
  }
  else if (!controller.DPad.State[direction])
  {
    controller.DPadPressed.State[direction] = false;
  }
  return false;
}

bool controller_direction_is_down(_CONTROLLER &controller, int direction)
{
  return controller.DPad.State[direction];
}

bool controller_button_pressed(_CONTROLLER &controller, int button)
{
  if (controller.Buttons.State[button] && !controller.ButtonsPressed.State[button])
  {
    controller.ButtonsPressed.State[button] = true;
    return true;
  }
  else if (!controller.Buttons.State[button])
  {
    controller.ButtonsPressed.State[button] = false;
  }
  return false;
}

bool controller_button_is_down(_CONTROLLER &controller, int button)
{
  return controller.Buttons.State[button];
}
