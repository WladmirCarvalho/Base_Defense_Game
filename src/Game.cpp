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
               timeSurvived(0.f), gameOver(false) {
    
    // Inicia a semente de aleatoriedade
    srand(static_cast<unsigned int>(time(nullptr)));

    // Configura os primeiros spawns
    spawnQueue.push(2.0f);
    spawnQueue.push(4.0f);
    spawnQueue.push(6.0f);

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
}

void Game::centerText(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    text.setPosition(sf::Vector2f(x, y));
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents();
        if (!gameOver) {
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
            
        if (!gameOver) {
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
        } else {
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
        gameOver = true;
        finalTimeText.setString("Tempo sobrevivido: " + std::to_string(static_cast<int>(timeSurvived)) + "s");
        centerText(finalTimeText, 400.f, 270.f);
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
    gameOver = false;
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

    if (gameOver) renderGameOver();

    window.display();
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