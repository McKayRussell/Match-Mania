// Author: Clemente Rodriguez 
#include "fonts.h"

void show_clementes_credits(int x, int y)
{
    Rect r;
    r.bot = y + 20;
    r.left = x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "Clemente is caliente");
}
