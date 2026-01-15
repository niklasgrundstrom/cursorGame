#pragma once
#include "Enemy.h"
#include <memory>

// Easy to extend - just inherit from Enemy and override if needed
class BasicEnemy : public Enemy {
public:
    BasicEnemy(float x, float y);
};

class FastEnemy : public Enemy {
public:
    FastEnemy(float x, float y);
};

class TankEnemy : public Enemy {
public:
    TankEnemy(float x, float y);
};

class FlyingEnemy : public Enemy {
public:
    FlyingEnemy(float x, float y);
};

// Factory function to create enemies based on wave difficulty
std::unique_ptr<Enemy> createEnemy(int wave, float x, float y);
