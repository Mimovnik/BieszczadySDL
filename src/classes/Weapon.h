#ifndef WEAPON_H
#define WEAPON_H

#include <utility>
#include "Rectangle.h"

class Weapon {
   public:
    int damage;
    Rectangle hitArea;
    int knockback;
    
    Weapon(){}

   Weapon(const Weapon& other) {
        this->damage = other.damage;
        this->hitArea.width = other.hitArea.width;
        this->hitArea.height= other.hitArea.height;
        this->knockback = other.knockback;
    }

    Weapon(int damage_, Rectangle damageArea_, int knockbackStrength_)
        : damage(damage_),
          hitArea(damageArea_),
          knockback(knockbackStrength_) {}

    Weapon& operator=(const Weapon& right) {
        Weapon tmp = right;
        std::swap(damage, tmp.damage);
        std::swap(hitArea.width, tmp.hitArea.width);
        std::swap(hitArea.height, tmp.hitArea.height);
        std::swap(knockback, tmp.knockback);
        return *this;
    }

    Weapon& operator=(Weapon&& right) {
        std::swap(damage, right.damage);
        std::swap(hitArea.width, right.hitArea.width);
        std::swap(hitArea.height, right.hitArea.height);
        std::swap(knockback, right.knockback);
        return *this;
    }
};

#endif