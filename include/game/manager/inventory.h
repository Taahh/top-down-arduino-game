#ifndef INVENTORY_H
#define INVENTORY_H

#include <linkedlist.h>
#include <util.h>
#include <data/sprite.h>

class Item
{
public:
    Item(Sprite *sprite = nullptr) : itemSprite(sprite) {}
    Sprite *sprite() { return itemSprite; }

private:
    Sprite *itemSprite;
};

class Inventory
{
public:
    Inventory()
    {
        this->itemList = new LinkedList<Item *>();
        this->selectedIndex = 0;
    }

    LinkedList<Item *> *items() { return itemList; }

private:
    LinkedList<Item *> *itemList;
    uint8_t selectedIndex;
};

#endif