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
    bool isPowered;

    Projectile(sf::Vector2f startPos, sf::Vector2f dir, bool heroOwned, bool isPowered = false);
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
    int maxAmmo;
    float speed;
    bool isMoving;
    float invulTimer;
    bool hasPowerUp;
    float powerUpTimer;

    Hero(float startX, float startY);
    void setTarget(sf::Vector2f target);
    void update(float dt);
    void takeDamage(int dmg);
};

// Enumeração para identificar o tipo de inimigo
enum class EnemyType { Common, MiniBoss, FinalBoss };

// Inimigo
class Enemy {
public:
    sf::CircleShape shape;
    EnemyType type;
    int hp;
    float speed;
    float shootTimer;
    float shootInterval;
    
    // Variáveis exclusivas do FinalBoss
    int damage;         
    int nextTeleportHp;
    bool isDying;
    float deathTimer; 

    Enemy(sf::Vector2f startPos, EnemyType type = EnemyType::Common);
    void update(float dt, sf::Vector2f targetPos);
    bool canShoot(float dt);
    
    // Nova função para gerenciar o teleporte quando tomar tiro
    void takeDamage(int dmg); 
};

// Enumeração para identificar o tipo de item
enum class BoostType { NormalAmmo, Health, MegaAmmo, PowerUp };

class Boost {
public:
    sf::RectangleShape shape;
    BoostType type;
    float lifetime; // Tempo que o item fica no chão antes de sumir

    Boost(float x, float y, BoostType type);
    void update(float dt);
};