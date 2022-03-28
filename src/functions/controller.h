#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <SDL.h>

#include "../classes/Alive.h"
#include "../classes/GameObject.h"
#include "../classes/QuadTree.h"

Animation* whichAnimation(Alive* entity);

void animationControl(Alive* entity, double realTime);

bool control(Alive* player, double realTime, Alive* creature, GameObject block,
             QuadTree* terrain);

#endif
