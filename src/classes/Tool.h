#ifndef TOOL_H
#define TOOL_H

#include "Weapon.h"
class Tool : public Weapon {
   public:
    int efficiency;
    
    Tool(){}

    Tool(const Tool& other) {
        this->damage = other.damage;
        this->hitArea.width = other.hitArea.width;
        this->hitArea.height = other.hitArea.height;
        this->knockback = other.knockback;
        this->efficiency = other.efficiency;
    }

    Tool(int efficiency, int damage, Rectangle damageArea, int knockbackStrength) {
        this->efficiency = efficiency;
        this->damage = damage;
        this->hitArea = damageArea;
        this->knockback = knockbackStrength;
    }

    Tool& operator=(const Tool& right) {
        Tool tmp = right;
        std::swap(efficiency, tmp.efficiency);
        std::swap(damage, tmp.damage);
        std::swap(hitArea.width, tmp.hitArea.width);
        std::swap(hitArea.height, tmp.hitArea.height);
        std::swap(knockback, tmp.knockback);
        return *this;
    }

    Tool& operator=(Tool&& right) {
        std::swap(efficiency, right.efficiency);
        std::swap(damage, right.damage);
        std::swap(hitArea.width, right.hitArea.width);
        std::swap(hitArea.height, right.hitArea.height);
        std::swap(knockback, right.knockback);
        return *this;
    }
};

#endif