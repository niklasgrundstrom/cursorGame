#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"

class Unit {
public:
    Unit(float x, float y, float range, float attackSpeed, int damage);
    
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderTarget& target);
    
    sf::FloatRect getBounds() const;
    int getCost() const { return cost; }
    void setRange(float newRange) { range = newRange; }
    float getRange() const { return range; }
    
    static int getBaseCost() { return 50; }

private:
    sf::Vector2f position;
    float range;
    float attackSpeed;
    int damage;
    float attackTimer;
    int cost;
    sf::CircleShape shape;
    sf::RectangleShape rangeIndicator;  // Horizontal range indicator
    bool showRange;
    
    Enemy* findNearestEnemy(std::vector<std::unique_ptr<Enemy>>& enemies);
};
