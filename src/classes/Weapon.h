#ifndef WEAPON_H
#define WEAPON_H

#include <utility>
#include "Rectangle.h"

class Weapon {
   public:
    int damage;
    Rectangle damageArea;
    int knockback;

   Weapon(const Weapon& other) {
        this->damage = other.damage;
        this->damageArea.width = other.damageArea.width;
        this->damageArea.height= other.damageArea.height;
        this->knockback = other.knockback;
    }

    Weapon(int damage_, Rectangle damageArea_, int knockbackStrength_)
        : damage(damage_),
          damageArea(damageArea_),
          knockback(knockbackStrength_) {}

    Weapon& operator=(const Weapon& right) {
        Weapon tmp = right;
        std::swap(damage, tmp.damage);
        std::swap(damageArea.width, tmp.damageArea.width);
        std::swap(damageArea.height, tmp.damageArea.height);
        std::swap(knockback, tmp.knockback);
        return *this;
    }

    Weapon& operator=(Weapon&& right) {
        std::swap(damage, right.damage);
        std::swap(damageArea.width, right.damageArea.width);
        std::swap(damageArea.height, right.damageArea.height);
        std::swap(knockback, right.knockback);
        return *this;
    }
};

#endif