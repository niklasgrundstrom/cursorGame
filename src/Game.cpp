#include "Game.h"
#include "EnemyTypes.h"
#include <sstream>
#include <cmath>
#include <algorithm>

Game::Game() 
    : window(sf::VideoMode({1200, 800}), "Defend The Castle"),
      castle(1100, 300, 80, 200),
      enemySpawnTimer(0.0f),
      enemySpawnInterval(2.0f),
      currentWave(1),
      waveTimer(0.0f),
      gameOver(false),
      gameWon(false),
      font(),
      waveText(font),
      gameOverText(font),
      instructionsText(font) {
    
    // Start with one tree
    trees.push_back(std::make_unique<Tree>(100.0f, 500.0f, 5.0f)); // 5 gold per second
    
    window.setFramerateLimit(60);
    
    if (!font.openFromFile("assets/fonts/arial.ttf")) {
        (void)font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
    
    waveText.setCharacterSize(24);
    waveText.setFillColor(sf::Color::White);
    waveText.setPosition({500, 10});
    
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({400, 350});
    gameOverText.setString("GAME OVER - Castle Destroyed!");
    
    instructionsText.setCharacterSize(18);
    instructionsText.setFillColor(sf::Color::White);
    instructionsText.setPosition({10, 420});
    instructionsText.setString("Click enemies to damage them\nPress S to toggle shop\nBuy units and upgrades in the shop");
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        processEvents();
        
        if (!gameOver) {
            update(deltaTime);
        }
        
        render();
    }
}

void Game::processEvents() {
    for (auto event = window.pollEvent(); event.has_value(); event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        // Toggle shop with 'S' key
        if (auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::S) {
                shop.toggle();
            }
        }
        
        if (auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left) {
                sf::Vector2f mousePos(
                    static_cast<float>(mousePressed->position.x),
                    static_cast<float>(mousePressed->position.y)
                );
                
                // Check if clicking in shop area (only if shop is open)
                if (shop.isOpen() && mousePos.x < 320 && mousePos.y < 420) {
                    handleShopClick(mousePos);
                } else {
                    handleMouseClick(mousePos);
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    // Update wave system
    waveTimer += deltaTime;
    if (waveTimer >= 30.0f) { // New wave every 30 seconds
        currentWave++;
        waveTimer = 0.0f;
        enemySpawnInterval = std::max(0.5f, enemySpawnInterval * 0.9f); // Spawn faster each wave
    }
    
    // Spawn enemies
    enemySpawnTimer += deltaTime;
    if (enemySpawnTimer >= enemySpawnInterval) {
        spawnEnemy();
        enemySpawnTimer = 0.0f;
    }
    
    // Update enemies
    for (auto& enemy : enemies) {
        enemy->update(deltaTime, castle.getShape().getPosition().x);
        
        // Check if enemy reached castle
        if (enemy->hasReachedCastle() && !enemy->isDead()) {
            castle.takeDamage(enemy->getDamage());
        }
    }
    
    // Remove dead enemies (no gold reward - trees generate income instead)
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [this](const std::unique_ptr<Enemy>& enemy) {
                return enemy->isDead();
            }),
        enemies.end()
    );
    
    // Update trees to generate gold
    for (auto& tree : trees) {
        tree->update(deltaTime);
    }
    
    // Update units
    for (auto& unit : units) {
        unit->update(deltaTime, enemies);
    }
    
    // Update shop (only if open)
    if (shop.isOpen()) {
        sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos(
            static_cast<float>(mousePosInt.x),
            static_cast<float>(mousePosInt.y)
        );
        shop.update(deltaTime, window, mousePos);
    }
    
    // Check game over
    if (castle.isDestroyed()) {
        gameOver = true;
    }
}

void Game::spawnEnemy() {
    float spawnY = 100.0f + (rand() % 600); // Random Y position
    auto enemy = createEnemy(currentWave, 0.0f, spawnY);
    enemies.push_back(std::move(enemy));
}

void Game::handleMouseClick(sf::Vector2f mousePos) {
    // Check if clicking on an enemy
    for (auto& enemy : enemies) {
        if (enemy->getBounds().contains(mousePos) && !enemy->isDead()) {
            enemy->takeDamage(shop.getClickDamage());
            break; // Only damage one enemy per click
        }
    }
    
    // Check if clicking to place a unit (if we just bought one)
    // For now, units are placed automatically near the castle
}

void Game::handleShopClick(sf::Vector2f mousePos) {
    // Check which button was clicked
    sf::Vector2f relativePos = mousePos - sf::Vector2f(20, 50);
    
    int buttonHeight = 50;
    int buttonSpacing = 60;
    
    if (relativePos.y >= 0 && relativePos.y < buttonHeight) {
        // Click Damage Upgrade
        shop.buyClickDamageUpgrade();
    } else if (relativePos.y >= buttonSpacing && relativePos.y < buttonSpacing + buttonHeight) {
        // Unit Damage Upgrade
        shop.upgradeUnitDamage();
    } else if (relativePos.y >= buttonSpacing * 2 && relativePos.y < buttonSpacing * 2 + buttonHeight) {
        // Unit Range Upgrade
        if (shop.upgradeUnitRange()) {
            // Update all existing units with new range
            float newRange = shop.getUnitRangeForLevel(shop.getUnitRangeUpgrade());
            for (auto& unit : units) {
                unit->setRange(newRange);
            }
        }
    } else if (relativePos.y >= buttonSpacing * 3 && relativePos.y < buttonSpacing * 3 + buttonHeight) {
        // Buy Unit
        if (shop.buyUnit()) {
            // Place unit near castle
            float unitY = 200.0f + (units.size() * 60.0f);
            if (unitY > 700) unitY = 200.0f + ((units.size() % 8) * 60.0f);
            
            float baseDamage = 20;
            // Base range is half screen width (600px), full range (1200px) after 3 upgrades
            float baseRange = 600.0f; // Half screen width
            float fullRange = 1200.0f; // Full screen width
            int damageUpgrade = shop.getUnitDamageUpgrade();
            int rangeUpgrade = shop.getUnitRangeUpgrade();
            
            float unitDamage = baseDamage * std::pow(1.3f, damageUpgrade - 1);
            // Range increases linearly: base (600) + (upgradeLevel - 1) * (1200 - 600) / 3
            // Level 1: 600, Level 2: 800, Level 3: 1000, Level 4: 1200 (full screen)
            float unitRange = shop.getUnitRangeForLevel(rangeUpgrade);
            
            units.push_back(std::make_unique<Unit>(
                1050.0f, unitY, unitRange, 1.0f, static_cast<int>(unitDamage)
            ));
        }
    } else if (relativePos.y >= buttonSpacing * 4 && relativePos.y < buttonSpacing * 4 + buttonHeight) {
        // Buy Tree
        if (shop.buyTree()) {
            // Place tree in a grid pattern
            int treeCount = static_cast<int>(trees.size());
            float treeX = 100.0f + ((treeCount % 5) * 80.0f);
            float treeY = 500.0f + ((treeCount / 5) * 100.0f);
            if (treeY > 750) treeY = 500.0f + ((treeCount % 10) * 50.0f);
            
            trees.push_back(std::make_unique<Tree>(treeX, treeY, 5.0f)); // 5 gold per second
        }
    }
}

void Game::render() {
    window.clear(sf::Color(50, 100, 50)); // Green background (grass)
    
    // Draw ground line
    sf::RectangleShape groundLine;
    groundLine.setSize(sf::Vector2f(1200, 2));
    groundLine.setPosition({0, 400});
    groundLine.setFillColor(sf::Color(100, 100, 100));
    window.draw(groundLine);
    
    // Draw trees
    for (auto& tree : trees) {
        tree->draw(window);
    }
    
    // Draw units
    for (auto& unit : units) {
        unit->draw(window);
    }
    
    // Draw enemies
    for (auto& enemy : enemies) {
        enemy->draw(window);
    }
    
    // Draw castle
    castle.draw(window);
    
    // Draw shop
    shop.draw(window);
    
    // Draw wave info
    std::stringstream ss;
    ss << "Wave: " << currentWave;
    waveText.setString(ss.str());
    window.draw(waveText);
    
    // Draw instructions
    window.draw(instructionsText);
    
    // Draw game over
    if (gameOver) {
        window.draw(gameOverText);
    }
    
    window.display();
}
