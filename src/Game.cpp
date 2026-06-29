#include "../include/Game.hpp"
#include <iostream>
#include <cstdlib> // Para rand() e srand()
#include <ctime>   // Para time()

Game::Game() : window(sf::VideoMode({800, 600}), "Base Defense"), 
               hero(400.f, 300.f), baseCore(400.f, 300.f), timeSurvived(0.f), gameOver(false) {
    
    // Inicia a semente de aleatoriedade baseada no relógio do computador
    srand(static_cast<unsigned int>(time(nullptr)));

    // Configura os primeiros spawns na Fila
    spawnQueue.push(2.0f);
    spawnQueue.push(4.0f);
    spawnQueue.push(6.0f);
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
        }
    }
}

void Game::update(float dt) {
    timeSurvived += dt;
    hero.update(dt);
    baseCore.update(dt);

    // Sistema de Spawn com Fila e Aleatoriedade nas Bordas
    if (!spawnQueue.empty() && timeSurvived >= spawnQueue.front()) {
        float spawnX = 0.f;
        float spawnY = 0.f;
        
        // Sorteia um lado: 0=Topo, 1=Direita, 2=Baixo, 3=Esquerda
        int side = rand() % 4; 
        
        if (side == 0) { // Topo
            spawnX = static_cast<float>(rand() % 800);
            spawnY = -20.f; // Nasce um pouco fora da tela
        } else if (side == 1) { // Direita
            spawnX = 820.f;
            spawnY = static_cast<float>(rand() % 600);
        } else if (side == 2) { // Baixo
            spawnX = static_cast<float>(rand() % 800);
            spawnY = 620.f;
        } else { // Esquerda
            spawnX = -20.f;
            spawnY = static_cast<float>(rand() % 600);
        }

        enemies.push_back(Enemy(sf::Vector2f(spawnX, spawnY)));
        spawnQueue.pop();
        
        // Reabastece a fila (para o jogo ficar infinito)
        // Dica de melhoria futura: diminuir esse 3.0f com o passar do tempo aumenta a dificuldade!
        spawnQueue.push(timeSurvived + 3.0f); 
    }

    // Atualiza Inimigos
    for (auto& e : enemies) {
        e.update(dt, baseCore.shape.getPosition());
        if (e.canShoot(dt)) {
            sf::Vector2f dir = normalizeVector(hero.shape.getPosition() - e.shape.getPosition());
            projectiles.push_back(Projectile(e.shape.getPosition(), dir, false));
        }
    }

    // Atualiza Projéteis
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        it->update(dt);
        if (it->isExpired()) it = projectiles.erase(it);
        else ++it;
    }

    // Atualiza Drops de Munição
    for (auto it = ammoDrops.begin(); it != ammoDrops.end(); ) {
        it->update(dt);
        if (it->isExpired()) it = ammoDrops.erase(it);
        else ++it;
    }

    if (baseCore.isDestroyed() || hero.hp <= 0) {
        std::cout << "Fim de Jogo! Tempo de sobrevivencia: " << timeSurvived << " segundos.\n";
        gameOver = true;
    }
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
    window.display();
}