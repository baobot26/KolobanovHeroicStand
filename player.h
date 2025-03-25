#ifndef PLAYER_H
#define PLAYER_H
#include "defs.h"

#define BULLET_SPEED 5
#define BULLET_SIZE 5
#define TRV_SPEED 3
#define RT_SPEED 1
#define COOLDOWN 5000
#define TANK_SIZE 50
struct Bullet{
    float x, y;
    float angle;
    float speed;
    bool active;
    Bullet(float startX, float startY, float dir);
    void move();
    void draw(SDL_Renderer* renderer);
};
struct Tank{
    float x, y; // tank location
    float speed = TRV_SPEED; // tank speed
    float angle; // tank firing angle
    float rotate_speed = RT_SPEED; 
    int lastShot;
    int shotCooldown = COOLDOWN;
    std::vector<Bullet> bullets;
    Tank(float startX, float startY);
    void move(float dirX, float dirY);
    void betweenWorld();
    void shoot();
    void update();
    bool checkCollision(const Bullet& bullet, const Tank& tank);
    void draw(SDL_Renderer* renderer);
};
#endif