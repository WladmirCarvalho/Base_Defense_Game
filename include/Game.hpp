#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <queue>
#include "Entities.hpp"

class Game {
private:
    sf::Font font;
    sf::RenderWindow window;
    Hero hero;
    Base baseCore;
    
    // Áudio
    sf::Music bgMusic;
    
    // HUD
    sf::Text hpText;
    sf::Text ammoText;
    sf::Text timerText;
    sf::Text baseHpText;
    sf::Text endText;
    sf::Text creditsText;
    sf::Text bossHpText;

    // Entidades e Sistema
    std::queue<float> spawnQueue;
    std::list<Enemy> enemies;
    std::list<Projectile> projectiles;
    std::list<Boost> boosts;

    sf::RectangleShape dangerBorder;

    float timeSurvived;
    float maxMusicTime;
    bool gameOver;
    bool victory;
    bool finalBossSpawned;
    bool finalBossDefeated;
    int totalEnemiesSpawned;

    void processEvents();
    void update(float dt);
    void handleCollisions();
    void render();

public:
    Game();
    void run();
};