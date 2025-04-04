#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "../header/music.h"
#include "../header/controller.h"
#include "../header/gamestate.h"
#include "../header/pausestate.h"
#include "../header/endstate.h"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

void initSDL();
void cleanupSDL();
bool runGame(Controller& controller, Music& music, TTF_Font* font);

#endif // GAME_H