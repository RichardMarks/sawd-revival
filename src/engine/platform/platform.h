#ifndef PLATFORM_H
#define PLATFORM_H

#include <cstdio>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class Platform {
  public:
    bool initialize (int screenWidth, int screenHeight);
    void destroy ();
    void openWindow ();
    void update ();
    void render ();
    bool isReadyToDraw ();
    bool isReadyToUpdate ();

    bool running;
    int frames;
    float width;
    float height;
    float scale;
    float mouseX;
    float mouseY;
    unsigned char key[ALLEGRO_KEY_MAX];

  protected:
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* disp;
    ALLEGRO_BITMAP* displayBuffer;
    ALLEGRO_FONT* debugFont;
    ALLEGRO_EVENT event;

    bool redraw;
    bool ticked;

    float bufferX;
    float bufferY;
    float bufferW;
    float bufferH;
};

#endif // !PLATFORM_H
