#ifndef GOBLIN_H
#define GOBLIN_H

#include "data/entity.h"
#include "data/sprite.h"
#include <game/game.h>
#include <data/sprites/character.h>

class Game;

extern Game *game;
class Goblin : public Entity
{
public:
    Goblin(int x, int y, uint8_t id) : Entity(x, y, id, "goblin")
    {
        // this->objectSprite = getSprite("goblin");
    }
    EntityType type();
    void tick()
    {
        // Character *character = game->getPl;
        // LinkedList<Tuple<int16_t, int16_t>> queue;
        // queue.push({xPos, yPos});
        // while (queue.size() > 0)
        // {
        //     Tuple<int16_t, int16_t> dummy;
        //     queue.peek(dummy);
        //     queue.pop();
        //     int dist = -1;
        //     for (int x = 0; x < 2; x++)
        //     {
        //         for (int y = 0; y < 2; y++)
        //         {
        //             int16_t newY = yPos + 1;
        //             int16_t newX = xPos + 1;
        //             if (dist == -1)
        //             {
        //                 dist = distance(character->x(), character->y(), newX, newY);
        //                 queue.push({newX, newY});
        //             }
        //             else
        //             {
        //                 int newDist = distance(character->x(), character->y(), newX, newY);
        //                 if (newDist > dist)
        //                 {
        //                     queue.pop();
        //                     queue.push({newX, newY});
        //                 }
        //             }
        //         }
        //     }
        // }
        if (health <= 0)
        {
            markForDeletion();
            return;
        }
        if (hit)
        {
            if (timer >= 2)
            {
                this->objectSprite = getSprite("goblin");
                timer = 0;
                hit = false;
            }
            else
            {
                this->objectSprite = getSprite("goblinHit");
                timer++;
            }
        }
    }
    void onHit()
    {
        this->objectSprite = getSprite("goblinHit");
        hit = true;
        health -= 5;
    }

private:
    bool hit = false;
    int8_t timer = 0;
    int8_t health = 20;
};

EntityType Goblin::type()
{
    return Enemy;
}

#endif