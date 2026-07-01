#include "../include/Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : window(sf::VideoMode({800, 600}), "Base Defense"), 
               hero(400.f, 300.f), baseCore(400.f, 300.f), 
               hpText(font), ammoText(font), timerText(font), 
               baseHpText(font), endText(font), creditsText(font), bossHpText(font),
               timeSurvived(0.f), gameOver(false), victory(false),
               finalBossSpawned(false), finalBossDefeated(false), totalEnemiesSpawned(0) {
    
    // Inicia a semente de aleatoriedade
    srand(static_cast<unsigned int>(time(nullptr)));

    // Configura os primeiros spawns
    spawnQueue.push(2.0f);
    spawnQueue.push(4.0f);
    spawnQueue.push(6.0f);

    // Configuração da Música de fundo
    if (bgMusic.openFromFile("musica.ogg")) {
        bgMusic.setLooping(false); 
        bgMusic.setVolume(50.f);
        bgMusic.play();
        maxMusicTime = bgMusic.getDuration().asSeconds();
    } else {
        std::cout << "Aviso: Arquivo de musica nao encontrado.\n";
        maxMusicTime = 120.f; // Tempo de segurança se a música der erro
    }

    // Configuração do HUD (Textos na tela)
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Aviso: Fonte arial.ttf nao encontrada na pasta.\n";
    }
    
    hpText.setCharacterSize(26);
    hpText.setFillColor(sf::Color::Black);
    hpText.setPosition(sf::Vector2f(600.f, 10.f));

    ammoText.setCharacterSize(26);
    ammoText.setFillColor(sf::Color::Black);
    ammoText.setPosition(sf::Vector2f(600.f, 35.f));

    baseHpText.setCharacterSize(26);
    baseHpText.setFillColor(sf::Color::Black);
    baseHpText.setPosition(sf::Vector2f(600.f, 65.f));

    bossHpText.setCharacterSize(26);
    bossHpText.setFillColor(sf::Color::Red);
    bossHpText.setPosition(sf::Vector2f(600.f, 95.f));
    bossHpText.setString("");

    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(sf::Vector2f(350.f, 10.f));

    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::Black);
    endText.setString(""); // Começa vazio

    creditsText.setCharacterSize(24);
    creditsText.setFillColor(sf::Color::Black);
    creditsText.setString("Desenvolvido por:\nMarcos Gomes\nSibelle Galina\nWladmir Carvalho\n\nObrigado por jogar!");
    creditsText.setPosition(sf::Vector2f(20.f, 650.f)); // Começa escondido abaixo da tela

    dangerBorder.setSize(sf::Vector2f(800.f, 600.f));
    dangerBorder.setPosition(sf::Vector2f(0.f, 0.f));
    dangerBorder.setFillColor(sf::Color::Transparent); // Meio transparente
    dangerBorder.setOutlineThickness(-30.f);
    dangerBorder.setOutlineColor(sf::Color::Transparent); // Começa invisível
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
                    bool powered = hero.hasPowerUp;
                    projectiles.push_back(Projectile(hero.shape.getPosition(), dir, true, powered));
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


// GERENCIAMENTO DE SPAWN
    bool isFinalPhase = (timeSurvived >= (maxMusicTime - 33.f));

    if (isFinalPhase) {
        if (!finalBossSpawned) {
            enemies.push_back(Enemy(sf::Vector2f(400.f, -50.f), EnemyType::FinalBoss));
            finalBossSpawned = true;
        }
    } 
    else {
        // LÓGICA DE SPAWN (Progressão de Dificuldade)
        // Estágio 1 (Até 50 segundos)
        int maxEnemies = 10; 
        float spawnRate = 4.0f; // Tempo entre os nascimentos
        int enemiesToSpawn = 1;

        // Estágio 2 (Aos 50 segundos)
        if (timeSurvived >= 50.f && timeSurvived < 110.f) {
            maxEnemies = 12; 
            spawnRate = 3.5f;
        } 
        // Estágio 3 (Aos 110 segundos até o Boss)
        else if (timeSurvived >= 110.f) {
            maxEnemies = 15; 
            spawnRate = 3.0f; // Nascem mais rápido
            enemiesToSpawn = 2; // Começam a nascer em duplas!
        }

        if (!spawnQueue.empty() && timeSurvived >= spawnQueue.front()) {
            if (static_cast<int>(enemies.size()) < maxEnemies) {

                for (int i = 0; i < enemiesToSpawn; ++i) {
                    float spawnX = 0.f; float spawnY = 0.f;
                    int side = rand() % 4; 
                    if (side == 0) { spawnX = static_cast<float>(rand() % 800); spawnY = -20.f; }
                    else if (side == 1) { spawnX = 820.f; spawnY = static_cast<float>(rand() % 600); }
                    else if (side == 2) { spawnX = static_cast<float>(rand() % 800); spawnY = 620.f; }
                    else { spawnX = -20.f; spawnY = static_cast<float>(rand() % 600); }

                    totalEnemiesSpawned++; 

                    EnemyType typeToSpawn = EnemyType::Common;
                    if (totalEnemiesSpawned % 20 == 0) { 
                        typeToSpawn = EnemyType::MiniBoss;
                    }

                    enemies.push_back(Enemy(sf::Vector2f(spawnX, spawnY), typeToSpawn));
                }
            }
            spawnQueue.pop();
            spawnQueue.push(timeSurvived + spawnRate); // Usa a taxa dinâmica
        }
    }

    // --- LÓGICA DO FINAL BOSS NA TELA ---
    if (finalBossSpawned && !finalBossDefeated) {
        bool bossFound = false;
        for (auto& e : enemies) {
            if (e.type == EnemyType::FinalBoss) {
                bossFound = true;
                bossHpText.setString("Boss: " + std::to_string(e.hp));
                break;
            }
        }
        
        if (!bossFound) {
            finalBossDefeated = true;
            bossHpText.setString(""); // Apaga o texto quando ele morre
            dangerBorder.setOutlineColor(sf::Color::Transparent); // Apaga a borda
        } else {
            // Efeito de piscar a borda vermelha (usando a função seno do tempo)
            // Oscila a transparência do vermelho entre 80 e 180 (sensação de sirene)
            int alpha = 130 + static_cast<int>(50.0f * std::sin(timeSurvived * 5.0f));
            dangerBorder.setOutlineColor(sf::Color(255, 0, 0, alpha));
        }
    }

    // Atualiza os inimigos existentes
    for (auto eIt = enemies.begin(); eIt != enemies.end(); ) {
        eIt->update(dt, baseCore.shape.getPosition());
        
        // Verifica se a animação de morte do chefe acabou
        if (eIt->isDying && eIt->deathTimer <= 0.f) {
            finalBossDefeated = true;
            bossHpText.setString(""); 
            dangerBorder.setOutlineColor(sf::Color::Transparent); 
            eIt = enemies.erase(eIt); // Apaga o corpo definitivamente
        } 
        else {
            if (eIt->canShoot(dt)) {
                sf::Vector2f dir = normalizeVector(hero.shape.getPosition() - eIt->shape.getPosition());
                projectiles.push_back(Projectile(eIt->shape.getPosition(), dir, false));
            }
            ++eIt;
        }
    }

    for (auto it = boosts.begin(); it != boosts.end(); ) {
        it->update(dt);

        if (hero.shape.getGlobalBounds().findIntersection(it->shape.getGlobalBounds()).has_value()) {
            
            if (it->type == BoostType::NormalAmmo) {
                hero.ammo += 5;
                if (hero.ammo > hero.maxAmmo) hero.ammo = hero.maxAmmo; // LIMITADOR
            }
            else if (it->type == BoostType::Health) {
                hero.hp += 50; 
                if (hero.hp > 100) hero.hp = 100;
            } 
            else if (it->type == BoostType::MegaAmmo) {
                hero.ammo += 20; 
                if (hero.ammo > hero.maxAmmo) hero.ammo = hero.maxAmmo; // LIMITADOR
            }
            else if (it->type == BoostType::PowerUp) {
                hero.hasPowerUp = true;
                hero.powerUpTimer = 10.0f;
            }
            
            it = boosts.erase(it);
        }
        else if (it->lifetime <= 0.f) {
            it = boosts.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        it->update(dt);
        if (it->isExpired()) it = projectiles.erase(it);
        else ++it;
    }

    // Atualiza os valores do HUD na tela
    hpText.setString("Vida: " + std::to_string(hero.hp) + "/100");
    ammoText.setString("Municao: " + std::to_string(hero.ammo) + "/" + std::to_string(hero.maxAmmo));
    timerText.setString("Tempo: " + std::to_string(static_cast<int>(timeSurvived)) + "s");
    baseHpText.setString("Base: " + std::to_string(baseCore.hp));

    // --- LÓGICA DE FIM DE JOGO ---
    // Condição de Derrota (O jogo CONGELA se você morrer ou a base cair)
    if (baseCore.isDestroyed() || hero.hp <= 0) {
        gameOver = true;
        endText.setString("GAME OVER");
        endText.setFillColor(sf::Color::Red);
    }
    // Condição de Vitória (O jogo NÃO CONGELA, a animação de morte termina fluida!)
    else if (victory) {
        endText.setString("YOU WIN!");
        endText.setFillColor(sf::Color::Cyan);
        
        // Sobe os créditos antecipadamente assim que o Boss começa a morrer
        if (creditsText.getPosition().y > 350.f) {
            creditsText.move(sf::Vector2f(0.f, -80.f * dt)); 
        }
    }
    // Condição Reserva: Se o Boss ainda não morreu, rola os créditos nos 5s finais da música
    else if (timeSurvived >= (maxMusicTime - 5.f) && timeSurvived < maxMusicTime) {
        if (creditsText.getPosition().y > 350.f) {
            creditsText.move(sf::Vector2f(0.f, -80.f * dt)); 
        }
    }
}

void Game::handleCollisions() {
    for (auto pIt = projectiles.begin(); pIt != projectiles.end(); ) {
        bool projectileDestroyed = false;

        if (pIt->isHeroOwned) {
            for (auto eIt = enemies.begin(); eIt != enemies.end(); ) {
                if (pIt->shape.getGlobalBounds().findIntersection(eIt->shape.getGlobalBounds()).has_value()) {
                    
                    int oldHp = eIt->hp; // Salva a vida antes do tiro
                    int oldTeleportHp = eIt->nextTeleportHp; // SALVA O GATILHO DO TELEPORTE

                    // Aplica o dano
                    if (pIt->isPowered) {
                        eIt->takeDamage(60); 
                    } else {
                        eIt->takeDamage(20); 
                    }

                    // --- DROP DE MUNIÇÃO A CADA TELEPORTE ---
                    // Se o alvo de teleporte diminuiu, significa que ele acabou de piscar na tela!
                    if (eIt->type == EnemyType::FinalBoss && eIt->nextTeleportHp < oldTeleportHp) {
                        // Dropa uma munição normal um pouco abaixo de onde ele reapareceu
                        boosts.push_back(Boost(eIt->shape.getPosition().x, eIt->shape.getPosition().y + 40.f, BoostType::MegaAmmo));
                    }

                    // --- CHECAGEM DO DROP DE 50% DO BOSS (em 750 HP) ---
                    if (eIt->type == EnemyType::FinalBoss && oldHp > 750 && eIt->hp <= 750) {
                        // Ele dropa uma Poção e um PowerUp nos lados do chefe
                        boosts.push_back(Boost(eIt->shape.getPosition().x - 30.f, eIt->shape.getPosition().y, BoostType::Health));
                        boosts.push_back(Boost(eIt->shape.getPosition().x + 30.f, eIt->shape.getPosition().y, BoostType::PowerUp));
                    }
                    // SE O INIMIGO ZERAR A VIDA
                    if(eIt->hp <= 0) {
                        
                        // SE FOR O FINAL BOSS
                        if (eIt->type == EnemyType::FinalBoss) {
                            if (!eIt->isDying) {
                                eIt->isDying = true; // Inicia a animação
                                eIt->hp = 1;         // Trava a vida
                                victory = true;     // Gatilho de vitória (texto YOU WIN!)
                            }
                            ++eIt; // Apenas avança, não apaga ainda!
                        } 
                        // SE FOR INIMIGO COMUM OU MINIBOSS
                        else {
                            // Lógica das Odds: 60/20/10/10
                            int itemRoll = rand() % 100;
                            BoostType droppedType;

                            if (itemRoll < 60) { droppedType = BoostType::NormalAmmo; } 
                            else if (itemRoll < 80) { droppedType = BoostType::Health; } 
                            else if (itemRoll < 90) { droppedType = BoostType::MegaAmmo; } 
                            else { droppedType = BoostType::PowerUp; }

                            // Cria o item
                            boosts.push_back(Boost(eIt->shape.getPosition().x, eIt->shape.getPosition().y, droppedType));

                            eIt = enemies.erase(eIt); // Apaga o inimigo da lista
                        }
                    } else {
                        ++eIt; // Só incrementa se não zerou a vida
                    }
                    
                    projectileDestroyed = true;
                    break;
                } else { 
                    ++eIt; 
                }
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

}

void Game::render() {
    window.clear(sf::Color(220, 220, 220));
    if (finalBossSpawned && !finalBossDefeated) {
        window.draw(dangerBorder);
    } 
    window.draw(baseCore.shape);
    window.draw(hero.shape);
    for (auto& e : enemies) window.draw(e.shape);
    for (auto& p : projectiles) window.draw(p.shape);
    for (auto& boost : boosts) {
        window.draw(boost.shape);
    }
    
    // Desenha o HUD 
    window.draw(baseHpText);
    window.draw(hpText);
    window.draw(ammoText);
    window.draw(timerText);
    window.draw(bossHpText);
    window.draw(creditsText);
    if (gameOver || victory) {
        // Criar o retângulo de fundo
        sf::RectangleShape box(sf::Vector2f(400.f, 100.f));
        box.setFillColor(sf::Color(0, 0, 0, 200)); 
        box.setOrigin(sf::Vector2f(200.f, 50.f));
        box.setPosition(sf::Vector2f(400.f, 315.f));
        
        // Ajustar o texto para ficar centralizado e negrito
        endText.setStyle(sf::Text::Bold);
        endText.setOrigin(sf::Vector2f(endText.getLocalBounds().size.x / 2.f, endText.getLocalBounds().size.y / 2.f));
        endText.setPosition(sf::Vector2f(400.f, 300.f));

        window.draw(box);    // Desenha o fundo primeiro
        window.draw(endText);// Desenha o texto por cima
    }
    
    window.display();
}