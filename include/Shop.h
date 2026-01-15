#pragma once
#include <SFML/Graphics.hpp>
#include "Unit.h"
#include "ResourceManager.h"

struct Upgrade {
    std::string name;
    int cost;
    int level;
    int baseCost;
    float multiplier;
};

class Shop {
public:
    Shop();
    
    void update(float deltaTime, sf::RenderTarget& target, sf::Vector2f mousePos);
    void draw(sf::RenderTarget& target);
    
    bool buyClickDamageUpgrade();
    bool buyUnit();
    bool buyTree();
    bool upgradeUnitDamage();
    bool upgradeUnitRange();
    
    int getClickDamage() const { return clickDamage; }
    int getUnitDamageUpgrade() const { return unitDamageUpgrade.level; }
    int getUnitRangeUpgrade() const { return unitRangeUpgrade.level; }
    float getUnitRangeForLevel(int level) const;
    
    bool isOpen() const { return shopOpen; }
    void toggle() { shopOpen = !shopOpen; }
    void setOpen(bool open) { shopOpen = open; }
    
    void reset();

private:
    int clickDamage;
    Upgrade clickDamageUpgrade;
    Upgrade unitDamageUpgrade;
    Upgrade unitRangeUpgrade;
    bool shopOpen;
    
    sf::RectangleShape shopPanel;
    sf::Font font;
    std::vector<sf::Text> shopTexts;
    std::vector<sf::RectangleShape> shopButtons;
    
    bool isMouseOverButton(int buttonIndex, sf::Vector2f mousePos);
    void updateShopUI();
};
