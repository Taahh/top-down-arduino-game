#include <game/game.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdio.h>

extern "C" char *__brkval;
extern "C" char __heap_start;

// int freeMemory()
// {
//     char stack;
//     char *heap;

//     if (__brkval == 0)
//     {
//         heap = &__heap_start;
//     }
//     else
//     {
//         heap = __brkval;
//     }

//     return &stack - heap;
// }
void renderTick(Game *game)
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
                if (character->previousLocs()->size() > 0)
                {
                    LinkedNode<Tuple<int, int>> *other = character->previousLocs()->head;
                    while (other != nullptr)
                    {
                        Tuple<int, int> prev = other->dataByte;
                        ILI9341_fillRect(prev.left, prev.right, 33, 33, 0x11b4);
                        character->previousLocs()->pop();
                        other = other->next;
                    }
                }
            }
            ILI9341_drawBitmapScaled(entity->x(), entity->y(), entity->sprite());
        }
        else if (object->hasSprite())
        {
            GameObjectImpl *impl = static_cast<GameObjectImpl *>(object);
            ILI9341_drawBitmapScaled(impl->x(), impl->y(), impl->sprite());
        }
        dummy = dummy->next;
    }
}