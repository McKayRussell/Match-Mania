#include "fonts.h"

void show_dat_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left =x;
    r.center = 1;
    gprint8b(&r, 16, 0x00ff0000, "Dat did some programming"); 

}

