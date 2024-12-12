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
enum GameStates
{
  Init,
  Tick
};
int LcdTick(int state)
{
  switch (state)
  {
  case Init:
    state = Tick;
    break;
  default:
    state = Tick;
    break;
  }
  switch (state)
  {
  case Tick:
    renderTick(game);
    break;
  }
  return state;
}

int GameTick(int state)
{
  switch (state)
  {
  case Init:
    state = Tick;
    break;
  default:
    state = Tick;
    break;
  }
  switch (state)
  {
  case Tick:
    gameTick(game);
    break;
  }
  return state;
}

int HudTick(int state)
{
  switch (state)
  {
  case Init:
    state = Tick;
    break;
  default:
    state = Tick;
    break;
  }
  switch (state)
  {
  case Tick:
    hudTick(game);
    break;
  }
  return state;
}

enum InputStates
{
  InputInit,
  ItemSwitchPress,
  ItemSwitchRelease,
  AttackPress,
  AttackRelease,
  ResetPress,
  ResetRelease
};

int InputTick(int state)
{
  bool itemSwitch = GetBit(PINC, 5);
  bool attack = GetBit(PINC, 4);
  // bool reset = GetBit(PINC, 2);
  // serial_println(reset);
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

  Item *item = game->inventory()->items()->at(game->inventory()->selected());

  switch (state)
  {
  case ItemSwitchRelease:
    game->inventory()->selectNext();
    item = game->inventory()->items()->at(game->inventory()->selected());
    break;
  case AttackPress:
    item->incrementTicksHeld();
    item->holdUse(game);
    break;
  case AttackRelease:
    if (item != nullptr)
    {
      item->pressUse(game);
      item->resetTicksHeld();
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
  Sprite *bowReadySprite = new Sprite("bowReady", bowReady, 16, 16, 2);
  Sprite *bowBackSprite = new Sprite("bowBack", bowBack, 16, 16, 2);
  Sprite *arrowRightSprite = new Sprite("arrowRight", arrowRight, 16, 16, 2);
  Sprite *arrowLeftSprite = new Sprite("arrowLeft", arrowLeft, 16, 16, 2);
  Sprite *arrowDownSprite = new Sprite("arrowDown", arrowDown, 16, 16, 2);
  Sprite *arrowUpSprite = new Sprite("arrowUp", arrowUp, 16, 16, 2);

  game->inventory()->items()->push(new Sword(swordSprite));
  game->inventory()->items()->push(new Bow(bowLoadedSprite));

  registerSprite(goblinSprite);
  registerSprite(goblinHitSprite);
  registerSprite(characterSprite);
  registerSprite(characterFaceRightSprite);
  registerSprite(characterFaceLeftSprite);
  registerSprite(characterFaceBehindSprite);
  registerSprite(houseSprite);
  registerSprite(inventorySprite);
  registerSprite(swordSprite);
  registerSprite(bowLoadedSprite);
  registerSprite(bowReadySprite);
  registerSprite(bowBackSprite);
  registerSprite(arrowRightSprite);
  registerSprite(arrowLeftSprite);
  registerSprite(arrowDownSprite);
  registerSprite(arrowUpSprite);

  game->setup();

  ILI9341_init();

  ILI9341_setRotation(2);
  ILI9341_fillScreen(0x11b4);

  TimerSet(GCD_PERIOD);
  TimerOn();

  tasks[0].period = LCD_PERIOD;
  tasks[0].elapsedTime = LCD_PERIOD;
  tasks[0].state = Init;
  tasks[0].TickFct = &LcdTick;

  tasks[1].period = GAME_PERIOD;
  tasks[1].elapsedTime = GAME_PERIOD;
  tasks[1].state = Init;
  tasks[1].TickFct = &GameTick;

  tasks[2].period = HUD_PERIOD;
  tasks[2].elapsedTime = HUD_PERIOD;
  tasks[2].state = Init;
  tasks[2].TickFct = &HudTick;

  tasks[3].period = HUD_PERIOD;
  tasks[3].elapsedTime = HUD_PERIOD;
  tasks[3].state = InputInit;
  tasks[3].TickFct = &InputTick;
  while (1)
  {
  }
  return 0;
}