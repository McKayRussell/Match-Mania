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

void win_message(int row, int col, int* yres, int* xres, int* witch) {    
    int count = 0;
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            if (cards[i][j].match == 1) 
                count++;
        }
    }    
    Rect r; 
    r.bot = *yres-150;
    r.left = *xres/2-50;
    r.center = 0;
	        
    if (count == row*col) {
        *witch = 1;
        //g.showBigfoot =1;
        ggprint16(&r, 0, 0x00000000, "YOU WIN!!! <3");
    } 
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
