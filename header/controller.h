#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL2/SDL.h>
#include <iostream>

class Controller {
private:
    SDL_GameController* gameController;

public:
    Controller();
    ~Controller();
    void init();
    void open();
    void close();
    float getLeftJoystickX();
    float getLeftJoystickY();
    float getleftTrigger();
    float getrightTrigger();
    bool getButtonState(Uint8 button);
};

#endif // CONTROLLER_H