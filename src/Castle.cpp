#include "Castle.h"
#include <sstream>

Castle::Castle(float x, float y, float width, float height) 
    : maxHP(1000), hp(maxHP), font(), hpText(font) {
    shape.setPosition({x, y});
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color(139, 69, 19)); // Brown
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(2.0f);
    
    // Try to load font, if fails we'll skip text rendering
    if (!font.openFromFile("assets/fonts/arial.ttf")) {
        (void)font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
    
    hpText.setCharacterSize(20);
    hpText.setFillColor(sf::Color::White);
    hpText.setPosition({x + 10, y - 30});
}

void Castle::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

void Castle::draw(sf::RenderTarget& target) {
    target.draw(shape);
    
    if (font.getInfo().family != "") {
        std::stringstream ss;
        ss << "HP: " << hp << "/" << maxHP;
        hpText.setString(ss.str());
        target.draw(hpText);
    }
}
