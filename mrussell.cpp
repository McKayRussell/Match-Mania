//McKay Russell
//
//

#include "fonts.h"

void show_mckays_credits(int x, int y)
{
    Rect r;
    r.bot = y - 20;
    r.left = x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "McKay did something");
}
