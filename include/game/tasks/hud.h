#include <game/game.h>
#include "constants.h"

void hudTick(Game *game)
{
    ILI9341_drawBitmapScaled((240 / 2) - 48, 272, getSprite("inventory"));
    LinkedNode<Item *> *dummy = game->inventory()->items()->head;
    int16_t x = (240 / 2) - 39;
    int16_t y = 280;
    uint8_t i = 0;
    while (dummy != nullptr)
    {
        // serial_println(dummy->dataByte->sprite()->name());
        ILI9341_drawBitmapScaled(x, y, dummy->dataByte->sprite());
        if (i == game->inventory()->selected()) {
            ILI9341_fillRect(x, y - 15, 32, 3, 0xbdc5);
        } else {
            ILI9341_fillRect(x, y - 15, 32, 3, defaultColor);
        }

        x += 42;
        i++;
        dummy = dummy->next;
    }
}