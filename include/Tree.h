#pragma once
#include <SFML/Graphics.hpp>

class Tree {
public:
    Tree(float x, float y, float goldPerSecond);
    
    void update(float deltaTime);
    void draw(sf::RenderTarget& target);
    
    float getGoldGenerated() const { return goldGenerated; }
    void resetGoldGenerated() { goldGenerated = 0.0f; }
    float getGoldPerSecond() const { return goldPerSecond; }
    
    sf::FloatRect getBounds() const;
    float getX() const { return position.x; }
    float getY() const { return position.y; }
    
    static int getBaseCost() { return 100; }

private:
    sf::Vector2f position;
    float goldPerSecond;
    float goldGenerated;
    float timer;
    
    sf::CircleShape trunk;
    sf::CircleShape leaves;
};
