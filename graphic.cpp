#include "graphic.h"
void Graphic::logErrorAndExit(const char* msg, const char* error){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}
void Graphic::init(){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        logErrorAndExit("SDL Init", SDL_GetError());
    }
    else{
        gWindow = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL) logErrorAndExit("Create Window", SDL_GetError());
        if(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) logErrorAndExit("SDL_image error:", IMG_GetError());
        gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(gRenderer == NULL) logErrorAndExit("Create Renderer", SDL_GetError());
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) logErrorAndExit("SDL_mixer error:", Mix_GetError());
    }
    
}
void Graphic::prepareScene(){
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
}
void Graphic::prepareScene(SDL_Texture* background){
    SDL_RenderClear(gRenderer);
    SDL_RenderCopy(gRenderer, background, NULL, NULL);
}
void Graphic::presetScene(){
    SDL_RenderPresent(gRenderer);
}
SDL_Texture* Graphic::loadTexture(const char* filename){
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    SDL_Texture* texture = IMG_LoadTexture(gRenderer, filename);
    if(texture == NULL) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
    return texture;
}
void Graphic::renderTexture(SDL_Texture* texture, int x, int y){
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
    SDL_RenderCopy(gRenderer, texture, NULL, &dest);
}
void Graphic::blitRec(SDL_Texture* texture, SDL_Rect* src, int x, int y){
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;
    SDL_RenderCopy(gRenderer, texture, src, &dest);
}
void Graphic::quit(){
    Mix_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}
Mix_Music* Graphic::loadMusic(const char* filename){
    Mix_Music* gMusic = Mix_LoadMUS(filename);
    if(gMusic == NULL) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "SDL_mixer error: %s", Mix_GetError());
    return gMusic;
}
void Graphic::play(Mix_Music* gMusic){
    if(gMusic == NULL) return;
    if(Mix_PlayingMusic() == 0){
        Mix_PlayMusic(gMusic, -1);
    }
    else if(Mix_PausedMusic() == 1){
        Mix_ResumeMusic();
    }
}
Mix_Chunk* Graphic::loadSound(const char* filename){
    Mix_Chunk* gChunk = Mix_LoadWAV(filename);
    if(gChunk == NULL) SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "SDL_mixer error: %s", Mix_GetError());
}
void Graphic::play(Mix_Chunk* gChunk){
    if(gChunk != NULL) Mix_PlayChannel(-1, gChunk, 0);
};
