#ifndef SPRITE_H
#define SPRITE_H

#include <stdint.h>

class Sprite
{
public:
    Sprite(char *name) : name(name) {}
    virtual void loadSprite();

private:
    uint16_t spriteBitmap[32];
    char *name;
};

#endif