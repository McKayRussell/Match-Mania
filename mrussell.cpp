//McKay Russell <3 UwU
//

#include "fonts.h"
#include "global.h"

extern struct Card cards[5][8];

void show_mckays_credits(int x, int y)
{
    Rect r;
    r.bot = y - 20;
    r.left = x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "McKay did something");
}

//McKay Russell
//inputs: mouse x coordinate, mouse y coordinate
void flipCard(int mx, int my, int row, int col, float st_x, float st_y, int yres, 
                                                                        Card cards[][8])
{
	float x = st_x-40; 
    float y = ((st_y+85)/yres)+25;
  
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (mx > x+(100 * j) && mx < x+(100 * j)+85 && my > y+(130 * i)
					&& my < y+(130 * i)+120)
			{
                cards[i][j].flip = 1;  
			}
        }
	}
}
