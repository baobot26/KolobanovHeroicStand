#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "defs.h"

struct Graphic{
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    void logErrorAndExit(const char* msg, const char* error);
    void init();
    void prepareScene();
    void prepareScene(SDL_Texture* background);
    void presetScene();
    Mix_Music* loadMusic(const char* filename);
    void play(Mix_Music* gMusic);
    Mix_Chunk* loadSound(const char* filename);
    void play(Mix_Chunk* gChunk);
    TTF_Font* loadFont(const char* filename, int size);
    SDL_Texture* renderText(const char* msg, TTF_Font* font, SDL_Color color);
    SDL_Texture* loadTexture(const char* filename);
    void renderTexture(SDL_Texture* texture, int x, int y);
    void blitRec(SDL_Texture* texture, SDL_Rect* src, int x, int y);
    void quit();
};


#endif