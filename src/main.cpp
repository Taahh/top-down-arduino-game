#include <helper.h>
#include <periph.h>
#include <serialATmega.h>
#include <timerISR.h>
#include <ili9341.h>
#include <data/sprite.h>
#include <bitmaps.h>
#include <stdio.h>
#include <util.h>
#include <game/game.h>
#include <game/tasks/renderer.h>
#include <game/tasks/game.h>
#include <game/tasks/hud.h>
#include <game/manager/items/sword.h>
#include <game/manager/items/bow.h>

using namespace std;

#define NUM_TASKS 4

typedef struct _task
{
  signed char state;         // Task's current state
  unsigned long period;      // Task period
  unsigned long elapsedTime; // Time elapsed since last task tick
  int (*TickFct)(int);       // Task tick function
} task;

const unsigned long LCD_PERIOD = 30;   // render tick
const unsigned long GAME_PERIOD = 30;  // game tick
const unsigned long HUD_PERIOD = 30;   // hud tick
const unsigned long INPUT_PERIOD = 30; // hud tick
const unsigned long GCD_PERIOD = 30;   // TODO:Set the GCD Period
task tasks[NUM_TASKS];

Game *game = new Game();

void TimerISR()
{
  for (unsigned int i = 0; i < NUM_TASKS; i++)
  { // Iterate through each task in the task array
    if (tasks[i].elapsedTime == tasks[i].period)
    {                                                    // Check if the task is ready to tick
      tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
      tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
    }
    tasks[i].elapsedTime += GCD_PERIOD; // Increment the elapsed time by GCD_PERIOD
  }
}

int LcdTick(int state)
{
  renderTick(game);
  return state;
}

int GameTick(int state)
{
  gameTick(game);
  return state;
}

int HudTick(int state)
{
  hudTick(game);
  return state;
}

enum InputStates
{
  InputInit,
  ItemSwitchPress,
  ItemSwitchRelease,
  AttackPress,
  AttackRelease
};

int InputTick(int state)
{
  bool itemSwitch = GetBit(PINC, 5);
  bool attack = GetBit(PINC, 4);
  switch (state)
  {
  case InputInit:
    if (itemSwitch)
    {
      state = ItemSwitchPress;
    }
    else if (attack)
    {
      state = AttackPress;
    }
    else
    {
      state = InputInit;
    }
    break;
  case ItemSwitchPress:
    state = itemSwitch ? ItemSwitchPress : ItemSwitchRelease;
    break;
  case ItemSwitchRelease:
    state = InputInit;
    break;
  case AttackPress:
    state = attack ? AttackPress : AttackRelease;
    break;
  case AttackRelease:
    state = InputInit;
    break;
  }

  switch (state)
  {
  case ItemSwitchRelease:
    game->inventory()->selectNext();
    break;
  case AttackRelease:
    Item* item = game->inventory()->items()->at(game->inventory()->selected());
    if (item != nullptr) {
      item->use();
    }
    break;
  }
  return state;
}

int main(void)
{
  serial_init(9600);
  // behaviorInit();
  ADC_init();
  SPI_init();
  DDRC = 0x00;
  DDRD = 0xF0;
  Sprite *goblinSprite = new Sprite("goblin", goblinMap, 16, 16, 2);
  Sprite *goblinHitSprite = new Sprite("goblinHit", goblinHitMap, 16, 16, 2);
  Sprite *characterSprite = new Sprite("character", characterMap, 16, 16, 2);
  Sprite *characterFaceRightSprite = new Sprite("charRight", characterFaceRight, 16, 16, 2);
  Sprite *characterFaceLeftSprite = new Sprite("charLeft", characterFaceLeft, 16, 16, 2);
  Sprite *characterFaceBehindSprite = new Sprite("charBehind", characterFaceBehind, 16, 16, 2);
  Sprite *houseSprite = new Sprite("house", house1Map, 32, 16, 3);
  Sprite *inventorySprite = new Sprite("inventory", inventory, 32, 16, 3);
  Sprite *swordSprite = new Sprite("sword", sword, 16, 16, 2);
  Sprite *bowLoadedSprite = new Sprite("bowLoaded", bowLoaded, 16, 16, 2);

  game->inventory()->items()->push(new Sword(swordSprite));
  game->inventory()->items()->push(new Bow(bowLoadedSprite));

  registerSprite(goblinSprite);
  registerSprite(characterSprite);
  registerSprite(characterFaceRightSprite);
  registerSprite(characterFaceLeftSprite);
  registerSprite(characterFaceBehindSprite);
  registerSprite(houseSprite);
  registerSprite(inventorySprite);
  registerSprite(swordSprite);
  registerSprite(bowLoadedSprite);

  game->setup();

  ILI9341_init();

  ILI9341_setRotation(2);
  ILI9341_fillScreen(0x11b4);

  // _delay_ms(1000);
  // ILI9341_fillRect(20, 20, 280, 200, 0xf800);
  // ILI9341_drawString(20, 20, "Hello there", 0xffff, 2);
  // _delay_ms(1000);
  // ILI9341_fillScreen(0x13a4);
  // ILI9341_fillRect(20, 20, width - 40, height - 40, 0x0000);

  TimerSet(GCD_PERIOD);
  TimerOn();

  tasks[0].period = LCD_PERIOD;
  tasks[0].elapsedTime = LCD_PERIOD;
  tasks[0].state = 0;
  tasks[0].TickFct = &LcdTick;

  tasks[1].period = GAME_PERIOD;
  tasks[1].elapsedTime = GAME_PERIOD;
  tasks[1].state = 0;
  tasks[1].TickFct = &GameTick;

  tasks[2].period = HUD_PERIOD;
  tasks[2].elapsedTime = HUD_PERIOD;
  tasks[2].state = 0;
  tasks[2].TickFct = &HudTick;

  tasks[3].period = HUD_PERIOD;
  tasks[3].elapsedTime = HUD_PERIOD;
  tasks[3].state = InputInit;
  tasks[3].TickFct = &InputTick;
  while (1)
  {
    // Main loop (do nothing)
  }
  return 0;
}