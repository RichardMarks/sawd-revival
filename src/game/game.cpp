#include "gamestates.h"
#include "game.h"

bool Game::initialize()
{
  // setup the gamestate structs

  gstitle_setup();
  gsmainmenu_setup();

  // create the gamestates
  gstitle.Create((void *)this, (void *)platformPtr);
  gsmainmenu.Create((void*)this, (void*)platformPtr);

  // set the initial gamestate
  state = NULL;
  nextState = NULL;
  set_state(&gstitle);

  return true;
}

void Game::destroy()
{
  // destroy the gamestates
  gstitle.Destroy((void *)this, (void *)platformPtr);
  gsmainmenu.Destroy((void*)this, (void*)platformPtr);

  state = NULL;
  nextState = NULL;
}

void Game::update()
{
  if (state != NULL)
  {
    state->Update((void *)this, (void *)platformPtr);
  }

  if (platformPtr->key[ALLEGRO_KEY_ESCAPE])
  {
    platformPtr->running = false;
  }

  // are we changing states?
  if (nextState != NULL)
  {
    // exit current state if there is a current state
    if (state != NULL)
    {
      state->Exit((void *)this, (void *)platformPtr);
    }
    // enter the new state
    nextState->Enter((void *)this, (void *)platformPtr);
    // set the state
    state = nextState;
    nextState = NULL;
  }
}

void Game::render()
{
  if (state != NULL)
  {
    state->Render((void *)this, (void *)platformPtr);
  }
}

void Game::set_state(_GAMESTATE *s)
{
  // if new state is null, we are killing the game. exit the current state and then quit
  if (s == NULL)
  {
    if (state != NULL)
    {
      state->Exit((void *)this, (void *)platformPtr);
    }
    platformPtr->running = false;
    return;
  }
  nextState = s;
}
