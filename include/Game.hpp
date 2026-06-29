#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <queue>
#include "Entities.hpp"

class Game {
private:
    sf::RenderWindow window;
    Hero hero;
    Base baseCore;
    
    // Áudio
    sf::Music bgMusic;
    
    // HUD
    sf::Font font;
    sf::Text hpText;
    sf::Text ammoText;

    // Entidades e Sistema
    std::queue<float> spawnQueue;
    std::list<Enemy> enemies;
    std::list<Projectile> projectiles;
    std::list<AmmoDrop> ammoDrops;

    float timeSurvived;
    bool gameOver;

    void processEvents();
    void update(float dt);
    void handleCollisions();
    void render();

public:
    Game();
    void run();
};