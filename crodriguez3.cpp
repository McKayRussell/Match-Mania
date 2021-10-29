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

void set_seasonal_theme(image img)
{
    glTexImage2D(GL_TEXTURE_2D, 0, 3, img[10].width, img[10].height,
									0, GL_RGB, GL_UNSIGNED_BYTE, img[10].data);
}
