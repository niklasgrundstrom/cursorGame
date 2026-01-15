#pragma once
#include <SFML/Graphics.hpp>

class Castle {
public:
    Castle(float x, float y, float width, float height);
    
    void takeDamage(int damage);
    bool isDestroyed() const { return hp <= 0; }
    int getHP() const { return hp; }
    int getMaxHP() const { return maxHP; }
    
    sf::RectangleShape& getShape() { return shape; }
    const sf::RectangleShape& getShape() const { return shape; }
    
    void draw(sf::RenderTarget& target);

private:
    int hp;
    int maxHP;
    sf::RectangleShape shape;
    sf::Font font;
    sf::Text hpText;
};
