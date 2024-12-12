#ifndef INVENTORY_H
#define INVENTORY_H

#include <linkedlist.h>
#include <util.h>
#include <data/sprite.h>
#include <game/game.h>

class Game;

class Item
{
public:
    Sprite *sprite() { return itemSprite; }
    virtual void pressUse(Game*) {}
    virtual void holdUse(Game*) {}

    int16_t ticksHeld() { return holdTimer; }
    void incrementTicksHeld() {
        holdTimer++;
    }

    void resetTicksHeld() {
        holdTimer = 0;
    }

protected:
    Item(Sprite *sprite = nullptr) : itemSprite(sprite) {}
    Sprite *itemSprite;

    private:
    int16_t holdTimer;
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