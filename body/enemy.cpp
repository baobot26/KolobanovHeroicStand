#include "../header/enemy.h"
#include "../header/bullet.h"
#include <cstdlib>

Enemy::Enemy(float screenWidth, float enemySpeed, float enemyWidth, float enemyHeight, int interval)
    : x(screenWidth), speed(enemySpeed), width(enemyWidth), height(enemyHeight), 
      alive(true), fireTimer(0), fireInterval(interval) {
    y = rand() % (600 - static_cast<int>(height));
}

Bullet* Enemy::update(float bulletSpeed, float bulletWidth, float bulletHeight) {
    if (alive) {
        x -= speed;
        fireTimer--;
        if (fireTimer <= 0) {
            fireTimer = fireInterval;
            return fire(bulletSpeed, bulletWidth, bulletHeight);
        }
    }
    return nullptr;
}

Bullet* Enemy::fire(float bulletSpeed, float bulletWidth, float bulletHeight) {
    return new Bullet(x - bulletWidth, y + height / 2, -bulletSpeed, bulletWidth, bulletHeight);
}

void Enemy::draw(SDL_Renderer* renderer) {
    if (alive) {
        SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height) };
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}

bool Enemy::checkCollision(Bullet* bullet) {
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

bool Enemy::isAlive() const { return alive; }