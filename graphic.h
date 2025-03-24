#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "defs.h"

struct Graphic{
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    void logErrorAndExit(const char* msg, const char* error);
    void init();
    void prepareScene(SDL_Texture* background);
    void presetScene();
    SDL_Texture loadTexture(const char* filename);
    void renderTexture(SDL_Texture* texture, int x, int y);
    void blitRec(SDL_Texture* texture, SDL_Rect* src, int x, int y);
    void quit();
};


#endif