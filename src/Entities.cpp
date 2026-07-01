#include "../include/Entities.hpp"

// Projectile
Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f dir, bool heroOwned, bool isPowered) 
    : direction(dir), speed(400.f), distanceTraveled(0.f), maxRange(300.f), isHeroOwned(heroOwned), isPowered(isPowered) {
    
    if (isPowered) {
        shape.setRadius(10.f); // Tiro bem maior
        shape.setFillColor(sf::Color::Cyan);
    } else {
        shape.setRadius(5.f);  // Tiro normal
        shape.setFillColor(isHeroOwned ? sf::Color::Yellow : sf::Color::Magenta);
    }
    
    shape.setPosition(startPos);
}
void Projectile::update(float dt) {
    sf::Vector2f moveVec = direction * speed * dt;
    shape.move(moveVec);
    distanceTraveled += getDistance(sf::Vector2f(0,0), moveVec);
}
bool Projectile::isExpired() const { return distanceTraveled >= maxRange; }

// Base
Base::Base(float x, float y) : hp(500), maxHp(500), regenTimer(0.f) {
    shape.setSize(sf::Vector2f(100.f, 100.f));
    shape.setFillColor(sf::Color(139, 69, 19));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(10.f);
    shape.setOrigin(sf::Vector2f(50.f, 50.f));
    shape.setPosition(sf::Vector2f(x, y));
}
void Base::update(float dt) {
    regenTimer += dt;
    if (regenTimer >= 5.0f && hp < maxHp) { 
        hp += 15; // taxa de cura
        if (hp > maxHp) hp = maxHp; // Garante que não ultrapasse o teto
        regenTimer = 0.f; 
    }
    
    // Calcula a porcentagem real da vida (ex: 250 / 500 = 0.5)
    float hpPercent = static_cast<float>(hp) / maxHp;

    if (hpPercent > 0.5f) { 
        shape.setOutlineThickness(15.f);  // Maior que 50% = Muro grosso
    } else if (hpPercent > 0.2f) { 
        shape.setOutlineThickness(10.f);  // Entre 20% e 50% = Muro médio
    } else if (hp > 0) { 
        shape.setOutlineThickness(5.f);   // Menor que 20% = Muro fino
    } else { 
        shape.setOutlineThickness(0.f);   // Destruída = Sem muro
    }
}
void Base::takeDamage(int dmg) { hp -= dmg; }
bool Base::isDestroyed() const { return hp <= 0; }

// Hero
Hero::Hero(float startX, float startY) : hp(100), ammo(50), maxAmmo(150), speed(200.f), isMoving(false), invulTimer(0.f), 
hasPowerUp(false), powerUpTimer(0.f) {
    shape.setRadius(15.f);
    shape.setFillColor(sf::Color::Blue);
    shape.setOrigin(sf::Vector2f(15.f, 15.f));
    shape.setPosition(sf::Vector2f(startX, startY));
    targetPos = sf::Vector2f(startX, startY);
}
void Hero::setTarget(sf::Vector2f target) { targetPos = target; isMoving = true; }
void Hero::update(float dt) {
    if (invulTimer > 0.f) invulTimer -= dt;
    if (isMoving) {
        float dist = getDistance(shape.getPosition(), targetPos);
        if (dist > 5.0f) {
            sf::Vector2f dir = normalizeVector(targetPos - shape.getPosition());
            shape.move(dir * speed * dt);
        } else { isMoving = false; }
    }
    if (hasPowerUp) {
        powerUpTimer -= dt;
        if (powerUpTimer <= 0.f) {
            hasPowerUp = false;
        }
    }
}
void Hero::takeDamage(int dmg) { 
    if (invulTimer <= 0.f) {
        hp -= dmg; 
        invulTimer = 0.5f; // Fica invulnerável por meio segundo
    }
}

// Enemy
Enemy::Enemy(sf::Vector2f startPos, EnemyType type) : type(type), shootTimer(0.f), isDying(false), deathTimer(2.0f) {
    shape.setPosition(startPos);

    if (type == EnemyType::Common) {
        shape.setRadius(15.f);
        shape.setFillColor(sf::Color::Red);
        hp = 20;
        speed = 50.f;
        damage = 5;
        shootInterval = 3.0f; // Atira a cada 3 segundos
    } 
    else if (type == EnemyType::MiniBoss) {
        shape.setRadius(30.f); // 2x maior fisicamente
        shape.setFillColor(sf::Color(255, 140, 0)); // Laranja para destacar
        hp = 200; // Vida maior (leva 10 tiros de -20hp)
        speed = 50.f; // Movimentação 2x mais lenta
        shootInterval = 2.0f; // Frequência de tiro maior
        damage = 10;
    }
    else if (type == EnemyType::FinalBoss) {
        shape.setRadius(90.f); // 8x maior que Common
        shape.setFillColor(sf::Color::Red);       // Vermelho
        shape.setOutlineColor(sf::Color::Black);  // Borda Preta
        shape.setOutlineThickness(5.f);           // Espessura da borda
        hp = 1500; // 75 tiros de -20hp
        speed = 25.f; 
        shootInterval = 1.0f; 
        damage = 20;
        nextTeleportHp = 1350; // Primeiro teleporte após perder 150 HP
    }
    
    // Centraliza a origem de acordo com o raio escolhido
    shape.setOrigin(sf::Vector2f(shape.getRadius(), shape.getRadius()));
}

void Enemy::update(float dt, sf::Vector2f targetPos) {
    // --- LÓGICA DE ANIMAÇÃO DE MORTE ---
    if (isDying) {
        deathTimer -= dt;
        
        // Efeito dramático: Gira rapidamente e vai encolhendo até 0
        shape.rotate(sf::degrees(720.f * dt)); 
        float scale = std::max(0.0f, deathTimer / 2.0f); // Reduz de 1.0 para 0.0 em 2 segundos
        shape.setScale(sf::Vector2f(scale, scale));
        
        if (deathTimer <= 0.f) {
            hp = 0; // Finaliza a vida de vez
        }
        return; // Retorna para que ele não se mova mais enquanto morre
    }
    sf::Vector2f dir = normalizeVector(targetPos - shape.getPosition());
    shape.move(dir * speed * dt);
}

bool Enemy::canShoot(float dt) {
    if (isDying) return false;
    shootTimer += dt;
    if (shootTimer >= shootInterval) { shootTimer = 0.f; return true; }
    return false;
}

// Boost de Vida e Ammo
Boost::Boost(float x, float y, BoostType type) : type(type), lifetime(8.0f) {
    shape.setSize(sf::Vector2f(20.f,20.f)); // Mantém quadrado padrão
    shape.setPosition(sf::Vector2f(x, y));

    if (type == BoostType::NormalAmmo) {
        shape.setSize(sf::Vector2f(15.f, 15.f)); // Munição normal é um pouco menor
        shape.setFillColor(sf::Color::Green);    // Cor da munição normal
    } else if (type == BoostType::Health) {
        shape.setFillColor(sf::Color::Magenta);     // Saúde
    } else if (type == BoostType::MegaAmmo) {
        shape.setFillColor(sf::Color::Yellow);   // Munição Extra
    } else if (type == BoostType::PowerUp) {
        shape.setFillColor(sf::Color::Cyan);      // PowerUp
    }
}

void Boost::update(float dt) {
    lifetime -= dt; // O tempo de vida vai diminuindo
}

void Enemy::takeDamage(int dmg) {
    hp -= dmg;
    
    // Se for o FinalBoss e ele ainda estiver vivo
    if (type == EnemyType::FinalBoss && hp > 0) {
        // Se a vida desceu abaixo do gatilho
        if (hp <= nextTeleportHp) {
            // Sorteia nova posição na tela (evitando as bordas extremas)
            float newX = static_cast<float>(rand() % 700 + 50); 
            float newY = static_cast<float>(rand() % 500 + 50);
            shape.setPosition(sf::Vector2f(newX, newY));
            
            // Prepara o próximo gatilho (a cada 20 de dano ele pula)
            nextTeleportHp -= 150; 
        }
    }
}