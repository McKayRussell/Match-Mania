//Dat Pham
#include <GL/glx.h>
#include "fonts.h"

void show_dat_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left =x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "Dat fat cat sat at a mat"); 

}

void drawBackground(int x, int y, GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
	    glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	    glTexCoord2f(0.0f, 0.0f); glVertex2i(0, y);
	    glTexCoord2f(1.0f, 0.0f); glVertex2i(x, y);
	    glTexCoord2f(1.0f, 1.0f); glVertex2i(x, 0);
    glEnd(); 
    glPopMatrix();
}

/*
//make struct rectangles for texture maps
void makeCards(int row, int col, int x, int y, Card) {
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            //g.cards[i][j].xbox = x + (j*100.0f);
            //g.cards[i][j].ybox = y - (i*130.0f);
        }
    } 
}

void dat_note(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left =x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "I put my name next to my functions in the comments."); 
}
*/


