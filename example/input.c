#include "input.h"
#include "init.h"

unsigned int getjoystatus(int j)
{
  return snesc_controllers[j];
}

void clearjoy(int j)
{
  snesc_controllers[j] = 0;
}

