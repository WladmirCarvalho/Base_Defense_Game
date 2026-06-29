#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <queue>
#include "Entities.hpp"

class Game {
private:
    sf::RenderWindow window;
    Hero hero;
    Base baseCore;

    std::list<Projectile> projectiles;
    std::list<Enemy> enemies;
    std::list<AmmoDrop> ammoDrops;
    std::queue<float> spawnQueue;

    float timeSurvived;
    bool gameOver;

    void processEvents();
    void update(float dt);
    void render();
    void handleCollisions();

public:
    Game();
    void run();
};