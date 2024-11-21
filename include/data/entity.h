#include <linkedlist.h>
#include <util.h>
#ifndef ENTITY_H
#define ENTITY_H

class Entity
{
public:
    void createObject(int x, int y);
    void move(int x, int y);
    void speed(float newSpeed);
    bool interactable(bool interactable);
    bool hostile(bool hostile);

    bool hostile();
    bool interactable();
    float speed();
    int x();
    int y();

private:
    int x;
    int y;
    float speed;
    bool hostile;
    bool interactable;
    LinkedList<Pixel> pixels;
};

#endif
