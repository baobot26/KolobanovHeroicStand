#include "../header/gamestate.h"
#include <iostream>

GameState::GameState(SDL_Renderer* rend, TTF_Font* f, int posX, int posY, int btnWidth, int btnHeight)
    : renderer(rend), font(f), selected(0), x(posX), y(posY), width(btnWidth), height(btnHeight) {}

void GameState::addOption(const std::string& option) {
    options.push_back(option);
}

bool GameState::update(Controller& controller) {
    static Uint32 lastUpdate = 0;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastUpdate < 150) return false; // limit frequency of updates(150ms)
    lastUpdate = currentTime;

    if (controller.getButtonState(SDL_CONTROLLER_BUTTON_DPAD_UP))
        selected = (selected - 1 + options.size()) % options.size();
    if (controller.getButtonState(SDL_CONTROLLER_BUTTON_DPAD_DOWN))
        selected = (selected + 1) % options.size();
    if (controller.getButtonState(SDL_CONTROLLER_BUTTON_Y))
        return true;
    return false;
}

void GameState::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (size_t i = 0; i < options.size(); ++i) {
        SDL_Color textColor = (static_cast<int>(i) == selected) ? SDL_Color{0, 255, 0, 255} : SDL_Color{255, 255, 0, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i].c_str(), textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        int textW, textH;
        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect = {x + (width - textW) / 2, y + static_cast<int>(i * (height + 10)), textW, textH};
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
    SDL_RenderPresent(renderer);
}