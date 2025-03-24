#ifndef PLAYER_H
#define PLAYER_H
#include "defs.h"

#define BULLET_SPEED 5
#define TRV_SPEED 3
#define RT_SPEED 1
#define COOLDOWN 5000
struct Bullet{
    float x, y;
    float angle;
    float speed;
    Bullet(float startX, float startY, float dir);
    void move();
    bool isOutScreen();
    void draw(SDL_Renderer* renderer);
};
struct Tank{
    float x, y; // tank location
    float speed = TRV_SPEED; // tank speed
    float angle; // tank firing angle
    float rotate_speed = RT_SPEED; 
    int lastShot;
    int shotCooldown = COOLDOWN;
    Tank(float startX, float startY);
    void move();
    void betweenWorld();
    void shoot();
    void update();
    bool checkCollision();
    void draw();
};
#endif