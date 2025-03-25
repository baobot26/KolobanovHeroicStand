#include "player.h"
Bullet::Bullet(float startX, float startY, float dir){
    x = startX;
    y = startY;
    dir = angle;
    active = true;
}
void Bullet::move(){
    while(active){
        x += cos(angle) * speed;
        y += sin(angle) * speed;
    }
    if(x <= 0 || x >= SCREEN_WIDTH || y <= 0 || y <= SCREEN_HEIGHT) active = false;
}
void Bullet::draw(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

}
Tank::Tank(float startX, float startY){
    x = startX;
    y = startY;
    angle = 0;
    lastShot = 0;
}
void Tank::move(float dirX, float dirY){
    x += speed * dirX;
    y += speed * dirY;
    betweenWorld();
}
void Tank::betweenWorld(){
    if(x < 0) x = SCREEN_WIDTH - TANK_SIZE / 2;
    if(x > SCREEN_WIDTH - TANK_SIZE / 2) x = 0;
    if(y < 0) y = SCREEN_HEIGHT - TANK_SIZE / 2;
    if(y > SCREEN_HEIGHT - TANK_SIZE / 2) y = 0;
}
void Tank::shoot(){
    if(lastShot <= 0){
        bullets.emplace_back(x, y, angle);
        lastShot = shotCooldown;
    }
}
void Tank::update(){
    if(lastShot > 0) lastShot--;
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return b.isOffScreen(); }), bullets.end());
}
void Tank::draw(SDL_Renderer* renderer){

}
bool Tank::checkCollision(const Bullet& bullet, const Tank& tank){
    return(bullet.x >= tank.x - TANK_SIZE / 2 || bullet.x <= tank.x + TANK_SIZE / 2 || bullet.y <= tank.y - TANK_SIZE / 2 || bullet.y >= tank.y + TANK_SIZE / 2);
}