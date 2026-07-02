#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <queue>
#include <string>
#include "Entities.hpp"
#include "Leaderboard.hpp"

enum class GameState { START, PLAYING, ENTER_NAME, GAME_OVER };

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

    // Tela inicial
    sf::Text titleText;
    sf::Text leaderboardTitleText;
    sf::Text leaderboardText;
    sf::RectangleShape startButton;
    sf::Text startButtonText;

    // Tela de novo recorde (estilo arcade)
    sf::Text newRecordText;
    sf::Text nameEntryPromptText;
    sf::Text nameEntryInputText;
    std::string playerNameInput;

    // Ranking
    Leaderboard leaderboard;
    int lastScore;

    // Entidades e Sistema
    std::queue<float> spawnQueue;
    std::list<Enemy> enemies;
    std::list<Projectile> projectiles;
    std::list<AmmoDrop> ammoDrops;

    float timeSurvived;
    GameState state;

    void processEvents();
    void update(float dt);
    void handleCollisions();
    void render();
    void renderStart();
    void renderGameOver();
    void renderEnterName();
    void resetGame();
    void centerText(sf::Text& text, float x, float y);
    void refreshLeaderboardText();

public:
    Game();
    void run();
};
