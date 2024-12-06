#include <game/entities.h>

void gameTick()
{
    LinkedNode<Entity *> *dummy = entities.head;
    while (dummy != nullptr)
    {
        Entity *entity = dummy->dataByte;
        entity->tick();
        dummy = dummy->next;
    }
}