#include <game/game.h>

void gameTick(Game *game)
{
    LinkedNode<Tuple<uint8_t, GameObject *>> *dummy = game->getGameObjects()->head;
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
    }
}