#ifndef ENTITY_H
#define ENTITY_H

#include <linkedlist.h>
#include <util.h>
#include <avr/pgmspace.h>
#include "sprite.h"
#include "objects.h"

enum EntityType : char
{
    Enemy,
    Player,
    Projectile,
    NPC
};

class Entity : public GameObjectImpl
{
public:
    virtual void tick() {}
    virtual EntityType type();

    virtual void onHit() {}

    bool entity() override
    {
        return true;
    }

    bool hasSprite() override
    {
        return true;
    }

protected:
    Entity(uint16_t x, uint16_t y, uint8_t id, char *sprite) : GameObjectImpl(x, y, id, sprite) {}
};

#endif
