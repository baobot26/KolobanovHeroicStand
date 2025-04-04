#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>

enum Button {
    BUTTON_L2 = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    BUTTON_R2 = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
};

class Controller {
private:
    SDL_GameController* gameController;

public:
    Controller();
    ~Controller();
    void init();
    void open();
    void close();
    bool isButtonPressed(Button button);
    float getLeftJoystickX();
    float getLeftJoystickY();
    float getleftTrigger();
    float getrightTrigger();
    bool getButtonState(Uint8 button);
};

#endif // CONTROLLER_H