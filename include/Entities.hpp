#pragma once
#include <SFML/Graphics.hpp>
#include "Utils.hpp"

// Projétil
class Projectile {
public:
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;
    float distanceTraveled;
    float maxRange;
    bool isHeroOwned;

    Projectile(sf::Vector2f startPos, sf::Vector2f dir, bool heroOwned);
    void update(float dt);
    bool isExpired() const;
};

// Item de Munição
class AmmoDrop {
public:
    sf::RectangleShape shape;
    int ammoAmount;
    float lifetime;

    AmmoDrop(sf::Vector2f pos);
    void update(float dt);
    bool isExpired() const;
};

// Base
class Base {
public:
    sf::RectangleShape shape;
    int hp;
    int maxHp;
    float regenTimer;

    Base(float x, float y);
    void update(float dt);
    void takeDamage(int dmg);
    bool isDestroyed() const;
};

// Herói
class Hero {
public:
    sf::CircleShape shape;
    sf::Vector2f targetPos;
    int hp;
    int ammo;
    float speed;
    bool isMoving;

    Hero(float startX, float startY);
    void setTarget(sf::Vector2f target);
    void update(float dt);
    void takeDamage(int dmg);
};

// Inimigo
class Enemy {
public:
    sf::CircleShape shape;
    int hp;
    float speed;
    float shootTimer;

    Enemy(sf::Vector2f startPos);
    void update(float dt, sf::Vector2f targetPos);
    bool canShoot(float dt);
};