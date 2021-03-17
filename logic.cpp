#include "framework.h"
#include "logic.h"
#include <time.h>

float randab(float a, float b)
{
    int x = (int)((b - a) * 100);
    int y = (int)(a * 100);
    return (float)(rand() % x + y) / 100;
};





