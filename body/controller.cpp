#include "../header/controller.h"
#include <iostream>

Controller::Controller() : gameController(nullptr) {}

Controller::~Controller() {
    close();
}

void Controller::init() {
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "Lỗi khởi tạo subsystem controller: " << SDL_GetError() << std::endl;
    }
}

void Controller::open() {
    if (SDL_NumJoysticks() > 0) {
        gameController = SDL_GameControllerOpen(0);
        if (!gameController) {
            std::cerr << "Lỗi mở controller: " << SDL_GetError() << std::endl;
        }
    }
}

void Controller::close() {
    if (gameController) {
        SDL_GameControllerClose(gameController);
        gameController = nullptr;
    }
}

bool Controller::isButtonPressed(Button button) {
    if (gameController) {
        return SDL_GameControllerGetButton(gameController, static_cast<SDL_GameControllerButton>(button));
    }
    return false;
}

float Controller::getLeftJoystickX() {
    if (gameController) {
        return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX) / 32767.0f;
    }
    return 0.0f;
}

float Controller::getLeftJoystickY() {
    if (gameController) {
        return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0f;
    }
    return 0.0f;
}
float Controller::getleftTrigger() {
    if (gameController) {
        return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 32767.0f;
    }
    return 0.0f;
}
float Controller::getrightTrigger() {
    if (gameController) {
        return SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32767.0f;
    }
    return 0.0f;
}
bool Controller::getButtonState(Uint8 button) {
    if (gameController) {
        return SDL_GameControllerGetButton(gameController, static_cast<SDL_GameControllerButton>(button));
    }
    return false;
}