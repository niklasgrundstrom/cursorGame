#include "EnemyTypes.h"
#include <random>
#include <cmath>

BasicEnemy::BasicEnemy(float x, float y)
    : Enemy(x, y, 50.0f, 50, 5, false) {
    reward = 10;
}

FastEnemy::FastEnemy(float x, float y)
    : Enemy(x, y, 100.0f, 30, 3, false) {
    reward = 15;
    shape.setFillColor(sf::Color(255, 100, 100));
}

TankEnemy::TankEnemy(float x, float y)
    : Enemy(x, y, 30.0f, 200, 15, false) {
    reward = 30;
    shape.setFillColor(sf::Color(100, 50, 50));
    shape.setRadius(25.0f);
}

FlyingEnemy::FlyingEnemy(float x, float y)
    : Enemy(x, y, 60.0f, 40, 8, true) {
    reward = 20;
}

std::unique_ptr<Enemy> createEnemy(int wave, float x, float y) {
    // Progressive difficulty: later waves have tougher enemies
    std::random_device rd;
    std::mt19937 gen(rd());
    
    int enemyType = 0;
    float difficulty = wave * 0.1f;
    
    if (wave <= 3) {
        // Early waves: mostly basic enemies
        std::uniform_int_distribution<> dis(0, 100);
        int roll = dis(gen);
        if (roll < 80) enemyType = 0; // Basic
        else enemyType = 1; // Fast
    } else if (wave <= 7) {
        // Mid waves: mix of types
        std::uniform_int_distribution<> dis(0, 100);
        int roll = dis(gen);
        if (roll < 50) enemyType = 0; // Basic
        else if (roll < 80) enemyType = 1; // Fast
        else if (roll < 95) enemyType = 2; // Tank
        else enemyType = 3; // Flying
    } else {
        // Late waves: more variety
        std::uniform_int_distribution<> dis(0, 100);
        int roll = dis(gen);
        if (roll < 30) enemyType = 0; // Basic
        else if (roll < 50) enemyType = 1; // Fast
        else if (roll < 75) enemyType = 2; // Tank
        else enemyType = 3; // Flying
    }
    
    std::unique_ptr<Enemy> enemy;
    switch (enemyType) {
        case 0:
            enemy = std::make_unique<BasicEnemy>(x, y);
            break;
        case 1:
            enemy = std::make_unique<FastEnemy>(x, y);
            break;
        case 2:
            enemy = std::make_unique<TankEnemy>(x, y);
            break;
        case 3:
            enemy = std::make_unique<FlyingEnemy>(x, y);
            break;
        default:
            enemy = std::make_unique<BasicEnemy>(x, y);
    }
    
    // Scale enemy stats based on wave
    // This makes enemies progressively tougher
    return enemy;
}
