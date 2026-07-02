#include "../include/Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : window(sf::VideoMode({800, 600}), "Base Defense"),
               hero(400.f, 300.f), baseCore(400.f, 300.f),
               hpText(font), ammoText(font),
               moveHintText(font), shootHintText(font),
               gameOverText(font), finalTimeText(font),
               replayButtonText(font), exitButtonText(font),
               titleText(font), leaderboardTitleText(font),
               leaderboardText(font), startButtonText(font),
               newRecordText(font), nameEntryPromptText(font),
               nameEntryInputText(font),
               leaderboard("leaderboard.txt"), lastScore(0),
               timeSurvived(0.f), state(GameState::START) {

    // Inicia a semente de aleatoriedade
    srand(static_cast<unsigned int>(time(nullptr)));

    // Configuração da Música de fundo
    if (bgMusic.openFromFile("musica.ogg")) {
        bgMusic.setLooping(true);
        bgMusic.setVolume(50.f);
        bgMusic.play();
    } else {
        std::cout << "Aviso: Arquivo de musica nao encontrado.\n";
    }

    // Configuração do HUD (Textos na tela)
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Aviso: Fonte arial.ttf nao encontrada na pasta.\n";
    }

    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::Black);
    hpText.setPosition(sf::Vector2f(650.f, 10.f));

    ammoText.setCharacterSize(20);
    ammoText.setFillColor(sf::Color::Black);
    ammoText.setPosition(sf::Vector2f(650.f, 35.f));

    moveHintText.setString("Aperte o botao direito do mouse para andar");
    moveHintText.setCharacterSize(16);
    moveHintText.setFillColor(sf::Color::Black);
    moveHintText.setPosition(sf::Vector2f(10.f, 10.f));

    shootHintText.setString("Aperte 'Q' para atirar");
    shootHintText.setCharacterSize(16);
    shootHintText.setFillColor(sf::Color::Black);
    shootHintText.setPosition(sf::Vector2f(10.f, 32.f));

    // Configuração da tela de Game Over
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    centerText(gameOverText, 400.f, 200.f);

    finalTimeText.setCharacterSize(22);
    finalTimeText.setFillColor(sf::Color::White);

    replayButton.setSize(sf::Vector2f(180.f, 50.f));
    replayButton.setFillColor(sf::Color(70, 130, 180));
    replayButton.setPosition(sf::Vector2f(210.f, 350.f));

    replayButtonText.setString("Jogar Novamente");
    replayButtonText.setCharacterSize(18);
    replayButtonText.setFillColor(sf::Color::White);
    centerText(replayButtonText, 300.f, 375.f);

    exitButton.setSize(sf::Vector2f(120.f, 50.f));
    exitButton.setFillColor(sf::Color(180, 70, 70));
    exitButton.setPosition(sf::Vector2f(470.f, 350.f));

    exitButtonText.setString("Sair");
    exitButtonText.setCharacterSize(18);
    exitButtonText.setFillColor(sf::Color::White);
    centerText(exitButtonText, 530.f, 375.f);

    // Configuração da tela inicial
    titleText.setString("BASE DEFENSE");
    titleText.setCharacterSize(40);
    titleText.setStyle(sf::Text::Bold);
    titleText.setFillColor(sf::Color::Black);
    centerText(titleText, 400.f, 70.f);

    leaderboardTitleText.setString("MELHORES PONTUACOES");
    leaderboardTitleText.setCharacterSize(22);
    leaderboardTitleText.setFillColor(sf::Color::Black);
    centerText(leaderboardTitleText, 400.f, 150.f);

    leaderboardText.setCharacterSize(20);
    leaderboardText.setFillColor(sf::Color::Black);
    refreshLeaderboardText();

    startButton.setSize(sf::Vector2f(200.f, 55.f));
    startButton.setFillColor(sf::Color(70, 130, 180));
    startButton.setPosition(sf::Vector2f(300.f, 470.f));

    startButtonText.setString("Iniciar");
    startButtonText.setCharacterSize(22);
    startButtonText.setFillColor(sf::Color::White);
    centerText(startButtonText, 400.f, 497.f);

    // Configuração da tela de novo recorde (estilo arcade)
    newRecordText.setString("NOVO RECORDE!");
    newRecordText.setCharacterSize(36);
    newRecordText.setFillColor(sf::Color::Red);
    newRecordText.setStyle(sf::Text::Bold);
    centerText(newRecordText, 400.f, 150.f);

    nameEntryPromptText.setString("Digite seu nome e aperte ENTER:");
    nameEntryPromptText.setCharacterSize(20);
    nameEntryPromptText.setFillColor(sf::Color::White);
    centerText(nameEntryPromptText, 400.f, 330.f);

    nameEntryInputText.setString("_");
    nameEntryInputText.setCharacterSize(28);
    nameEntryInputText.setFillColor(sf::Color::Yellow);
    nameEntryInputText.setStyle(sf::Text::Bold);
    centerText(nameEntryInputText, 400.f, 380.f);
}

void Game::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    text.setPosition(sf::Vector2f(x, y));
}

void Game::refreshLeaderboardText() {
    const auto& entries = leaderboard.getEntries();
    if (entries.empty()) {
        leaderboardText.setString("Nenhuma pontuacao registrada ainda.");
    } else {
        std::string text;
        int rank = 1;
        for (const auto& entry : entries) {
            text += std::to_string(rank) + ". " + entry.name + " - " + std::to_string(entry.score) + "s\n";
            rank++;
        }
        text.pop_back();
        leaderboardText.setString(text);
    }
    centerText(leaderboardText, 400.f, 230.f);
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        if (state == GameState::PLAYING) {
            update(dt);
            handleCollisions();
        }
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (state == GameState::START) {
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (startButton.getGlobalBounds().contains(mousePos)) {
                        resetGame();
                    }
                }
            }
        } else if (state == GameState::PLAYING) {
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Right) {
                    hero.setTarget(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
                }
            }

            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Q && hero.ammo > 0) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    sf::Vector2f dir = normalizeVector(mousePos - hero.shape.getPosition());
                    projectiles.push_back(Projectile(hero.shape.getPosition(), dir, true));
                    hero.ammo--;
                }
            }
        } else if (state == GameState::ENTER_NAME) {
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                char32_t unicode = textEntered->unicode;
                if (unicode == 8) {
                    if (!playerNameInput.empty()) playerNameInput.pop_back();
                } else if (unicode >= 32 && unicode < 127 && unicode != ' ' && playerNameInput.size() < 10) {
                    playerNameInput += static_cast<char>(unicode);
                }
                nameEntryInputText.setString(playerNameInput.empty() ? "_" : playerNameInput + "_");
                centerText(nameEntryInputText, 400.f, 380.f);
            }

            if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPress->code == sf::Keyboard::Key::Enter && !playerNameInput.empty()) {
                    leaderboard.addScore(playerNameInput, lastScore);
                    refreshLeaderboardText();
                    state = GameState::GAME_OVER;
                }
            }
        } else if (state == GameState::GAME_OVER) {
            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (replayButton.getGlobalBounds().contains(mousePos)) {
                        resetGame();
                    } else if (exitButton.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                }
            }
        }
    }
}

void Game::update(float dt) {
    timeSurvived += dt;
    hero.update(dt);
    baseCore.update(dt);

    // Sistema de Spawn nas bordas
    if (!spawnQueue.empty() && timeSurvived >= spawnQueue.front()) {
        float spawnX = 0.f;
        float spawnY = 0.f;
        int side = rand() % 4;

        if (side == 0) { spawnX = static_cast<float>(rand() % 800); spawnY = -20.f; }
        else if (side == 1) { spawnX = 820.f; spawnY = static_cast<float>(rand() % 600); }
        else if (side == 2) { spawnX = static_cast<float>(rand() % 800); spawnY = 620.f; }
        else { spawnX = -20.f; spawnY = static_cast<float>(rand() % 600); }

        enemies.push_back(Enemy(sf::Vector2f(spawnX, spawnY)));
        spawnQueue.pop();
        spawnQueue.push(timeSurvived + 3.0f);
    }

    for (auto& e : enemies) {
        e.update(dt, baseCore.shape.getPosition());
        if (e.canShoot(dt)) {
            sf::Vector2f dir = normalizeVector(hero.shape.getPosition() - e.shape.getPosition());
            projectiles.push_back(Projectile(e.shape.getPosition(), dir, false));
        }
    }

    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        it->update(dt);
        if (it->isExpired()) it = projectiles.erase(it);
        else ++it;
    }

    for (auto it = ammoDrops.begin(); it != ammoDrops.end(); ) {
        it->update(dt);
        if (it->isExpired()) it = ammoDrops.erase(it);
        else ++it;
    }

    // Atualiza os valores do HUD na tela
    hpText.setString("Vida: " + std::to_string(hero.hp) + "/100");
    ammoText.setString("Municao: " + std::to_string(hero.ammo));

    if (baseCore.isDestroyed() || hero.hp <= 0) {
        std::cout << "Fim de Jogo! Tempo: " << timeSurvived << "s\n";

        lastScore = static_cast<int>(timeSurvived);
        finalTimeText.setString("Tempo sobrevivido: " + std::to_string(lastScore) + "s");
        centerText(finalTimeText, 400.f, 270.f);

        if (leaderboard.qualifies(lastScore)) {
            playerNameInput.clear();
            nameEntryInputText.setString("_");
            centerText(nameEntryInputText, 400.f, 380.f);
            state = GameState::ENTER_NAME;
        } else {
            state = GameState::GAME_OVER;
        }
    }
}

void Game::resetGame() {
    hero = Hero(400.f, 300.f);
    baseCore = Base(400.f, 300.f);
    enemies.clear();
    projectiles.clear();
    ammoDrops.clear();

    while (!spawnQueue.empty()) spawnQueue.pop();
    spawnQueue.push(2.0f);
    spawnQueue.push(4.0f);
    spawnQueue.push(6.0f);

    timeSurvived = 0.f;
    state = GameState::PLAYING;
}

void Game::handleCollisions() {
    for (auto pIt = projectiles.begin(); pIt != projectiles.end(); ) {
        bool projectileDestroyed = false;

        if (pIt->isHeroOwned) {
            for (auto eIt = enemies.begin(); eIt != enemies.end(); ) {
                if (pIt->shape.getGlobalBounds().findIntersection(eIt->shape.getGlobalBounds()).has_value()) {
                    eIt->hp -= 10;
                    if (eIt->hp <= 0) {
                        ammoDrops.push_back(AmmoDrop(eIt->shape.getPosition()));
                        eIt = enemies.erase(eIt);
                    } else { ++eIt; }
                    projectileDestroyed = true;
                    break;
                } else { ++eIt; }
            }
        } else {
            if (pIt->shape.getGlobalBounds().findIntersection(hero.shape.getGlobalBounds()).has_value()) {
                hero.takeDamage(10);
                projectileDestroyed = true;
            }
            if (pIt->shape.getGlobalBounds().findIntersection(baseCore.shape.getGlobalBounds()).has_value()) {
                baseCore.takeDamage(5);
                projectileDestroyed = true;
            }
        }

        if (projectileDestroyed) pIt = projectiles.erase(pIt);
        else ++pIt;
    }

    for (auto it = ammoDrops.begin(); it != ammoDrops.end(); ) {
        if (hero.shape.getGlobalBounds().findIntersection(it->shape.getGlobalBounds()).has_value()) {
            hero.ammo += it->ammoAmount;
            it = ammoDrops.erase(it);
        } else { ++it; }
    }
}

void Game::render() {
    window.clear(sf::Color(220, 220, 220));

    if (state == GameState::START) {
        renderStart();
    } else {
        window.draw(baseCore.shape);
        window.draw(hero.shape);
        for (auto& e : enemies) window.draw(e.shape);
        for (auto& p : projectiles) window.draw(p.shape);
        for (auto& a : ammoDrops) window.draw(a.shape);

        // Desenha o HUD
        window.draw(hpText);
        window.draw(ammoText);
        window.draw(moveHintText);
        window.draw(shootHintText);

        if (state == GameState::ENTER_NAME) renderEnterName();
        else if (state == GameState::GAME_OVER) renderGameOver();
    }

    window.display();
}

void Game::renderStart() {
    window.draw(titleText);
    window.draw(leaderboardTitleText);
    window.draw(leaderboardText);
    window.draw(startButton);
    window.draw(startButtonText);
}

void Game::renderGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    window.draw(gameOverText);
    window.draw(finalTimeText);
    window.draw(replayButton);
    window.draw(replayButtonText);
    window.draw(exitButton);
    window.draw(exitButtonText);
}

void Game::renderEnterName() {
    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    window.draw(newRecordText);
    window.draw(finalTimeText);
    window.draw(nameEntryPromptText);
    window.draw(nameEntryInputText);
}
