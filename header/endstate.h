#ifndef ENDSTATE_H
#define ENDSTATE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "../header/controller.h"

class EndState {
public:
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<std::string> options;
    int selected;
    int x, y, width, height;
    int score;

    EndState(SDL_Renderer* rend, TTF_Font* f, int posX, int posY, int btnWidth, int btnHeight);
    void addOption(const std::string& option);
    int update(Controller& controller); // return -1 or other values
    void draw();
    int getSelected() const { return selected; }
    void setScore(int s) { score = s; }
};

#endif