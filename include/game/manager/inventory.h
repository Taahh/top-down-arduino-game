#ifndef INVENTORY_H
#define INVENTORY_H

#include <linkedlist.h>
#include <util.h>
#include <data/sprite.h>

class Item
{
public:
    Sprite *sprite() { return itemSprite; }
    virtual void use();

protected:
    Item(Sprite *sprite = nullptr) : itemSprite(sprite) {}
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
    uint8_t selected() { return selectedIndex; }
    void selectNext() {
        if (selectedIndex + 1 >= itemList->size()) {
            selectedIndex = 0;
        } else {
            selectedIndex++;
        }
    }

private:
    LinkedList<Item *> *itemList;
    uint8_t selectedIndex;
};

#endif