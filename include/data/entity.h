#include <linkedlist.h>
#include <util.h>
#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
public:
    virtual void createObject(int x, int y);
    virtual void move(int x, int y);
    virtual void speed(float newSpeed);
    virtual bool interactable(bool interactable);
    virtual bool hostile(bool hostile);

    bool hostile() { return this->isHostile; }
    bool interactable() { return this->isInteractable; }
    float speed() { return this->movementSpeed; }
    int x() { return this->xPos; }
    int y() { return this->yPos; }

private:
    int xPos;
    int yPos;
    float movementSpeed;
    bool isHostile;
    bool isInteractable;
    LinkedList<Pixel> pixels;
};

#endif
