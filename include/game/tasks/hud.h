#include <game/game.h>

void hudTick(Game *game)
{
    ILI9341_drawBitmapScaled((240 / 2) - 48, 272, getSprite("inventory"));
    LinkedNode<Item *> *dummy = game->inventory()->items()->head;
    int16_t x = (240 / 2) - 39;
    int16_t y = 280;
    while (dummy != nullptr)
    {
        // serial_println(dummy->dataByte->sprite()->name());
        ILI9341_drawBitmapScaled(x, y, dummy->dataByte->sprite());
        x += 42;
        dummy = dummy->next;
    }
}