#include "Unit.h"
#include <cmath>
#include <algorithm>

Unit::Unit(float x, float y, float range, float attackSpeed, int damage)
    : position(x, y), range(range), attackSpeed(attackSpeed), 
      damage(damage), attackTimer(0.0f), cost(50), showRange(true) {
    
    shape.setRadius(12.0f);
    shape.setPosition({position.x - 12, position.y - 12});
    shape.setFillColor(sf::Color(50, 150, 50));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    
    // Horizontal range indicator - extends left from unit position
    rangeIndicator.setSize({range, 800.0f}); // Full screen height, range width
    rangeIndicator.setPosition({position.x - range, 0.0f});
    rangeIndicator.setFillColor(sf::Color(100, 100, 255, 50));
    rangeIndicator.setOutlineColor(sf::Color(100, 100, 255));
    rangeIndicator.setOutlineThickness(1.0f);
}

void Unit::update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies) {
    attackTimer += deltaTime;
    
    if (attackTimer >= attackSpeed) {
        Enemy* target = findNearestEnemy(enemies);
        if (target) {
            target->takeDamage(damage);
            attackTimer = 0.0f;
        }
    }
}

Enemy* Unit::findNearestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies) {
    Enemy* nearest = nullptr;
    float nearestDist = range;
    
    for (auto& enemy : enemies) {
        if (enemy->isDead() || enemy->hasReachedCastle()) continue;
        
        // Horizontal range only - check if enemy is within range to the left
        float dx = position.x - enemy->getX(); // Distance from unit to enemy (leftward)
        
        if (dx >= 0 && dx <= range && dx < nearestDist) {
            nearestDist = dx;
            nearest = enemy.get();
        }
    }
    
    return nearest;
}

sf::FloatRect Unit::getBounds() const {
    return shape.getGlobalBounds();
}

void Unit::draw(sf::RenderTarget& target) {
    // Always show horizontal range indicator
    // Update range indicator size and position in case range changed
    rangeIndicator.setSize({range, 800.0f});
    rangeIndicator.setPosition({position.x - range, 0.0f});
    target.draw(rangeIndicator);
    target.draw(shape);
}
