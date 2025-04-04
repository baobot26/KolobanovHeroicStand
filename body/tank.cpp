#include "../header/tank.h"

Tank::Tank(float startX, float startY, float tankSpeed, float tankWidth, float tankHeight)
    : x(startX), y(startY), speed(tankSpeed), width(tankWidth), height(tankHeight), alive(true) {}

void Tank::move(float dx, float dy) {
    x += dx * speed;
    y += dy * speed;
}

Bullet* Tank::fire(float bulletSpeed, float bulletWidth, float bulletHeight) {
    return new Bullet(x + width, y + height / 2, bulletSpeed, bulletWidth, bulletHeight);
}

void Tank::draw(SDL_Renderer* renderer) {
    if (alive) {
        SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Tank::checkCollision(Bullet* bullet) {
    if (!alive || !bullet->isActive()) return false;
    float bx = bullet->getX();
    float by = bullet->getY();
    float bw = bullet->getWidth();
    float bh = bullet->getHeight();
    if (x < bx + bw && x + width > bx && y < by + bh && y + height > by) {
        alive = false;
        return true;
    }
    return false;
}

void Tank::keepInBounds(int screenWidth, int screenHeight) {
    if (x < 0) x = 0;
    if (x > screenWidth - width) x = screenWidth - width;
    if (y < 0) y = 0;
    if (y > screenHeight - height) y = screenHeight - height;
}

bool Tank::isAlive() const { return alive; }