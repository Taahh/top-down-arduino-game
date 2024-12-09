#ifndef CHARACTER_H
#define CHARACTER_H

#include "data/entity.h"
#include "data/sprite.h"
#include "helper.h"
#include "periph.h"
#include <ili9341.h>
#include <serialATmega.h>

class Character : public Entity
{
public:
    Character(int x, int y) : Entity(x, y)
    {
        this->entitySprite = getSprite("character");
        // this->movementSpeed = 1.5f;
        this->previousLocations = new LinkedList<Tuple<int, int>>();
    }
    void tick();

    EntityType type();
    LinkedList<Tuple<int, int>> *previousLocs() { return this->previousLocations; }

private:
    LinkedList<Tuple<int, int>> *previousLocations;
    uint8_t direction();
    void move();
};

EntityType Character::type()
{
    return Player;
}

uint8_t Character::direction()
{
    unsigned int vry = ADC_read(0);
    unsigned int vrx = ADC_read(1);
    bool left = vrx < 250;
    bool right = vrx > 700;
    bool down = vry < 250;
    bool up = vry > 700;
    uint8_t bitmask = 0x00;
    if (left)
    {
        bitmask = SetBit(bitmask, 3, 1);
    }
    if (right)
    {
        bitmask = SetBit(bitmask, 2, 1);
    }
    if (up)
    {
        bitmask = SetBit(bitmask, 0, 1);
    }
    if (down)
    {
        bitmask = SetBit(bitmask, 1, 1);
    }
    return bitmask & 0x0F;
}

void Character::move()
{
    uint8_t dir = direction();
    if (dir == 0)
    {
        return;
    }
    Tuple<int, int> prev = Tuple<int, int>{this->xPos, this->yPos};
    if (GetBit(dir, 1))
    {
        previousLocations->push(prev);
        yPos = yPos + 1.5f < height ? yPos + 1.5f : yPos;
    }
    if (GetBit(dir, 0))
    {
        previousLocations->push(prev);
        yPos = yPos - 1.5f >= 0 ? yPos - 1.5f : yPos;
    }
    if (GetBit(dir, 3))
    {
        previousLocations->push(prev);
        xPos = xPos - 1.5f >= 0 ? xPos - 1.5f : xPos;
    }
    if (GetBit(dir, 2))
    {
        previousLocations->push(prev);
        xPos = xPos + 1.5f < width ? xPos + 1.5f : xPos;
    }
}

void Character::tick()
{
    move();
}

#endif