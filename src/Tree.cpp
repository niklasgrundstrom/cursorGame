#include "Tree.h"
#include "ResourceManager.h"

Tree::Tree(float x, float y, float goldPerSecond)
    : position(x, y), goldPerSecond(goldPerSecond), goldGenerated(0.0f), timer(0.0f) {
    
    // Trunk (brown circle)
    trunk.setRadius(8.0f);
    trunk.setPosition({position.x - 8, position.y - 8});
    trunk.setFillColor(sf::Color(101, 67, 33)); // Brown
    trunk.setOutlineColor(sf::Color::Black);
    trunk.setOutlineThickness(1.0f);
    
    // Leaves (green circle on top)
    leaves.setRadius(15.0f);
    leaves.setPosition({position.x - 15, position.y - 30});
    leaves.setFillColor(sf::Color(34, 139, 34)); // Forest green
    leaves.setOutlineColor(sf::Color(0, 100, 0));
    leaves.setOutlineThickness(1.0f);
}

void Tree::update(float deltaTime) {
    timer += deltaTime;
    
    // Generate gold over time
    float goldThisFrame = goldPerSecond * deltaTime;
    goldGenerated += goldThisFrame;
    
    // Add to resource manager when we accumulate at least 1 gold
    if (goldGenerated >= 1.0f) {
        int goldToAdd = static_cast<int>(goldGenerated);
        ResourceManager::getInstance().addCurrency(goldToAdd);
        goldGenerated -= static_cast<float>(goldToAdd);
    }
}

void Tree::draw(sf::RenderTarget& target) {
    target.draw(trunk);
    target.draw(leaves);
}

sf::FloatRect Tree::getBounds() const {
    return leaves.getGlobalBounds(); // Use leaves bounds for clicking
}
