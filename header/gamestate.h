#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "../header/controller.h"

class GameState {
public:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> options;
    int selected;
    int x, y, width, height;

    GameState(SDL_Renderer* rend, TTF_Font* f, int posX, int posY, int btnWidth, int btnHeight);
    void addOption(const std::string& option);
    bool update(Controller& controller); // Trả về true nếu có lựa chọn
    void draw();
    int getSelected() const { return selected; }
};

#endif