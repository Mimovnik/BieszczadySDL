#ifndef TOOL_H
#define TOOL_H

#include "Weapon.h"
class Tool : public Weapon {
   public:
    int efficiency;
    
    Tool(){}

    Tool(const Tool& other) {
        this->damage = other.damage;
        this->damageArea.width = other.damageArea.width;
        this->damageArea.height = other.damageArea.height;
        this->knockback = other.knockback;
        this->efficiency = other.efficiency;
    }

    Tool(int efficiency, int damage, Rectangle damageArea, int knockbackStrength) {
        this->efficiency = efficiency;
        this->damage = damage;
        this->damageArea = damageArea;
        this->knockback = knockbackStrength;
    }

    Tool& operator=(const Tool& right) {
        Tool tmp = right;
        std::swap(efficiency, tmp.efficiency);
        std::swap(damage, tmp.damage);
        std::swap(damageArea.width, tmp.damageArea.width);
        std::swap(damageArea.height, tmp.damageArea.height);
        std::swap(knockback, tmp.knockback);
        return *this;
    }

    Tool& operator=(Tool&& right) {
        std::swap(efficiency, right.efficiency);
        std::swap(damage, right.damage);
        std::swap(damageArea.width, right.damageArea.width);
        std::swap(damageArea.height, right.damageArea.height);
        std::swap(knockback, right.knockback);
        return *this;
    }
};

#endif