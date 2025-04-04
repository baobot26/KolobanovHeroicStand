#include "../header/bullet.h"

Bullet::Bullet(float startX, float startY, float bulletSpeed, float bulletWidth, float bulletHeight)
    : x(startX), y(startY), speed(bulletSpeed), width(bulletWidth), height(bulletHeight), active(true) {}

void Bullet::update() {
    if (active) {
        x += speed;
    }
}

bool Bullet::isOffScreen(int screenWidth) {
    return x > screenWidth || x + width < 0;
}

void Bullet::draw(SDL_Renderer* renderer) {
    if (active) {
        SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

float Bullet::getX() const { return x; }
float Bullet::getY() const { return y; }
float Bullet::getWidth() const { return width; }
float Bullet::getHeight() const { return height; }
bool Bullet::isActive() const { return active; }
void Bullet::setActive(bool state) { active = state; }