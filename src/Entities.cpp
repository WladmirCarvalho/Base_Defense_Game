#include "../include/Entities.hpp"

// Projectile
Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f dir, bool heroOwned) 
    : direction(dir), speed(400.f), distanceTraveled(0.f), maxRange(300.f), isHeroOwned(heroOwned) {
    shape.setRadius(5.f);
    shape.setFillColor(heroOwned ? sf::Color::Yellow : sf::Color::Magenta);
    shape.setPosition(startPos);
}
void Projectile::update(float dt) {
    sf::Vector2f moveVec = direction * speed * dt;
    shape.move(moveVec);
    distanceTraveled += getDistance(sf::Vector2f(0,0), moveVec);
}
bool Projectile::isExpired() const { return distanceTraveled >= maxRange; }

// AmmoDrop
AmmoDrop::AmmoDrop(sf::Vector2f pos) : ammoAmount(10), lifetime(10.f) {
    shape.setSize(sf::Vector2f(15.f, 15.f));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(pos);
}
void AmmoDrop::update(float dt) { lifetime -= dt; }
bool AmmoDrop::isExpired() const { return lifetime <= 0.f; }

// Base
Base::Base(float x, float y) : hp(50), maxHp(50), regenTimer(0.f) {
    shape.setSize(sf::Vector2f(100.f, 100.f));
    shape.setFillColor(sf::Color(139, 69, 19));
    shape.setOrigin(sf::Vector2f(50.f, 50.f));
    shape.setPosition(sf::Vector2f(x, y));
}
void Base::update(float dt) {
    regenTimer += dt;
    if (regenTimer >= 5.f && hp < maxHp) { hp += 5; regenTimer = 0.f; }
}
void Base::takeDamage(int dmg) { hp -= dmg; }
bool Base::isDestroyed() const { return hp <= 0; }

// Hero
Hero::Hero(float startX, float startY) : hp(100), ammo(50), speed(200.f), isMoving(false) {
    shape.setRadius(15.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(sf::Vector2f(15.f, 15.f));
    shape.setPosition(sf::Vector2f(startX, startY));
    targetPos = sf::Vector2f(startX, startY);
}
void Hero::setTarget(sf::Vector2f target) { targetPos = target; isMoving = true; }
void Hero::update(float dt) {
    if (isMoving) {
        float dist = getDistance(shape.getPosition(), targetPos);
        if (dist > 5.0f) {
            sf::Vector2f dir = normalizeVector(targetPos - shape.getPosition());
            shape.move(dir * speed * dt);
        } else { isMoving = false; }
    }
}
void Hero::takeDamage(int dmg) { hp -= dmg; }

// Enemy
Enemy::Enemy(sf::Vector2f startPos) : hp(20), speed(50.f), shootTimer(0.f) {
    shape.setRadius(15.f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(sf::Vector2f(15.f, 15.f));
    shape.setPosition(startPos);
}
void Enemy::update(float dt, sf::Vector2f targetPos) {
    sf::Vector2f dir = normalizeVector(targetPos - shape.getPosition());
    shape.move(dir * speed * dt);
}
bool Enemy::canShoot(float dt) {
    shootTimer += dt;
    if (shootTimer >= 3.f) { shootTimer = 0.f; return true; }
    return false;
}