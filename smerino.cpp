//Author: Steven Merino

#include "fonts.h"

void show_steven_credits(int x, int y)
{
    Rect r;
    r.bot = y - 40;
    r.left = x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "Steven was def here putting in work");

}