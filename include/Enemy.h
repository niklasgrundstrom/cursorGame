#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Enemy {
public:
    Enemy(float x, float y, float speed, int maxHP, int damage, bool flying);
    virtual ~Enemy() = default;
    
    virtual void update(float deltaTime, float castleX);
    virtual void draw(sf::RenderTarget& target);
    
    void takeDamage(int damage);
    bool isDead() const { return hp <= 0; }
    bool hasReachedCastle() const { return reachedCastle; }
    
    sf::FloatRect getBounds() const;
    int getDamage() const { return damage; }
    int getReward() const { return reward; }
    bool isFlying() const { return flying; }
    
    float getX() const { return position.x; }
    float getY() const { return position.y; }

protected:
    sf::Vector2f position;
    float speed;
    int hp;
    int maxHP;
    int damage;
    bool flying;
    bool reachedCastle;
    int reward;
    sf::CircleShape shape;
    sf::RectangleShape hpBar;
    sf::RectangleShape hpBarBackground;
};
