#include "Enemy.h"
#include <algorithm>

Enemy::Enemy(float x, float y, float speed, int maxHP, int damage, bool flying)
    : position(x, y), speed(speed), hp(maxHP), maxHP(maxHP), 
      damage(damage), flying(flying), reachedCastle(false), reward(10) {
    
    float radius = flying ? 15.0f : 20.0f;
    shape.setRadius(radius);
    shape.setPosition({position.x - radius, position.y - radius});
    shape.setFillColor(flying ? sf::Color(100, 100, 255) : sf::Color(200, 50, 50));
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    
    hpBarBackground.setSize(sf::Vector2f(radius * 2, 4));
    hpBarBackground.setFillColor(sf::Color(50, 50, 50));
    
    hpBar.setSize(sf::Vector2f(radius * 2, 4));
    hpBar.setFillColor(sf::Color::Green);
}

void Enemy::update(float deltaTime, float castleX) {
    if (reachedCastle) return;
    
    float moveDistance = speed * deltaTime;
    position.x += moveDistance;
    
    if (position.x >= castleX) {
        position.x = castleX;
        reachedCastle = true;
    }
    
    float radius = shape.getRadius();
    shape.setPosition({position.x - radius, position.y - radius});
    hpBarBackground.setPosition({position.x - radius, position.y - radius - 8});
    hpBar.setPosition({position.x - radius, position.y - radius - 8});
    
    float hpPercent = static_cast<float>(hp) / maxHP;
    hpBar.setSize(sf::Vector2f(radius * 2 * hpPercent, 4));
}

void Enemy::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

sf::FloatRect Enemy::getBounds() const {
    return shape.getGlobalBounds();
}

void Enemy::draw(sf::RenderTarget& target) {
    target.draw(shape);
    target.draw(hpBarBackground);
    target.draw(hpBar);
}
