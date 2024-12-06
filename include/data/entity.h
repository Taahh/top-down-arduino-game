#include <linkedlist.h>
#include <util.h>
#include <avr/pgmspace.h>
#include "sprite.h"
#ifndef ENTITY_H
#define ENTITY_H

enum EntityType {
    Enemy,
    Player,
    NPC
};

class Entity
{
public:
    virtual void tick() {}

    virtual void speed(float newSpeed)
    {
        this->movementSpeed = newSpeed;
    }
    virtual bool interactable(bool interactable)
    {
        this->isInteractable = interactable;
    }
    bool hostile(bool hostile)
    {
        this->isHostile = hostile;
    }

    virtual EntityType type();

    bool hostile() { return this->isHostile; }
    bool interactable() { return this->isInteractable; }
    float speed() { return this->movementSpeed; }
    int x() { return this->xPos; }
    int y() { return this->yPos; }
    Sprite* sprite() { return this->entitySprite; }

protected:
    int xPos;
    int yPos;
    float movementSpeed;
    bool isHostile;
    bool isInteractable;
    Sprite* entitySprite;
    Entity(int x, int y) : xPos(x), yPos(y) {}
};

#endif
