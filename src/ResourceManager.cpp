#include "ResourceManager.h"

bool ResourceManager::spendCurrency(int amount) {
    if (currency >= amount) {
        currency -= amount;
        return true;
    }
    return false;
}
