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
#include "../header/menu.h"
std::string song1 = "assets/music/national_anthem_of_ussr.ogg";
std::string song2 = "assets/music/lets_go.ogg";
std::string song3 = "assets/music/march_of_the_defenders_of_moscow.ogg";
std::string song4 = "assets/music/the_red_army_is_the_strongest.ogg";
std::string song5 = "assets/music/the_sacred_war.ogg";
std::string song6 = "assets/music/farewell_of_slavianka.ogg";
std::string font = "assets/font/Orbitron/static/Orbitron-Regular.ttf";
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void initSDL() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);
    window = SDL_CreateWindow("Tank Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void cleanupSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

bool runGame(Controller& controller, Music& music) {
    Tank tank(0, 300, 5.0f, 50.0f, 30.0f);
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    int spawnTimer = 60;
    bool canFire = true;
    bool paused = false;

    Menu pauseMenu(renderer, 300, 200, 200, 50, font); // Thay bằng đường dẫn font
    pauseMenu.addOption("Resume");
    pauseMenu.addOption("Restart");
    pauseMenu.addOption("Quit");

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
                paused = !paused;
            }
            if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER || 
                    event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
                    paused = !paused; // L1 hoặc R1 bật/tắt menu
                }
                if (paused) { // Xử lý D-Pad và Y khi menu tạm dừng bật
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) 
                        pauseMenu.selected = (pauseMenu.getSelected() - 1 + pauseMenu.options.size()) % pauseMenu.options.size();
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) 
                        pauseMenu.selected = (pauseMenu.getSelected() + 1) % pauseMenu.options.size();
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y) {
                        int choice = pauseMenu.getSelected();
                        if (choice == 0) { // Resume
                            paused = false;
                        } else if (choice == 1) { // Restart
                            for (auto bullet : bullets) delete bullet;
                            bullets.clear();
                            for (auto enemy : enemies) delete enemy;
                            enemies.clear();
                            tank = Tank(0, 300, 5.0f, 50.0f, 30.0f);
                            spawnTimer = 60;
                            canFire = true;
                            paused = false;
                        } else if (choice == 2) { // Quit
                            running = false;
                        }
                    }
                }
            }
        }

        if (paused) {
            pauseMenu.draw(); // Chỉ vẽ menu, không gọi update để tránh xung đột sự kiện
        } else {
            const Uint8* keys = SDL_GetKeyboardState(nullptr);
            float dx = 0.0f, dy = 0.0f;
            if (keys[SDL_SCANCODE_W]) dy -= 1.0f;
            if (keys[SDL_SCANCODE_S]) dy += 1.0f;
            if (keys[SDL_SCANCODE_A]) dx -= 1.0f;
            if (keys[SDL_SCANCODE_D]) dx += 1.0f;

            dx += controller.getLeftJoystickX();
            dy += controller.getLeftJoystickY();
            tank.move(dx, dy);
            tank.keepInBounds(800, 600);

            float leftTrigger = controller.getleftTrigger();
            float rightTrigger = controller.getrightTrigger();
            if ((keys[SDL_SCANCODE_SPACE] || leftTrigger > 0.3f || rightTrigger > 0.3f) && canFire) {
                bullets.push_back(tank.fire(10.0f, 5.0f, 5.0f));
                canFire = false;
            }
            if (leftTrigger < 0.3f && rightTrigger < 0.3f && !keys[SDL_SCANCODE_SPACE]) {
                canFire = true;
            }

            spawnTimer--;
            if (spawnTimer <= 0) {
                enemies.push_back(new Enemy(800, 3.0f, 50.0f, 30.0f, 60));
                spawnTimer = 60;
            }

            for (auto bullet : bullets) {
                bullet->update();
            }
            for (auto enemy : enemies) {
                Bullet* newBullet = enemy->update(10.0f, 5.0f, 5.0f);
                if (newBullet) bullets.push_back(newBullet);
            }

            for (auto bullet : bullets) {
                if (tank.checkCollision(bullet)) break;
                for (auto enemy : enemies) {
                    enemy->checkCollision(bullet);
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            tank.draw(renderer);
            for (auto bullet : bullets) {
                if (bullet->isActive() && !bullet->isOffScreen(800)) {
                    bullet->draw(renderer);
                } else {
                    bullet->setActive(false);
                }
            }
            for (auto enemy : enemies) {
                if (enemy->isAlive()) enemy->draw(renderer);
            }

            SDL_RenderPresent(renderer);
        }

        SDL_Delay(16);
    }

    for (auto bullet : bullets) delete bullet;
    for (auto enemy : enemies) delete enemy;
    return true;
}

int main(int argc, char* argv[]) {
    initSDL();
    srand(static_cast<unsigned>(time(0)));

    std::vector<std::string> tracks = {song1, song2, song3, song4, song5, song6};
    Music music(tracks);
    music.initMixer();
    music.playPlaylist();

    Controller controller;
    controller.init();
    controller.open();

    Menu mainMenu(renderer, 300, 200, 200, 50, font); // Thay bằng đường dẫn font
    mainMenu.addOption("New Game");
    mainMenu.addOption("Exit");

    bool running = true;
    while (running) {
        bool selected = mainMenu.update(controller);
        mainMenu.draw();

        if (selected) {
            int choice = mainMenu.getSelected();
            if (choice == 0) { // New Game
                if (!runGame(controller, music)) {
                    continue;
                } else {
                    running = false;
                }
            } else if (choice == 1) { // Exit
                running = false;
            }
        }

        SDL_Delay(16);
    }

    controller.close();
    cleanupSDL();
    return 0;
}