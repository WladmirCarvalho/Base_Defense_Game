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
    sf::Text moveHintText;
    sf::Text shootHintText;

    // Tela de Game Over
    sf::Text gameOverText;
    sf::Text finalTimeText;
    sf::RectangleShape replayButton;
    sf::Text replayButtonText;
    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

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
    void renderGameOver();
    void resetGame();
    void centerText(sf::Text& text, float x, float y);

public:
    Game();
    void run();
};