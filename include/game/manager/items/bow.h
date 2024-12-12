#ifndef ARROW_H
#define ARROW_H
class Arrow : public Entity, public Movable
{
public:
    Arrow(int x, int y, uint8_t id, uint8_t direction, Game *game) : Entity(x, y, id, "arrowRight"), Movable()
    {
        facing = direction;
        this->game = game;
        switch (direction)
        {
        case 0:
            this->objectSprite = getSprite("arrowUp");
            break;
        case 1:
            this->objectSprite = getSprite("arrowDown");
            break;
        case 3:
            this->objectSprite = getSprite("arrowLeft");
            break;
        default:
            this->objectSprite = getSprite("arrowRight");
            break;
        }
    }
    void tick()
    {
        Tuple<int, int> prev = Tuple<int, int>{this->xPos, this->yPos};
        previousLocations->push(prev);

        switch (facing)
        {
        case 0:
            yPos -= 5;
            break;
        case 1:
            yPos += 5;
            break;
        case 2:
            xPos += 5;
            break;
        case 3:
            xPos -= 5;
            break;
        }

        Entity *collided = nullptr;
        LinkedNode<Tuple<uint8_t, GameObject *>> *dummy = game->getGameObjects()->head;
        while (dummy != nullptr)
        {
            if (dummy->dataByte.left == this->identifier())
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
                    if (entity->x() == xPos && entity->y() == yPos)
                    {
                        collided = entity;
                    }
                    // far most
                    if ((xPos <= entity->x() + (sprite->width() * sprite->scale()) - 5 && xPos >= entity->x() - (sprite->width() * sprite->scale()) + 5) && (yPos <= entity->y() + (sprite->height() * sprite->scale()) && yPos >= entity->y() - (sprite->height() * sprite->scale())))
                    {
                        collided = entity;
                    }
                }
            }

            if (collided != nullptr)
            {
                collided->onHit();
                break;
            }
            dummy = dummy->next;
        }

        if (xPos >= width - 5 || yPos > (height - height / 3) || yPos <= 0 || xPos <= 0 || collided != nullptr)
        {
            markForDeletion();
        }
    }

    EntityType type() override
    {
        return Projectile;
    }

private:
    uint8_t facing;
    Game *game;
};
#endif

#ifndef BOW_H
#define BOW_H

#include <game/manager/inventory.h>
#include "serialATmega.h"
#include <data/objects.h>
#include <game/game.h>

class Bow : public Item
{
public:
    Bow(Sprite *sprite = nullptr) : Item(sprite) {}

    void pressUse(Game *);
    void holdUse(Game *);
};
void Bow::holdUse(Game *game)
{
    if (ticksHeld() > 4)
    {
        itemSprite = getSprite("bowBack");
    }
    else if (ticksHeld() > 2)
    {
        itemSprite = getSprite("bowReady");
    }
}

void Bow::pressUse(Game *game)
{
    itemSprite = getSprite("bowLoaded");
    if (ticksHeld() > 4)
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
        Arrow *arrow = nullptr;
        switch (character->facingDirection())
        {
        case 0:
            arrow = new Arrow(character->x(), character->y() - 10, game->nextObjectId(), character->facingDirection(), game);
            game->registerObject(arrow);
            break;
        case 1:
            arrow = new Arrow(character->x(), character->y() - 10, game->nextObjectId(), character->facingDirection(), game);
            game->registerObject(arrow);
            break;
        case 2:
            arrow = new Arrow(character->x() + 10, character->y(), game->nextObjectId(), character->facingDirection(), game);
            game->registerObject(arrow);
            break;
        case 3:
            arrow = new Arrow(character->x() - 10, character->y(), game->nextObjectId(), character->facingDirection(), game);
            game->registerObject(arrow);
            break;
        }
    }
}

#endif