#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <queue>
#include <string>
#include "Entities.hpp"
#include "Leaderboard.hpp"

enum class GameState { START, PLAYING, GAME_OVER };

class Game {
private:
    sf::Font font;
    sf::RenderWindow window;
    Hero hero;
    Base baseCore;

    // Áudio
    sf::Music bgMusic;
    sf::Music menuMusic;

    // HUD
    sf::Text hpText;
    sf::Text ammoText;
    sf::Text timerText;
    sf::Text baseHpText;
    sf::Text endText;
    sf::Text creditsText;
    sf::Text bossHpText;

    // (Menu e Leaderboard) ---
    sf::Text moveHintText;
    sf::RectangleShape hpSq, ammoSq, megaSq, pwrSq;
    sf::Text hpDesc, ammoDesc, megaDesc, pwrDesc;
    sf::Text shootHintText;
    sf::Text objectiveHintText;
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
    sf::Text newRecordText;
    sf::Text nameEntryPromptText;
    sf::Text nameEntryInputText;
    sf::Text top1Text;
    std::string playerNameInput;
    // Ranking
    Leaderboard leaderboard;
    int lastScore;

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
    
    GameState state; // Estado do Menu

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
