#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <ctime>
#include <string>
#include "../header/music.h"
#include "../header/bullet.h"
#include "../header/tank.h"
#include "../header/enemy.h"
#include "../header/controller.h"
#include "../header/gamestate.h"
#include "../header/pausestate.h"
#include "../header/endstate.h"
#include "../header/game.h"
#include <iostream>
std::string song1 = "assets/music/national_anthem_of_ussr.ogg";
std::string song2 = "assets/music/lets_go.ogg";
std::string song3 = "assets/music/march_of_the_defenders_of_moscow.ogg";
std::string song4 = "assets/music/the_red_army_is_the_strongest.ogg";
std::string song5 = "assets/music/the_sacred_war.ogg";
std::string song6 = "assets/music/farewell_of_slavianka.ogg";

std::string fontPath = "assets/font/Orbitron/static/Orbitron-Regular.ttf";

enum GameMode{
    MENU,
    PLAYING
};
int main(int argc, char* argv[]) {
    initSDL(); // Khởi tạo SDL
    srand(static_cast<unsigned>(time(0)));

    // Tải font
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), 24);
    if (!font) {
        std::cerr << "Lỗi tải font: " << TTF_GetError() << std::endl;
        cleanupSDL();
        return 1;
    }

    // Khởi tạo âm nhạc
    std::vector<std::string> tracks = {song1, song2, song3, song4, song5, song6};
    Music music(tracks);
    music.initMixer();
    music.playPlaylist();

    // Khởi tạo controller
    Controller controller;
    controller.init();
    controller.open();

    // Tạo menu chính
    GameState mainMenu(renderer, font, 300, 200, 200, 50); // renderer là biến toàn cục SDL_Renderer*
    mainMenu.addOption("New Game");
    mainMenu.addOption("Exit");
    GameMode mode = MENU; // Bắt đầu ở menu chính
    bool running = true;
    bool delayActive = false; // Biến kiểm soát trạng thái delay
    Uint32 delayStartTime = 0; // Thời gian bắt đầu delay

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        if (mode == MENU) {
            if (!delayActive) {
                // Cập nhật và vẽ menu chính nếu không trong thời gian delay
                // Giả sử mainMenu là đối tượng GameState quản lý menu
                bool selected = mainMenu.update(controller);
                mainMenu.draw();

                if (selected) {
                    int choice = mainMenu.getSelected();
                    if (choice == 0) { // "New Game"
                        mode = PLAYING;
                    } else if (choice == 1) { // "Exit"
                        running = false;
                    }
                }
            } else {
                // Kiểm tra xem đã hết 5 giây chưa
                Uint32 currentTime = SDL_GetTicks();
                if (currentTime - delayStartTime >= 5000) {
                    delayActive = false; // Hết delay, cho phép tương tác lại
                }
            }
        } else if (mode == PLAYING) {
            // Chạy game (giả định hàm runGame trả về true khi Quit)
            bool result = runGame(controller, music, font);
            if (result == true) { // Người chơi chọn "Quit"
                mode = MENU;
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                mainMenu.draw(); // Vẽ lại menu
                SDL_RenderPresent(renderer);
                delayActive = true; // Kích hoạt delay
                delayStartTime = SDL_GetTicks(); // Ghi lại thời gian bắt đầu
            } else if (result == false) { // Thoát hoàn toàn
                running = false;
            }
        }

        SDL_Delay(16); // Giới hạn FPS
    }

    // Dọn dẹp tài nguyên (giả định đã có)
    return 0;
}