#ifndef GOBLIN_H
#define GOBLIN_H

#include "data/entity.h"
#include "data/sprite.h"

class Goblin : public Entity
{
public:
    Goblin(int x, int y) : Entity(x, y)
    {
        this->entitySprite = getSprite("goblin");
    }
    EntityType type();
};

EntityType Goblin::type() {
    return Enemy;
}

#endif