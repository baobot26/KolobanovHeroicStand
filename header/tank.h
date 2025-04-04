#ifndef TANK_H
#define TANK_H

#include <SDL2/SDL.h>
#include "../header/bullet.h"

class Tank {
private:
    float x, y;
    float speed;
    float width, height;
    bool alive;

public:
    Tank(float startX, float startY, float tankSpeed, float tankWidth, float tankHeight);
    void move(float dx, float dy);
    Bullet* fire(float bulletSpeed, float bulletWidth, float bulletHeight);
    void draw(SDL_Renderer* renderer);
    bool checkCollision(Bullet* bullet);
    void keepInBounds(int screenWidth, int screenHeight);
    bool isAlive() const;
};

#endif // TANK_H