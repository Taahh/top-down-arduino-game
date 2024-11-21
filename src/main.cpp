#include "helper.h"
#include "periph.h"
#include "serialATmega.h"
#include "timerISR.h"
#include "ili9341.h"
#include <stdio.h>
#include <util.h>

using namespace std;

#define NUM_TASKS 2

typedef struct _task
{
  signed char state;         // Task's current state
  unsigned long period;      // Task period
  unsigned long elapsedTime; // Time elapsed since last task tick
  int (*TickFct)(int);       // Task tick function
} task;

const unsigned long CONTROLLER_PERIOD = 100;
const unsigned long LCD_PERIOD = 100;
const unsigned long GCD_PERIOD = 100; // TODO:Set the GCD Period
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
uint8_t getDirection()
{
  unsigned int vry = ADC_read(0);
  unsigned int vrx = ADC_read(1);
  bool left = vrx < 250;
  bool right = vrx > 700;
  bool down = vry < 250;
  bool up = vry > 700;
  uint8_t bitmask = 0x00;
  if (left)
  {
    bitmask = SetBit(bitmask, 3, 1);
  }
  if (right)
  {
    bitmask = SetBit(bitmask, 2, 1);
  }
  if (up)
  {
    bitmask = SetBit(bitmask, 0, 1);
  }
  if (down)
  {
    bitmask = SetBit(bitmask, 1, 1);
  }
  return bitmask & 0x0F;
}
int speed = 3;
int x = 20;
int y = 20;
int prevX = x;
int prevY = y;
LinkedList<Tuple<int, int>> previousLocations;
const uint16_t bitmap[32] = {0x3, 0xc0, 0xf, 0xf0, 0x1f, 0xf8, 0x1d, 0x38, 0x18, 0x18, 0x8, 0x10,
                             0x4, 0x20, 0x2, 0x40, 0x7, 0xe0, 0xf, 0xf0, 0xf, 0xf0, 0x17, 0xe8,
                             0x1b, 0xd8, 0x3, 0xc0, 0x3, 0xc0, 0x7, 0xe0};
int LcdTick(int state)
{
  if (previousLocations.size() > 0)
  {
    LinkedNode<Tuple<int, int>> *dummy = previousLocations.head;
    while (dummy != nullptr)
    {
      Tuple<int, int> prev = dummy->dataByte;
      ILI9341_fillRect(prev.left, prev.right, 11, 11, 0x13a4);
      previousLocations.pop();
      dummy = dummy->next;
    }
  }
  ILI9341_drawBitmap(x, y, 16, 16, bitmap);
  // ILI9341_fillRect(x, y, 10, 10, 0xffff);
  // ILI9341_drawString(20, 20, "Hello there", 0xffff, 0x0000, 2);
  return state;
}

int ControllerTick(int state)
{
  uint8_t dir = getDirection();
  serial_println(dir, 2);
  if (dir == 0)
  {
    return state;
  }
  Tuple<int, int> prev = Tuple<int, int>{x, y};
  if (GetBit(dir, 1))
  {
    previousLocations.push(prev);
    y = y + speed < height ? y + (1 * speed) : y;
  }
  if (GetBit(dir, 0))
  {
    previousLocations.push(prev);
    y = y - speed >= 0 ? y - (1 * speed) : y;
  }
  if (GetBit(dir, 3))
  {
    previousLocations.push(prev);
    x = x - speed >= 0 ? x - (1 * speed) : x;
  }
  if (GetBit(dir, 2))
  {
    previousLocations.push(prev);
    x = x + speed < width ? x + (1 * speed) : x;
  }
  return state;
}

int main(void)
{
  serial_init(9600);
  ADC_init();
  SPI_init();
  ILI9341_init();

  ILI9341_setRotation(3);
  ILI9341_fillScreen(0x13a4);
  // ILI9341_fillRect(20, 20, width - 40, height - 40, 0x0000);
  TimerSet(GCD_PERIOD);
  TimerOn();

  tasks[0].period = LCD_PERIOD;
  tasks[0].elapsedTime = LCD_PERIOD;
  tasks[0].state = 0;
  tasks[0].TickFct = &LcdTick;

  tasks[1].period = CONTROLLER_PERIOD;
  tasks[1].elapsedTime = CONTROLLER_PERIOD;
  tasks[1].state = 0;
  tasks[1].TickFct = &ControllerTick;
  while (1)
  {
    // Main loop (do nothing)
  }
  return 0;
}