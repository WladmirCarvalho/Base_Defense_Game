#include "../include/Game.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

Game::Game() : window(sf::VideoMode({800, 600}), "Base Defense"), 
               hero(400.f, 300.f), baseCore(400.f, 300.f), 
               hpText(font), ammoText(font), timerText(font), 
               baseHpText(font), endText(font), creditsText(font), bossHpText(font),
               moveHintText(font), shootHintText(font), objectiveHintText(font),
               gameOverText(font), finalTimeText(font),
               replayButtonText(font), exitButtonText(font),
               titleText(font), leaderboardTitleText(font),
               leaderboardText(font), startButtonText(font),
               newRecordText(font), nameEntryPromptText(font),
               nameEntryInputText(font),
               leaderboard("leaderboard.txt"), lastScore(0),
               timeSurvived(0.f), gameOver(false), victory(false),
               finalBossSpawned(false), finalBossDefeated(false), totalEnemiesSpawned(0),
               state(GameState::START) {

    // Inicia a semente de aleatoriedade
    srand(static_cast<unsigned int>(time(nullptr)));

    // Configuração da Música de fundo
    if (bgMusic.openFromFile("musica.ogg")) {

        bgMusic.setLooping(false);
        bgMusic.setVolume(50.f);
        maxMusicTime = bgMusic.getDuration().asSeconds();
    } else {
        std::cout << "Aviso: Arquivo de musica nao encontrado.\n";
        maxMusicTime = 120.f; // Tempo de segurança se a música der erro
    }

    // Configuração da Música do menu
    if (menuMusic.openFromFile("menu.wav")) {
        menuMusic.setLooping(true);
        menuMusic.setVolume(40.f);
        menuMusic.play();
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
    ammoText.setPosition(sf::Vector2f(600.f, 45.f));

    baseHpText.setCharacterSize(26);
    baseHpText.setFillColor(sf::Color::Black);
    baseHpText.setPosition(sf::Vector2f(600.f, 80.f));

    bossHpText.setCharacterSize(26);
    bossHpText.setFillColor(sf::Color::Red);
    bossHpText.setPosition(sf::Vector2f(600.f, 115.f));
    bossHpText.setString("");

    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::Black);
    timerText.setPosition(sf::Vector2f(350.f, 10.f));

    endText.setCharacterSize(50);
    endText.setFillColor(sf::Color::Black);
    endText.setString(""); 

    creditsText.setCharacterSize(24);
    creditsText.setFillColor(sf::Color::Black);
    creditsText.setString("Desenvolvido por:\nMarcos Gomes\nSibelle Galina\nWladmir Carvalho\n\nObrigado por jogar!");
    creditsText.setPosition(sf::Vector2f(20.f, 650.f)); 

    dangerBorder.setSize(sf::Vector2f(800.f, 600.f));
    dangerBorder.setPosition(sf::Vector2f(0.f, 0.f));
    dangerBorder.setFillColor(sf::Color::Transparent); 
    dangerBorder.setOutlineThickness(-30.f);
    dangerBorder.setOutlineColor(sf::Color::Transparent); 

    // --- Instruções na Tela Inicial) ---
    moveHintText.setString("Botao Direito do Mouse: Move o heroi para o local clicado.");
    moveHintText.setCharacterSize(18);
    moveHintText.setFillColor(sf::Color(80, 80, 80)); // Um cinza elegante
    centerText(moveHintText, 400.f, 380.f); // Centralizado no menu

    shootHintText.setString("Tecla 'Q': Dispara Projetil na direcao do cursor do mouse");
    shootHintText.setCharacterSize(18);
    shootHintText.setFillColor(sf::Color(80, 80, 80));
    centerText(shootHintText, 400.f, 410.f);

    objectiveHintText.setString("Objetivo: Elimine o Final Boss antes que o tempo acabe!");
    objectiveHintText.setCharacterSize(18);
    objectiveHintText.setFillColor(sf::Color::Red); // Vermelho para dar urgência
    objectiveHintText.setStyle(sf::Text::Bold);
    centerText(objectiveHintText, 400.f, 440.f);

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

    // Configuração da tela de novo recorde
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
                    bool powered = hero.hasPowerUp;
                    projectiles.push_back(Projectile(hero.shape.getPosition(), dir, true, powered));
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
    } // AQUI TERMINA O BLOCO ELSE DO SPAWN

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

    // --- ATUALIZAÇÃO DO HUD E TIMER REGRESSIVO ---
    int timeLeft = static_cast<int>(maxMusicTime - timeSurvived);
    if (timeLeft < 0) timeLeft = 0; // Trava no zero para não mostrar números negativos

    hpText.setString("Vida: " + std::to_string(hero.hp) + "/100");
    ammoText.setString("Municao: " + std::to_string(hero.ammo) + "/" + std::to_string(hero.maxAmmo));
    timerText.setString("Tempo: " + std::to_string(timeLeft) + "s"); // Agora mostra o tempo caindo!
    baseHpText.setString("Base: " + std::to_string(baseCore.hp));

    // --- LÓGICA DE FIM DE JOGO UNIFICADA ---
    if (baseCore.isDestroyed() || hero.hp <= 0) {
        gameOver = true;
    }
    
    // Condição Reserva: Se a música acabar e o Boss AINDA estiver vivo, é Game Over!
    if (timeSurvived >= maxMusicTime && !finalBossDefeated) {
        gameOver = true;
    }

    // Se o jogo acabou por derrota ou vitória
    if (gameOver || victory) {
        bgMusic.stop();
        menuMusic.play(); 
        
        lastScore = static_cast<int>(timeSurvived);
        
        // Define se foi Vitória ou Derrota na tela final
        if (victory) {
            gameOverText.setString("YOU WIN!");
            gameOverText.setFillColor(sf::Color::Cyan); 
            finalTimeText.setString("Tempo de conclusao: " + std::to_string(lastScore) + "s");
        } else {
            gameOverText.setString("GAME OVER");
            gameOverText.setFillColor(sf::Color::Red);
            finalTimeText.setString("Tempo sobrevivido: " + std::to_string(lastScore) + "s");
        }
        
        centerText(gameOverText, 400.f, 200.f); // Re-centraliza a palavra
        centerText(finalTimeText, 400.f, 270.f);

        // SÓ entra no Scoreboard se VENCEU o jogo e tem pontuação
        if (victory && leaderboard.qualifies(lastScore)) {
            playerNameInput.clear();
            nameEntryInputText.setString("_");
            centerText(nameEntryInputText, 400.f, 380.f);
            
            newRecordText.setCharacterSize(24);
            centerText(newRecordText, 400.f, 235.f);

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
    boosts.clear();
    gameOver = false;
    victory = false;
    finalBossSpawned = false;
    finalBossDefeated = false;

    while (!spawnQueue.empty()) spawnQueue.pop();
    spawnQueue.push(2.0f);
    spawnQueue.push(4.0f);
    spawnQueue.push(6.0f);

    timeSurvived = 0.f;
    state = GameState::PLAYING;

    menuMusic.stop(); //pára músca do menu

    bgMusic.stop(); // Garante que a trilha volte do zero se o jogador estiver dando "Replay"
    bgMusic.play(); // Inicia a música em sincronia com o timeSurvived
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
    if (state == GameState::START) {
        renderStart();
    } else {
        if (finalBossSpawned && !finalBossDefeated && state == GameState::PLAYING) {
            window.draw(dangerBorder);
        } 
        window.draw(baseCore.shape);
        window.draw(hero.shape);
        for (auto& e : enemies) window.draw(e.shape);
        for (auto& p : projectiles) window.draw(p.shape);
        
        for (auto& boost : boosts) {
            window.draw(boost.shape);
        }
        
        // Desenha o HUD de Gameplay (SEM as instruções para não poluir!)
        window.draw(baseHpText);
        window.draw(hpText);
        window.draw(ammoText);
        window.draw(timerText);
        window.draw(bossHpText);

        if (victory && state == GameState::PLAYING) {
            window.draw(creditsText);
        }
        
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
    window.draw(moveHintText);
    window.draw(shootHintText);
    window.draw(objectiveHintText);
}

void Game::renderGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(800.f, 600.f));
    overlay.setFillColor(sf::Color(0, 0, 0, 160));
    window.draw(overlay);

    // --- NOVO: A caixinha semi-transparente de destaque ---
    sf::RectangleShape box(sf::Vector2f(400.f, 80.f));
    box.setFillColor(sf::Color(0, 0, 0, 200)); // Fundo preto escuro
    box.setOutlineColor(gameOverText.getFillColor()); // Puxa a cor do texto (Azul ou Vermelho!)
    box.setOutlineThickness(2.f);
    box.setOrigin(sf::Vector2f(200.f, 40.f));
    box.setPosition(sf::Vector2f(400.f, 200.f));
    window.draw(box);

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

    // --- A caixinha semi-transparente de destaque ---
    sf::RectangleShape box(sf::Vector2f(400.f, 80.f));
    box.setFillColor(sf::Color(0, 0, 0, 200));
    box.setOutlineColor(gameOverText.getFillColor());
    box.setOutlineThickness(2.f);
    box.setOrigin(sf::Vector2f(200.f, 40.f));
    box.setPosition(sf::Vector2f(400.f, 200.f));
    window.draw(box);

    // Desenha a palavra YOU WIN ou GAME OVER por cima da caixa
    window.draw(gameOverText); 
    
    window.draw(newRecordText);
    window.draw(finalTimeText);
    window.draw(nameEntryPromptText);
    window.draw(nameEntryInputText);
}
