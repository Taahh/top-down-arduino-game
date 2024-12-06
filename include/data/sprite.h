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
    Sprite(char *name, uint16_t *bitmap) : spriteName(name), spriteBitmap(bitmap) {}
    uint16_t *bitmap() { return spriteBitmap; }
    char *name() { return spriteName; }

protected:
    uint16_t *spriteBitmap;
    char *spriteName;
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