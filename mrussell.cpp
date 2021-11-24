//McKay Russell
//

#include "fonts.h"
#include "global.h"

extern struct Card cards[5][8];

void show_mckays_credits(int x, int y)
{
    Rect r;
    r.left = x;
    r.center = 1;
    r.bot = y + 40;
    ggprint16(&r, 16, 0x00000000, "Prof. Gordon supplied the framework.");
    r.bot = y - 20;
    ggprint16(&r, 16, 0x00000000, "McKay made the cards flip.");
}

//McKay Russell
//inputs: mouse x coordinate, mouse y coordinate
void flipCard(int mx, int my, int row, int col, float st_x, float st_y,
    int yres, Card cards[][8])
{
	float x = st_x - 40; 
    float y = ((st_y + 85) / yres) + 25;
    y = yres - 685;
  
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (mx > x + (100 * j) && mx < x + (100 * j) + 85 &&
                my > y + (130 * i) && my < y + (130 * i) + 120)
			{
                cards[i][j].flip = 1;  
			}
        }
	}
}

void resetCards(int row, int col, Card cards[][8])
{
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            cards[i][j].flip = 0; 
            cards[i][j].match = 0;
            cards[i][j].score = 0;
        }
    }
}
