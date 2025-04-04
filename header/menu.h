#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>

class Controller;

class Menu {
    public: // Chuyển selected và options thành public
        SDL_Renderer* renderer;
        TTF_Font* font;
        std::vector<std::string> options;
        int selected;
        int x, y;
        int width, height;
    
        Menu(SDL_Renderer* rend, int posX, int posY, int btnWidth, int btnHeight, const std::string& fontPath);
        ~Menu();
        void addOption(const std::string& option);
        bool update(Controller& controller); // Giữ lại cho menu chính
        void draw();
        int getSelected() const { return selected; }
    };
    
#endif // MENU_H
