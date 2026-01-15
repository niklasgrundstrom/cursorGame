#pragma once

class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    int getCurrency() const { return currency; }
    void addCurrency(int amount) { currency += amount; }
    bool spendCurrency(int amount);
    
    void reset() { currency = 0; }

private:
    ResourceManager() : currency(0) {}
    int currency;
};
