#include "../header/menu.h"
#include "../header/controller.h"
#include <iostream>
Menu::Menu(SDL_Renderer* rend, int posX, int posY, int btnWidth, int btnHeight, const std::string& fontPath)
    : renderer(rend), font(nullptr), selected(0), x(posX), y(posY), width(btnWidth), height(btnHeight) {
    if (TTF_Init() == -1) {
        std::cerr << "Lỗi khởi tạo SDL_ttf: " << TTF_GetError() << std::endl;
    }
    font = TTF_OpenFont(fontPath.c_str(), 24);
    if (!font) {
        std::cerr << "Lỗi tải font từ " << fontPath << ": " << TTF_GetError() << std::endl;
    } else {
        std::cout << "Font tải thành công: " << fontPath << std::endl;
    }
}

Menu::~Menu() {
    if (font) {
        TTF_CloseFont(font);
    }
}

void Menu::addOption(const std::string& option) {
    options.push_back(option);
}

bool Menu::update(Controller& controller) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return false;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_UP) selected = (selected - 1 + options.size()) % options.size();
            if (event.key.keysym.sym == SDLK_DOWN) selected = (selected + 1) % options.size();
            if (event.key.keysym.sym == SDLK_RETURN) return true;
        }
        if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) 
                selected = (selected - 1 + options.size()) % options.size();
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) 
                selected = (selected + 1) % options.size();
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y) 
                return true; // Chọn bằng nút Y
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    if (keys[SDL_SCANCODE_UP]) selected = (selected - 1 + options.size()) % options.size();
    if (keys[SDL_SCANCODE_DOWN]) selected = (selected + 1) % options.size();

    // Bỏ joystick Y và trigger R2
    return false;
}

void Menu::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (!font) {
        std::cerr << "Font là nullptr, không thể render text!" << std::endl;
        SDL_RenderPresent(renderer);
        return;
    }

    for (size_t i = 0; i < options.size(); ++i) {
        SDL_Rect button = {x, y + static_cast<int>(i * (height + 10)), width, height};
        
        SDL_Color textColor;
        if (static_cast<int>(i) == selected) {
            textColor.r = 0;
            textColor.g = 255;
            textColor.b = 0;
            textColor.a = 255;
        } else {
            textColor.r = 255;
            textColor.g = 255;
            textColor.b = 0;
            textColor.a = 255;
        }

        SDL_Surface* surface = TTF_RenderText_Solid(font, options[i].c_str(), textColor);
        if (!surface) {
            std::cerr << "Lỗi render text cho '" << options[i] << "': " << TTF_GetError() << std::endl;
            continue;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture) {
            std::cerr << "Lỗi tạo texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            continue;
        }

        int textW, textH;
        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
        SDL_Rect textRect = {x + (width - textW) / 2, y + static_cast<int>(i * (height + 10)) + (height - textH) / 2, textW, textH};
        SDL_RenderCopy(renderer, texture, nullptr, &textRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}