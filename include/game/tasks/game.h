#include <game/game.h>

void gameTick(Game *game)
{
    if (game->getScore() >= 5) {
        return;
    }
    {
        Character *character = game->getPlayer();
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
                        PORTD = SetBit(PORTD, 7, 1);
                        break;
                    }
                    else
                    {
                        PORTD = SetBit(PORTD, 7, 0);
                    }
                }
            }
            dummy = dummy->next;
        }
    }

    LinkedNode<Tuple<uint8_t, GameObject *>> *dummy = game->getGameObjects()->head;
    size_t i = 0;
    while (dummy != nullptr)
    {
        GameObject *object = dummy->dataByte.right;
        if (object->entity())
        {
            Entity *entity = static_cast<Entity *>(object);
            if (entity->type() == Player)
            {
                Character *character = static_cast<Character *>(entity);
                int16_t x, y;
                character->move(x, y);
                if (game->canMove(entity->identifier(), x, y))
                {
                    character->x(x);
                    character->y(y);
                }
            }
            entity->tick();
        }
        dummy = dummy->next;
        if (object->markedForDeletion())
        {
            if (object->entity()) {
                Entity* entity = (Entity*) object;
                if (entity->type() == Enemy) {
                    game->incScore();
                }
            }
            ILI9341_fillRect(object->x(), object->y(), 35, 35, defaultColor);
            game->getGameObjects()->pop(i);
        }
        i++;
    }
}