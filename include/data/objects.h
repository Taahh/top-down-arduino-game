#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <stdint.h>

class GameObject
{
public:
    GameObject(int16_t x, int16_t y, uint8_t id) : xPos(x), yPos(y), id(id) {
        markedDeletion = false;
    }
    uint8_t identifier() { return id; }
    virtual bool entity();
    virtual bool hasSprite();
    int16_t x() { return this->xPos; }
    int16_t y() { return this->yPos; }

    void x(uint16_t newX)
    {
        this->xPos = newX;
    }

    void y(uint16_t newY)
    {
        this->yPos = newY;
    }
    void markForDeletion() {
        markedDeletion = true;
    }
    
    bool markedForDeletion() {
        return markedDeletion;
    }

protected:
    int16_t xPos;
    int16_t yPos;

private:
    uint8_t id;
    bool markedDeletion;
};

#endif

#ifndef VISUAL_H
#define VISUAL_H

class Visualable
{
public:
    Sprite *sprite() { return this->objectSprite; }

protected:
    Sprite *objectSprite;
};

#endif

#ifndef MOVABLE_H
#define MOVABLE_H
class Movable
{
public:
    Movable()
    {
        this->previousLocations = new LinkedList<Tuple<int, int>>();
    }
    LinkedList<Tuple<int, int>> *previousLocs() { return this->previousLocations; }

protected:
    LinkedList<Tuple<int, int>> *previousLocations;
};
#endif

#ifndef GAMEOBJECT_IMPL_H
#define GAMEOBJECT_IMPL_H

class GameObjectImpl : public GameObject,
                       public Visualable
{
public:
    GameObjectImpl(uint16_t x, uint16_t y, uint8_t id, Sprite *sprite) : GameObject(x, y, id)
    {
        this->objectSprite = sprite;
    }

    GameObjectImpl(uint16_t x, uint16_t y, uint8_t id, char *sprite) : GameObject(x, y, id)
    {
        this->objectSprite = getSprite(sprite);
    }
    bool entity() override
    {
        return false;
    }

    bool hasSprite() override
    {
        return true;
    }
};

#endif