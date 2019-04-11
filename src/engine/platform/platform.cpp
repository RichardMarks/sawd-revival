#include "platform.h"

bool Platform::initialize (int screenWidth, int screenHeight) {
  printf("init platform\n");

  if (!al_init()) {
    printf("Could not init allegro\n");
    return false;
  }

  if (!al_install_keyboard()) {
    printf("Could not init keyboard\n");
    return false;
  }

  if (!al_install_mouse()) {
    printf("Could not init mouse\n");
    return false;
  }

  if (!al_install_audio()) {
    printf("Count not init audio\n");
    return false;
  }

  this->timer = al_create_timer(1.0 / 30.0);
  if (!this->timer) {
    printf("Could not init timer\n");
    return false;
  }

  this->queue = al_create_event_queue();
  if (!this->queue) {
    printf("Could not init event queue\n");
    return 1;
  }

  int windowWidth = 1920;
  int windowHeight = 1080;
  al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);

  this->disp = al_create_display(windowWidth, windowHeight);
  if (!this->disp) {
    printf("Could not init display %d x %d\n", windowWidth, windowHeight);
    return false;
  }
  windowWidth = al_get_display_width(disp);
  windowHeight = al_get_display_height(disp);
  printf("Created display %d x %d\n", windowWidth, windowHeight);

  this->displayBuffer = al_create_bitmap(screenWidth, screenHeight);
  if (!this->displayBuffer) {
    printf("Could not create display buffer %d x %d", screenWidth, screenHeight);
    return false;
  }

  if (!al_init_font_addon()) {
    printf("Could not init font addon\n");
    return false;
  }

  if (!al_init_ttf_addon()) {
    printf("Could not init ttf addon\n");
    return false;
  }

  if (!al_init_image_addon()) {
    printf("Could not init image addon\n");
    return false;
  }

  if (!al_init_acodec_addon()) {
    printf("Could not init audio codec addon\n");
    return false;
  }

  if (!al_reserve_samples(1)) {
    printf("Could not reserve samples\n");
    return false;
  }

  this->debugFont = al_create_builtin_font();
  if (!this->debugFont) {
    printf("Could not create debug font\n");
    return false;
  }

  this->width = screenWidth;
  this->height = screenHeight;

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

  this->bufferX = scaleX;
  this->bufferY = scaleY;
  this->bufferW = scaleWidth;
  this->bufferH = scaleHeight;
  this->scale = scale;

  al_register_event_source(this->queue, al_get_keyboard_event_source());
  al_register_event_source(this->queue, al_get_mouse_event_source());
  al_register_event_source(this->queue, al_get_display_event_source(this->disp));
  al_register_event_source(this->queue, al_get_timer_event_source(this->timer));

  memset(this->key, 0, sizeof(this->key));

  this->frames = 0;
  this->running = true;
  return true;
}

void Platform::destroy () {
  printf("destroy platform\n");

  if (this->debugFont) {
    al_destroy_font(this->debugFont);
    this->debugFont = NULL;
  }

  if (this->displayBuffer) {
    al_destroy_bitmap(this->displayBuffer);
    this->displayBuffer = NULL;
  }

  if (this->disp) {
    al_destroy_display(this->disp);
    this->disp = NULL;
  }

  if (this->timer) {
    al_destroy_timer(this->timer);
    this->timer = NULL;
  }

  if (this->queue) {
    al_destroy_event_queue(this->queue);
    this->queue = NULL;
  }
}

void Platform::openWindow () {
  al_start_timer(this->timer);
  this->redraw = false;
}

bool Platform::isReadyToUpdate () {
  return this->ticked;
}

void Platform::update () {
  // printf("platform update\n");

  al_wait_for_event(this->queue, &this->event);

  this->ticked = false;
  switch (this->event.type) {
    case ALLEGRO_EVENT_TIMER: {
      this->ticked = true;
      this->redraw = true;

      // clear key buffer
      for (int i = 0; i < ALLEGRO_KEY_MAX; i += 1) {
        this->key[i] &= 1;
      }
    } break;

    case ALLEGRO_EVENT_MOUSE_AXES:
      this->mouseX = (this->event.mouse.x - this->bufferX) * 1 / this->scale;
      this->mouseY = (this->event.mouse.y - this->bufferY) * 1 / this->scale;
      break;

    case ALLEGRO_EVENT_KEY_DOWN:
      this->key[this->event.keyboard.keycode] = 1 | 2;
      break;

    case ALLEGRO_EVENT_KEY_UP:
      this->key[this->event.keyboard.keycode] &= 2;
      break;

    case ALLEGRO_EVENT_DISPLAY_CLOSE:
      this->running = false;
      break;
  }
}

bool Platform::isReadyToDraw () {
  if (this->redraw && al_is_event_queue_empty(this->queue)) {
    this->redraw = false;
    al_set_target_bitmap(this->displayBuffer);
    al_clear_to_color(al_map_rgb(0, 0, 0));

    return true;
  }
  return false;
}

void Platform::render () {
  // printf("platform render\n");

  al_draw_text(this->debugFont, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello, World");

  al_set_target_backbuffer(this->disp);
  al_clear_to_color(al_map_rgb(0x30, 0x30, 0x30));
  al_draw_scaled_bitmap(
    this->displayBuffer,
    0,
    0,
    this->width,
    this->height,
    this->bufferX,
    this->bufferY,
    this->bufferW,
    this->bufferH,
    0
  );
  al_flip_display();
}
