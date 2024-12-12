#ifndef SWORD_H
#define SWORD_H

#include <game/manager/inventory.h>
#include <game/game.h>
#include "serialATmega.h"
#include <util.h>

class Sword : public Item
{
public:
    Sword(Sprite *sprite = nullptr) : Item(sprite) {}

    void pressUse(Game *);
};

void Sword::pressUse(Game *game)
{
    Character *character = nullptr;
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
                    character = (Character *)entity;
                }
            }
            dummy = dummy->next;
        }
    }
    if (character == nullptr)
        return;
    Entity *hit = nullptr;
    LinkedNode<Tuple<uint8_t, GameObject *>> *dummy = game->getGameObjects()->head;
    while (dummy != nullptr)
    {
        if (dummy->dataByte.left == character->identifier())
        {
            dummy = dummy->next;
            continue;
        }
        GameObject *object = dummy->dataByte.right;
        if (object->entity())
        {
            Entity *entity = (Entity *)object;
            Sprite *sprite = entity->sprite();
            if (entity->type() == Projectile)
            {
                dummy = dummy->next;
                continue;
            }
            if (entity->type() == Enemy)
            {
                int dist = distance(character->x(), character->y(), entity->x(), entity->y());
                if (dist < 35)
                {
                    hit = entity;
                }
            }
        }

        if (hit != nullptr)
        {
            hit->onHit();
            break;
        }
        dummy = dummy->next;
    }
}

#endif