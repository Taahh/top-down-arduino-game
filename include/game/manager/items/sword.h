#ifndef SWORD_H
#define SWORD_H

#include <game/manager/inventory.h>
#include "serialATmega.h"

class Sword : public Item {
    public:
    Sword(Sprite *sprite = nullptr) : Item(sprite) {}

    void use();
};

void Sword::use() {
    serial_println("sword");
}

#endif