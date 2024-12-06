#include <helper.h>
#include <periph.h>
#include <serialATmega.h>
#include <timerISR.h>
#include <ili9341.h>
#include <data/sprite.h>
#include <bitmaps.h>
#include <stdio.h>
#include <util.h>
#include <game/events/renderer.h>
#include <game/events/game.h>

using namespace std;

#define NUM_TASKS 2

typedef struct _task
{
  signed char state;         // Task's current state
  unsigned long period;      // Task period
  unsigned long elapsedTime; // Time elapsed since last task tick
  int (*TickFct)(int);       // Task tick function
} task;

const unsigned long LCD_PERIOD = 30; // render tick
const unsigned long GAME_PERIOD = 30; // tick
const unsigned long GCD_PERIOD = 30; // TODO:Set the GCD Period
task tasks[NUM_TASKS];

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
  // behaviorUpdate();
  
  renderTick();
  // ILI9341_drawBitmap(200, 200, 16, 16, enemy_bitmap);
  // ILI9341_drawBitmap(150, 150, 16, 16, enemy_bitmap);
  // ILI9341_drawBitmap(150, 175, 16, 16, enemy_bitmap);
  // ILI9341_drawBitmap(100, 100, 16, 16, enemy_bitmap);
  // ILI9341_drawBitmap(100, 125, 16, 16, enemy_bitmap);
  // ILI9341_drawBitmap(x, y, 16, 16, bitmap);

  // ILI9341_fillRect(x, y, 10, 10, 0xffff);
  // ILI9341_drawString(20, 20, "Hello there", 0xffff, 0x0000, 2);
  return state;
}

int GameTick(int state)
{
  gameTick();
  return state;
}

int main(void)
{
  serial_init(9600);
  // behaviorInit();
  ADC_init();
  SPI_init();

  Sprite* goblinSprite = new Sprite("goblin", goblinMap);
  Sprite* characterSprite = new Sprite("character", characterMap);
  registerSprite(goblinSprite);
  registerSprite(characterSprite);

  entitySetup();

  ILI9341_init();

  ILI9341_setRotation(3);
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
  while (1)
  {
    // Main loop (do nothing)
  }
  return 0;
}