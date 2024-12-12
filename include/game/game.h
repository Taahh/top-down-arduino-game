#ifndef GAME_H
#define GAME_H

#include <linkedlist.h>
#include <util.h>
#include <data/entity.h>
#include <data/sprites/goblin.h>
#include <data/sprites/character.h>
#include "manager/inventory.h"

class Game
{
public:
    Game()
    {
        gameObjects = new LinkedList<Tuple<uint8_t, GameObject *>>();
        this->userInventory = new Inventory();
    }
    void setup();
    void registerObject(GameObject *);
    GameObjectImpl *spawnObject(int16_t x, int16_t y, Sprite *sprite);
    bool canMove(uint8_t id, int16_t x, int16_t y);
    Inventory *inventory() { return userInventory; }
    LinkedList<Tuple<uint8_t, GameObject *>> *getGameObjects() { return gameObjects; }
    int nextObjectId()
    {
        return currentObjId++;
    }
    void incScore() { score++; }
    uint8_t getScore() { return score; }

    Character *getPlayer();

private:
    LinkedList<Tuple<uint8_t, GameObject *>> *gameObjects;
    uint8_t currentObjId = 0;
    Inventory *userInventory;
    uint8_t score = 0;
};
void Game::registerObject(GameObject *object)
{
    gameObjects->push({object->identifier(), object});
}

GameObjectImpl *Game::spawnObject(int16_t x, int16_t y, Sprite *sprite)
{
    GameObjectImpl *obj = new GameObjectImpl(x, y, currentObjId++, sprite);
    gameObjects->push({obj->identifier(), obj});
}

void Game::setup()
{
    // serial_println("setup");
    registerObject(new Goblin(50, 100, currentObjId++));
    registerObject(new Goblin(50, 50, currentObjId++));
    registerObject(new Goblin(50, 80, currentObjId++));
    registerObject(new Goblin(100, 100, currentObjId++));
    registerObject(new Goblin(100, 150, currentObjId++));
    registerObject(new Character(150, 100, currentObjId++));
    registerObject(new GameObjectImpl(0, 0, currentObjId++, "house"));
    // registerObject(new GameObjectImpl(0, 150, currentObjId++, "arrow"));
}

Character *Game::getPlayer()
{
    LinkedNode<Tuple<uint8_t, GameObject *>> *dummy = game->getGameObjects()->head;
    while (dummy != nullptr)
    {
        GameObject *object = dummy->dataByte.right;
        if (object->entity())
        {
            Entity *entity = (Entity *)object;
            if (entity->type() == Player)
            {
                return (Character *)entity;
            }
        }
        dummy = dummy->next;
    }
    return nullptr;
}

bool Game::canMove(uint8_t id, int16_t x, int16_t y)
{
    LinkedNode<Tuple<uint8_t, GameObject *>> *dummy = gameObjects->head;
    while (dummy != nullptr)
    {
        if (dummy->dataByte.left == id)
        {
            dummy = dummy->next;
            continue;
        }
        GameObject *object = dummy->dataByte.right;
        if (object->entity())
        {
            Entity *entity = (Entity *)object;
            if (entity->type() == Projectile)
            {
                dummy = dummy->next;
                continue;
            }
        }
        if (object->hasSprite())
        {
            GameObjectImpl *objectImpl = static_cast<GameObjectImpl *>(object);
            Sprite *sprite = objectImpl->sprite();
            if (objectImpl->x() == x && objectImpl->y() == y)
            {
                return false;
            }
            // far most
            if ((x <= objectImpl->x() + (sprite->width() * sprite->scale()) - 5 && x >= objectImpl->x() - (sprite->width() * sprite->scale()) + 5) && (y <= objectImpl->y() + (sprite->height() * sprite->scale()) && y >= objectImpl->y() - (sprite->height() * sprite->scale())))
            {
                return false;
            }
        }
        dummy = dummy->next;
    }
    return true;
}

Character *getCharacter(Game *game)
{
    LinkedNode<Tuple<uint8_t, GameObject *>> *dummy = game->getGameObjects()->head;
    while (dummy != nullptr)
    {
        GameObject *object = dummy->dataByte.right;
        if (object->entity())
        {
            Entity *entity = (Entity *)object;
            if (entity->type() == Player)
            {
                return (Character *)entity;
            }
        }
        dummy = dummy->next;
    }
    return nullptr;
}

#endif