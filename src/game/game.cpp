#include "game.h"

bool Game::initialize () {
  return true;
}

void Game::destroy () {

}

void Game::update () {
  if (this->platformPtr->key[ALLEGRO_KEY_ESCAPE]) {
    this->platformPtr->running = false;
  }
}

void Game::render () {

}
