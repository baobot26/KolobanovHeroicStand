#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

class Bullet {
private:
    float x, y;
    float speed;
    float width, height;
    bool active;

public:
    Bullet(float startX, float startY, float bulletSpeed, float bulletWidth, float bulletHeight);
    void update();
    bool isOffScreen(int screenWidth);
    void draw(SDL_Renderer* renderer);
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    bool isActive() const;
    void setActive(bool state);
};

#endif // BULLET_H