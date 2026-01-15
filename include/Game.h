#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Castle.h"
#include "Enemy.h"
#include "Unit.h"
#include "Tree.h"
#include "Shop.h"
#include "ResourceManager.h"

class Game {
public:
    Game();
    ~Game() = default;
    
    void run();
    
private:
    void processEvents();
    void update(float deltaTime);
    void render();
    
    void spawnEnemy();
    void handleMouseClick(sf::Vector2f mousePos);
    void handleShopClick(sf::Vector2f mousePos);
    
    sf::RenderWindow window;
    sf::Clock clock;
    
    Castle castle;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Unit>> units;
    std::vector<std::unique_ptr<Tree>> trees;
    Shop shop;
    
    float enemySpawnTimer;
    float enemySpawnInterval;
    int currentWave;
    float waveTimer;
    bool gameOver;
    bool gameWon;
    
    sf::Font font;
    sf::Text waveText;
    sf::Text gameOverText;
    sf::Text instructionsText;
    
    // Note: Text objects must be initialized with font in constructor
};
