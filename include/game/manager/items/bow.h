#ifndef BOW_H
#define BOW_H

#include <game/manager/inventory.h>
#include "serialATmega.h"

class Bow : public Item {
    public:
    Bow(Sprite *sprite = nullptr) : Item(sprite) {}

    void use();
};

void Bow::use() {
    serial_println("bow");
}

#endif