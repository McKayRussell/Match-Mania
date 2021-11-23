// Author: Clemente Rodriguez 
#include "fonts.h"
#include "global.h"
#include <iostream>
#include <GL/glx.h>
using namespace std;

extern struct Card cards[5][8];

void show_clementes_credits(int x, int y)
{
    Rect r;
    r.bot = y + 20;
    r.left = x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "Clemente is caliente");
}


int scoreboard(int row, int col, int* xres, int* yres, int* score, 
                                                            Card cards[][8])
{    
    int count = 0;
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            if (cards[i][j].match == 1) {
                count++;
                if (cards[i][j].score == 0) { 
                    cards[i][j].score = 1; 
                    *score += 5;
                }
            }
        }
    }    
    
    Rect s;
    unsigned int c = 0x00ffffff;    
    s.bot = *yres-50;
    s.left = *xres-150;
    int h = 10;
      
    ggprint8b(&s, h, c, "Scoreboard");
    ggprint8b(&s, h, c, "----------------");
    ggprint8b(&s, h, c, " %i", *score);
  	
    return count; //matched cards count
}

void set_seasonal_theme(GLuint thanksgiving, int yres, int xres)
{
    glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, thanksgiving);
	glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
    glEnd();
    glPopMatrix();
		
}
