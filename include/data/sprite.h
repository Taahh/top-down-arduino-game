#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "linkedlist.h"
#include "util.h"
using namespace std;

class Sprite
{
public:
    Sprite(char *name, uint16_t *bitmap, int16_t width = 16, int16_t height = 16, int8_t scale = 1) : spriteName(name), spriteBitmap(bitmap), spriteWidth(width), spriteHeight(height), spriteScale(scale) {}
    uint16_t *bitmap() { return spriteBitmap; }
    char *name() { return spriteName; }
    int16_t width() { return spriteWidth; }
    int16_t height() { return spriteHeight; }
    int8_t scale() { return spriteScale; }

    void scale(uint16_t newScale)
    {
        this->spriteScale = newScale;
    }

protected:
    uint16_t *spriteBitmap;
    char *spriteName;
    int16_t spriteWidth;
    int16_t spriteHeight;
    int8_t spriteScale = 1;
};

LinkedList<Tuple<char *, Sprite *>> sprites;

void registerSprite(Sprite *sprite)
{
    Tuple<char *, Sprite *> tuple = {
        sprite->name(),
        sprite};
    sprites.push(tuple);
}

Sprite *getSprite(char *name)
{
    LinkedNode<Tuple<char *, Sprite *>> *dummy = sprites.head;
    while (dummy != nullptr)
    {
        if (dummy->dataByte.left == name)
        {
            return dummy->dataByte.right;
        }
        dummy = dummy->next;
    }
    return nullptr;
}

#endif