#include <game/entities.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdio.h>

extern "C" char *__brkval;
extern "C" char __heap_start;

int freeMemory()
{
    char stack;
    char *heap;

    if (__brkval == 0)
    {
        heap = &__heap_start;
    }
    else
    {
        heap = __brkval;
    }

    return &stack - heap;
}
void renderTick()
{
    LinkedNode<Entity *> *dummy = entities.head;
    while (dummy != nullptr)
    {
        Entity *entity = dummy->dataByte;
        if (entity->type() == Player)
        {
            Character *character = static_cast<Character *>(entity);
            if (character->previousLocs()->size() > 0)
            {
                LinkedNode<Tuple<int, int>> *other = character->previousLocs()->head;
                while (other != nullptr)
                {
                    Tuple<int, int> prev = other->dataByte;
                    ILI9341_fillRect(prev.left, prev.right, 17, 17, 0x11b4);
                    character->previousLocs()->pop();
                    other = other->next;
                }
            }
        }
        ILI9341_drawBitmap(entity->x(), entity->y(), 16, 16, entity->sprite()->bitmap());
        dummy = dummy->next;
    }
    char text[32];
    sprintf(text, "Free: %d", freeMemory());
    ILI9341_drawString(0, 200, text, 0x00, 2);
}