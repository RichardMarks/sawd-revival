#ifndef CONTROLLER_H
#define CONTROLLER_H

#define CONTROLLER_DPAD_UP 0    // W key or UP arrow key
#define CONTROLLER_DPAD_DOWN 1  // S key or DOWN arrow key
#define CONTROLLER_DPAD_LEFT 2  // A key or LEFT arrow key
#define CONTROLLER_DPAD_RIGHT 3 // D or RIGHT arrow key

#define CONTROLLER_BUTTON_A 0 // Z key
#define CONTROLLER_BUTTON_B 1 // X key
#define CONTROLLER_BUTTON_C 2 // C key
#define CONTROLLER_BUTTON_D 3 // ESCAPE

typedef struct t_CONTROLLER_DPAD
{
  bool State[4];
} _CONTROLLER_DPAD;

typedef struct t_CONTROLLER_BUTTONS
{
  bool State[4];
} _CONTROLLER_BUTTONS;

typedef struct t_CONTROLLER
{
  _CONTROLLER_DPAD DPad;
  _CONTROLLER_DPAD DPadPressed;
  _CONTROLLER_BUTTONS Buttons;
  _CONTROLLER_BUTTONS ButtonsPressed;
} _CONTROLLER;

// if (controller_button_pressed(p1, CONTROLLER_BUTTON_A))
// if (controller_button_is_down(p1, CONTROLLER_BUTTON_B))

extern void reset_controller(_CONTROLLER &controller);
extern void update_controller(_CONTROLLER &controller, unsigned char *key);

extern bool controller_direction_pressed(_CONTROLLER &controller, int direction);
extern bool controller_direction_is_down(_CONTROLLER &controller, int direction);

extern bool controller_button_pressed(_CONTROLLER &controller, int button);
extern bool controller_button_is_down(_CONTROLLER &controller, int button);

#endif // !CONTROLLER_H
