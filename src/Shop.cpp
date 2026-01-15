#include "Shop.h"
#include "Tree.h"
#include <sstream>
#include <cmath>

Shop::Shop() : shopOpen(true) {
    clickDamage = 10;
    clickDamageUpgrade = {"Click Damage", 50, 1, 50, 1.5f};
    unitDamageUpgrade = {"Unit Damage", 100, 1, 100, 1.3f};
    unitRangeUpgrade = {"Unit Range", 75, 1, 75, 1.2f};
    
    shopPanel.setSize(sf::Vector2f(300, 400));
    shopPanel.setPosition({10.0f, 10.0f});
    shopPanel.setFillColor(sf::Color(50, 50, 50, 200));
    shopPanel.setOutlineColor(sf::Color::White);
    shopPanel.setOutlineThickness(2.0f);
    
    if (!font.openFromFile("assets/fonts/arial.ttf")) {
        (void)font.openFromFile("C:/Windows/Fonts/arial.ttf");
    }
    
    updateShopUI();
}

void Shop::updateShopUI() {
    shopTexts.clear();
    shopButtons.clear();
    
    int yOffset = 50;
    int buttonHeight = 50;
    int buttonSpacing = 60;
    
    // Click Damage Upgrade
    sf::RectangleShape button1;
    button1.setSize(sf::Vector2f(280, buttonHeight));
    button1.setPosition({20.0f, static_cast<float>(yOffset)});
    button1.setFillColor(sf::Color(100, 100, 150));
    button1.setOutlineColor(sf::Color::White);
    button1.setOutlineThickness(1.0f);
    shopButtons.push_back(button1);
    
    sf::Text text1(font);
    text1.setCharacterSize(16);
    text1.setFillColor(sf::Color::White);
    std::stringstream ss1;
    ss1 << "Click Damage Lv" << clickDamageUpgrade.level << " - $" << clickDamageUpgrade.cost;
    text1.setString(ss1.str());
    text1.setPosition({30.0f, static_cast<float>(yOffset + 15)});
    shopTexts.push_back(text1);
    
    yOffset += buttonSpacing;
    
    // Unit Damage Upgrade
    sf::RectangleShape button2;
    button2.setSize(sf::Vector2f(280, buttonHeight));
    button2.setPosition({20.0f, static_cast<float>(yOffset)});
    button2.setFillColor(sf::Color(100, 100, 150));
    button2.setOutlineColor(sf::Color::White);
    button2.setOutlineThickness(1.0f);
    shopButtons.push_back(button2);
    
    sf::Text text2(font);
    text2.setCharacterSize(16);
    text2.setFillColor(sf::Color::White);
    std::stringstream ss2;
    ss2 << "Unit Damage Lv" << unitDamageUpgrade.level << " - $" << unitDamageUpgrade.cost;
    text2.setString(ss2.str());
    text2.setPosition({30.0f, static_cast<float>(yOffset + 15)});
    shopTexts.push_back(text2);
    
    yOffset += buttonSpacing;
    
    // Unit Range Upgrade
    sf::RectangleShape button3;
    button3.setSize(sf::Vector2f(280, buttonHeight));
    button3.setPosition({20.0f, static_cast<float>(yOffset)});
    button3.setFillColor(sf::Color(100, 100, 150));
    button3.setOutlineColor(sf::Color::White);
    button3.setOutlineThickness(1.0f);
    shopButtons.push_back(button3);
    
    sf::Text text3(font);
    text3.setCharacterSize(16);
    text3.setFillColor(sf::Color::White);
    std::stringstream ss3;
    ss3 << "Unit Range Lv" << unitRangeUpgrade.level << " - $" << unitRangeUpgrade.cost;
    text3.setString(ss3.str());
    text3.setPosition({30.0f, static_cast<float>(yOffset + 15)});
    shopTexts.push_back(text3);
    
    yOffset += buttonSpacing;
    
    // Buy Unit Button
    sf::RectangleShape button4;
    button4.setSize(sf::Vector2f(280, buttonHeight));
    button4.setPosition({20.0f, static_cast<float>(yOffset)});
    button4.setFillColor(sf::Color(100, 150, 100));
    button4.setOutlineColor(sf::Color::White);
    button4.setOutlineThickness(1.0f);
    shopButtons.push_back(button4);
    
    sf::Text text4(font);
    text4.setCharacterSize(16);
    text4.setFillColor(sf::Color::White);
    std::stringstream ss4;
    ss4 << "Buy Unit - $" << Unit::getBaseCost();
    text4.setString(ss4.str());
    text4.setPosition({30.0f, static_cast<float>(yOffset + 15)});
    shopTexts.push_back(text4);
    
    yOffset += buttonSpacing;
    
    // Buy Tree Button
    sf::RectangleShape button5;
    button5.setSize(sf::Vector2f(280, buttonHeight));
    button5.setPosition({20.0f, static_cast<float>(yOffset)});
    button5.setFillColor(sf::Color(34, 139, 34));
    button5.setOutlineColor(sf::Color::White);
    button5.setOutlineThickness(1.0f);
    shopButtons.push_back(button5);
    
    sf::Text text5(font);
    text5.setCharacterSize(16);
    text5.setFillColor(sf::Color::White);
    std::stringstream ss5;
    ss5 << "Buy Tree - $" << Tree::getBaseCost() << " (5 gold/sec)";
    text5.setString(ss5.str());
    text5.setPosition({30.0f, static_cast<float>(yOffset + 15)});
    shopTexts.push_back(text5);
}

void Shop::update(float deltaTime, sf::RenderTarget& target, sf::Vector2f mousePos) {
    // Handle button hover effects
    for (size_t i = 0; i < shopButtons.size(); ++i) {
        if (isMouseOverButton(i, mousePos)) {
            shopButtons[i].setFillColor(sf::Color(150, 150, 200));
        } else {
            if (i == 3) {
                shopButtons[i].setFillColor(sf::Color(100, 150, 100));
            } else if (i == 4) {
                shopButtons[i].setFillColor(sf::Color(34, 139, 34));
            } else {
                shopButtons[i].setFillColor(sf::Color(100, 100, 150));
            }
        }
    }
}

bool Shop::isMouseOverButton(int buttonIndex, sf::Vector2f mousePos) {
    if (buttonIndex < 0 || buttonIndex >= static_cast<int>(shopButtons.size())) {
        return false;
    }
    return shopButtons[buttonIndex].getGlobalBounds().contains(mousePos);
}

bool Shop::buyClickDamageUpgrade() {
    auto& rm = ResourceManager::getInstance();
    if (rm.spendCurrency(clickDamageUpgrade.cost)) {
        clickDamageUpgrade.level++;
        clickDamage = static_cast<int>(clickDamage * clickDamageUpgrade.multiplier);
        clickDamageUpgrade.cost = static_cast<int>(clickDamageUpgrade.baseCost * 
            std::pow(clickDamageUpgrade.multiplier, clickDamageUpgrade.level - 1));
        updateShopUI();
        return true;
    }
    return false;
}

bool Shop::buyUnit() {
    auto& rm = ResourceManager::getInstance();
    if (rm.spendCurrency(Unit::getBaseCost())) {
        return true;
    }
    return false;
}

bool Shop::buyTree() {
    auto& rm = ResourceManager::getInstance();
    if (rm.spendCurrency(Tree::getBaseCost())) {
        return true;
    }
    return false;
}

bool Shop::upgradeUnitDamage() {
    auto& rm = ResourceManager::getInstance();
    if (rm.spendCurrency(unitDamageUpgrade.cost)) {
        unitDamageUpgrade.level++;
        unitDamageUpgrade.cost = static_cast<int>(unitDamageUpgrade.baseCost * 
            std::pow(unitDamageUpgrade.multiplier, unitDamageUpgrade.level - 1));
        updateShopUI();
        return true;
    }
    return false;
}

bool Shop::upgradeUnitRange() {
    auto& rm = ResourceManager::getInstance();
    if (rm.spendCurrency(unitRangeUpgrade.cost)) {
        unitRangeUpgrade.level++;
        unitRangeUpgrade.cost = static_cast<int>(unitRangeUpgrade.baseCost * 
            std::pow(unitRangeUpgrade.multiplier, unitRangeUpgrade.level - 1));
        updateShopUI();
        return true;
    }
    return false;
}

float Shop::getUnitRangeForLevel(int level) const {
    float baseRange = 600.0f; // Half screen width
    float fullRange = 1200.0f; // Full screen width
    return baseRange + (level - 1) * (fullRange - baseRange) / 3.0f;
}

void Shop::draw(sf::RenderTarget& target) {
    if (!shopOpen) {
        // Always show currency even when shop is closed
        auto& rm = ResourceManager::getInstance();
        sf::Text currencyText(font);
        currencyText.setCharacterSize(20);
        currencyText.setFillColor(sf::Color::Yellow);
        std::stringstream ss;
        ss << "Gold: $" << rm.getCurrency() << " (Press S for Shop)";
        currencyText.setString(ss.str());
        currencyText.setPosition({20.0f, 15.0f});
        target.draw(currencyText);
        return;
    }
    
    target.draw(shopPanel);
    
    for (const auto& button : shopButtons) {
        target.draw(button);
    }
    
    for (const auto& text : shopTexts) {
        target.draw(text);
    }
    
    // Currency display
    auto& rm = ResourceManager::getInstance();
    sf::Text currencyText(font);
    currencyText.setCharacterSize(20);
    currencyText.setFillColor(sf::Color::Yellow);
    std::stringstream ss;
    ss << "Gold: $" << rm.getCurrency();
    currencyText.setString(ss.str());
    currencyText.setPosition({20.0f, 15.0f});
    target.draw(currencyText);
}

void Shop::reset() {
    clickDamage = 10;
    clickDamageUpgrade = {"Click Damage", 50, 1, 50, 1.5f};
    unitDamageUpgrade = {"Unit Damage", 100, 1, 100, 1.3f};
    unitRangeUpgrade = {"Unit Range", 75, 1, 75, 1.2f};
    updateShopUI();
}
