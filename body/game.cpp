#include "../header/game.h"
#include "../header/tank.h"
#include "../header/enemy.h"
#include "../header/bullet.h"
#include <vector>
#include <ctime>
#include <iostream>
#include <string>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "SDL_init: " << SDL_GetError() << std::endl;
        exit(1);
    }
    window = SDL_CreateWindow("Kolobanov Heroic Stand", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    if (!window) {
        std::cerr << "Create Window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Create Renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    if (TTF_Init() < 0) {
        std::cerr << "TTF_init: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

void cleanupSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

bool runGame(Controller& controller, Music& music, TTF_Font* font) {
    Tank tank(0, 300, 5.0f, 50.0f, 30.0f);
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    int spawnTimer = 200;
    bool canFire = true;
    bool paused = false;
    bool gameOver = false;
    int score = 0;

    PauseState pauseMenu(renderer, font, 300, 150, 200, 50);
    pauseMenu.addOption("Resume");
    pauseMenu.addOption("Restart");
    pauseMenu.addOption("Exit");
    pauseMenu.addOption("Quit");

    EndState gameOverMenu(renderer, font, 300, 250, 200, 50);
    gameOverMenu.addOption("Restart");
    gameOverMenu.addOption("Exit");
    gameOverMenu.addOption("Quit");

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return false;
            if (!gameOver) {
                if (event.type == SDL_CONTROLLERBUTTONDOWN) {
                    if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER ||
                        event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
                        paused = !paused;
                    }
                }
            }
        }

        if (paused) {
            int choice = pauseMenu.update(controller); 
            if (choice != -1) {
                if (choice == 0) paused = false; // Resume
                else if (choice == 1) { // Restart
                    for (auto bullet : bullets) delete bullet;
                    bullets.clear();
                    for (auto enemy : enemies) delete enemy;
                    enemies.clear();
                    tank = Tank(0, 300, 5.0f, 50.0f, 30.0f);
                    spawnTimer = 60;
                    canFire = true;
                    score = 0;
                    paused = false;
                } else if (choice == 2) { // Exit
                    running = false;
                    return false;
                } else if (choice == 3) { // Quit
                    for (auto bullet : bullets) delete bullet;
                    bullets.clear();
                    for (auto enemy : enemies) delete enemy;
                    enemies.clear();
                    return true;
                }
            }
            pauseMenu.draw();
            SDL_RenderPresent(renderer);
        } else if (gameOver) {
            gameOverMenu.setScore(score);
            int choice = gameOverMenu.update(controller); 
            if (choice != -1) {
                if (choice == 0) { // Restart
                    for (auto bullet : bullets) delete bullet;
                    bullets.clear();
                    for (auto enemy : enemies) delete enemy;
                    enemies.clear();
                    tank = Tank(0, 300, 5.0f, 50.0f, 30.0f);
                    spawnTimer = 60;
                    canFire = true;
                    score = 0;
                    gameOver = false;
                } else if (choice == 1) { // Exit
                    running = false;
                    return false;
                } else if (choice == 2) { // Quit
                    for (auto bullet : bullets) delete bullet;
                    bullets.clear();
                    for (auto enemy : enemies) delete enemy;
                    enemies.clear();
                    return true;
                }
            }
            gameOverMenu.draw();
            SDL_RenderPresent(renderer);
        } else {
            float dx = controller.getLeftJoystickX();
            float dy = controller.getLeftJoystickY();
            tank.move(dx, dy);
            tank.keepInBounds(800, 600);

            float leftTrigger = controller.getleftTrigger();
            float rightTrigger = controller.getrightTrigger();
            if ((leftTrigger > 0.3f || rightTrigger > 0.3f) && canFire) {
                bullets.push_back(tank.fire(10.0f, 5.0f, 5.0f));
                canFire = false;
            }
            if (leftTrigger < 0.3f && rightTrigger < 0.3f) canFire = true;

            spawnTimer--;
            if (spawnTimer <= 0) {
                enemies.push_back(new Enemy(800, 3.0f, 50.0f, 30.0f, 60));
                spawnTimer = 60;
            }

            for (auto bullet : bullets) bullet->update();
            for (auto enemy : enemies) {
                Bullet* newBullet = enemy->update(10.0f, 5.0f, 5.0f);
                if (newBullet) bullets.push_back(newBullet);
            }

            for (auto bullet : bullets) {
                if (tank.checkCollision(bullet)) {
                    gameOver = true;
                    break;
                }
                for (auto enemy : enemies) {
                    if (enemy->checkCollision(bullet)) score += 1;
                }
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            tank.draw(renderer);
            for (auto bullet : bullets) {
                if (bullet->isActive() && !bullet->isOffScreen(800)) bullet->draw(renderer);
                else bullet->setActive(false);
            }
            for (auto enemy : enemies) {
                if (enemy->isAlive()) enemy->draw(renderer);
            }

            if (font) {
                std::string scoreText = "Score: " + std::to_string(score);
                SDL_Color white = {255, 255, 255, 255};
                SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
                if (surface) {
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                    if (texture) {
                        int textW, textH;
                        SDL_QueryTexture(texture, nullptr, nullptr, &textW, &textH);
                        SDL_Rect textRect = {10, 10, textW, textH};
                        SDL_RenderCopy(renderer, texture, nullptr, &textRect);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_FreeSurface(surface);
                }
            }

            SDL_RenderPresent(renderer);
        }

        SDL_Delay(16);
    }

    for (auto bullet : bullets) delete bullet;
    for (auto enemy : enemies) delete enemy;
    return false;
}