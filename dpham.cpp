//Dat Pham
//
//Note: I have another function in rainforest.cpp called pickCard(). It
//needs to be there to work and is not possible to bring it over.
//
#include <GL/glx.h>
#include "fonts.h"
#include "global.h"
#include <iostream>
using namespace std;

extern struct Card cards[5][8];
extern void pickCard(int,int,int);

//test to see how pass/use structs with extern
void test(int row, int col, Card cards[][8])
{
    for (int i=0; i<row; i++) {
	for (int j=0; j<col; j++) {	
            cards[i][j].flip = 5;
            //cout << cards[i][j].id << endl;
        }
    }
}

void show_dat_credits(int x, int y)
{
    Rect r;
    r.bot = y;
    r.left =x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "Dat fat cat sat at a mat"); 
}

//make struct rectangles for texture maps
//inputs: x,y   - the starting cordinates of the layout
void makeCards(int row, int col, int x, int y) {
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
            cards[i][j].xbox = x + (j*100.0f);
            cards[i][j].ybox = y - (i*130.0f);
        }
    } 
} 

//draw texture maps
void drawCard(int row,int col, GLuint texture) 
{
    glPushMatrix();
    glTranslatef(cards[row][col].xbox, cards[row][col].ybox, 0.0f);
			
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);    
        glTexCoord2f(0.0f, 1.0f); 
        glVertex2f(-cards[row][col].wbox, -cards[row][col].hbox);
        glTexCoord2f(0.0f, 0.0f); 
        glVertex2f(-cards[row][col].wbox,  cards[row][col].hbox); 
        glTexCoord2f(1.0f, 0.0f); 
        glVertex2f( cards[row][col].wbox,  cards[row][col].hbox); 
        glTexCoord2f(1.0f, 1.0f); 
        glVertex2f( cards[row][col].wbox, -cards[row][col].hbox);      
    glEnd();
    glPopMatrix();
}

void drawBack(int row, int col, GLuint texture, Card cards[][8])
{
	for (int i=0; i<row; i++) {
		for (int j=0; j<col; j++) {	
            if (cards[i][j].flip == 0) {      
                drawCard(i,j,texture);
            }
        }
	}        
}

//draw randomnized cards front
//input: arr[]  - the array that used random_shuffle() 
void randomHelper(int row, int col, int arr[]) 
{           
    int k = 0;
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
	        cards[i][j].id = arr[k];
            if (cards[i][j].match == 0) {      
                pickCard(i,j,arr[k]);
            }
            k++;
        }
    }    
}

void matchPairs(int row, int col, Card cards[][8])
{
    int prev_i, prev_j, prev_id; //remember previous card
    
    int compare = 0;
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
			//usleep(12000);
            if (cards[i][j].flip == 1 && cards[i][j].match == 0) { 
                compare++;
                
                //saving prev card info
                if (compare == 1) {
                    prev_i = i;
                    prev_j = j;
                    prev_id = cards[i][j].id;
                } 
                
                //does the 2 cards match?
                if (compare == 2) {
                    if (prev_id == cards[i][j].id) {
                        cards[i][j].match = 1;
                        cards[prev_i][prev_j].match = 1;
                        compare = 0;
                        break;
                    }
                    
                    cards[i][j].flip = 0;
                    cards[prev_i][prev_j].flip = 0; 
                    compare = 0; 
                    break; 
                } 
            }
        }
    }
}

void matchTriplets(int row, int col, Card cards[][8])
{   
    //remember previous cards
    int prev_i, prev_j, prev_id; 
    int prev_i2, prev_j2, prev_id2; 
    
    int compare = 0;
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
			//usleep(12000);
            if (cards[i][j].flip == 1 && cards[i][j].match == 0) { 
                compare++;
                
                //saving prev cards info
                if (compare == 1) {
                    prev_i = i;
                    prev_j = j;
                    prev_id = cards[i][j].id;
                } 

                if (compare == 2) {
                    prev_i2 = i;
                    prev_j2 = j;
                    prev_id2 = cards[i][j].id;
                } 
                
                //does the 3 cards match?
                if (compare == 3) {
                    if (prev_id == cards[i][j].id && prev_id2 == cards[i][j].id) {
                        cards[i][j].match = 1;
                        cards[prev_i][prev_j].match = 1;
                        cards[prev_i2][prev_j2].match = 1;
                        compare = 0;
                        break;
                    }
                    
                    cards[i][j].flip = 0;
                    cards[prev_i][prev_j].flip = 0; 
                    cards[prev_i2][prev_j2].flip = 0; 
                    compare = 0; 
                    break; 
                } 
            }
        }
    }
}

void matchQuadruplets(int row, int col, Card cards[][8])
{
    //remeber previous cards
    int prev_i, prev_j, prev_id; 
    int prev_i2, prev_j2, prev_id2; 
    int prev_i3, prev_j3, prev_id3; 
   
    int compare = 0;	
    for (int i=0; i<row; i++) {
        for (int j=0; j<col; j++) {
			//usleep(12000);
            if (cards[i][j].flip == 1 && cards[i][j].match == 0) { 
                compare++;
                
                //saving prev cards info
                if (compare == 1) {
                    prev_i = i;
                    prev_j = j;
                    prev_id = cards[i][j].id;
                } 
                if (compare == 2) {
                    prev_i2 = i;
                    prev_j2 = j;
                    prev_id2 = cards[i][j].id;
                }
                if (compare == 3) {
                    prev_i3 = i;
                    prev_j3 = j;
                    prev_id3 = cards[i][j].id;
                }

                //does the 4 cards match?
                if (compare == 4) {
                    if (prev_id == cards[i][j].id && 
                        prev_id2 == cards[i][j].id &&
                        prev_id3 == cards[i][j].id ) {
                        cards[i][j].match = 1;
                        cards[prev_i][prev_j].match = 1;
                        cards[prev_i2][prev_j2].match = 1;
                        cards[prev_i3][prev_j3].match = 1; 
                        compare = 0;
                        break;
                    }
                    
                    cards[i][j].flip = 0;
                    cards[prev_i][prev_j].flip = 0; 
                    cards[prev_i2][prev_j2].flip = 0; 
                    cards[prev_i3][prev_j3].flip = 0;  
                    compare = 0; 
                    break; 
                } 
            }
        }
    }
}

