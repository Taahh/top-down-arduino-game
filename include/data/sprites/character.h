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
    Character(int x, int y, uint8_t id) : Entity(x, y, id, "character")
    {
        // this->movementSpeed = 3f;
        this->previousLocations = new LinkedList<Tuple<int, int>>();
    }
    void tick();

    EntityType type();
    void move(int16_t &x, int16_t &y);
    LinkedList<Tuple<int, int>> *previousLocs() { return this->previousLocations; }

private:
    LinkedList<Tuple<int, int>> *previousLocations;
    uint8_t direction();
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

void Character::move(int16_t &x, int16_t &y)
{
    uint8_t dir = direction();
    x = xPos;
    y = yPos;
    if (dir == 0)
    {
        return;
    }
    Tuple<int, int> prev = Tuple<int, int>{this->xPos, this->yPos};
    // allow diagonal by removing else, but for the sake of the different sprite states no.
    if (GetBit(dir, 1))
    {
        previousLocations->push(prev);
        y = y + (objectSprite->height() * objectSprite->scale()) + 3 < height ? y + 3 : y;
        this->objectSprite = getSprite("character");
    }
    if (GetBit(dir, 0))
    {
        previousLocations->push(prev);
        y = y - (objectSprite->height() / 4 * objectSprite->scale()) - 3 >= 0 ? y - 3 : y;
        this->objectSprite = getSprite("charBehind");
    }
    if (GetBit(dir, 3))
    {
        previousLocations->push(prev);
        x = x - (objectSprite->width() / 4 * objectSprite->scale()) - 3 >= 0 ? x - 3 : x;
        this->objectSprite = getSprite("charLeft");
    }
    if (GetBit(dir, 2))
    {
        previousLocations->push(prev);
        x = x + (objectSprite->width() * objectSprite->scale()) + 3 < width ? x + 3 : x;
        this->objectSprite = getSprite("charRight");
    }
}

void Character::tick()
{
    // this->move();
}

#endif