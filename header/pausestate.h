#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "../header/controller.h"

class PauseState {
public:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> options;
    int selected;
    int x, y, width, height;

    PauseState(SDL_Renderer* rend, TTF_Font* f, int posX, int posY, int btnWidth, int btnHeight);
    void addOption(const std::string& option);
    int update(Controller& controller); // Trả về chỉ số tùy chọn hoặc -1
    void draw();
    int getSelected() const { return selected; }
};

#endif