#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include "../header/bullet.h"

class Enemy {
private:
    float x, y;
    float speed;
    float width, height;
    bool alive;
    int fireTimer;
    int fireInterval;

public:
    Enemy(float screenWidth, float enemySpeed, float enemyWidth, float enemyHeight, int interval);
    Bullet* update(float bulletSpeed, float bulletWidth, float bulletHeight);
    Bullet* fire(float bulletSpeed, float bulletWidth, float bulletHeight);
    void draw(SDL_Renderer* renderer);
    bool checkCollision(Bullet* bullet);
    bool isAlive() const;
};

#endif // ENEMY_H