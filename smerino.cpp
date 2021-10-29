/*Author: Steven Merino
* FILE: smerino.cpp
*/


#include "fonts.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glx.h>
#include <iostream>
#include "global.h"

extern struct Card cards[5][8];

typedef struct t_button {
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} Button;
Button button [3];
int buttonNum = 0;

void initialize_buttons() 
{
	//initialize buttons...
	buttonNum = 0;

	// Round 1 Button position
	button[buttonNum].r.width = 200;
	button[buttonNum].r.height = 100;
	button[buttonNum].r.left = 100;
	button[buttonNum].r.bot = 30;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Easy: Round 1");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 1.0f;
	button[buttonNum].color[1] = 1.0f;
	button[buttonNum].color[2] = 1.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Round 2 Button position
	button[buttonNum].r.width = 200;
	button[buttonNum].r.height = 100;
	button[buttonNum].r.left = 450;
	button[buttonNum].r.bot = 30;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Medium: Round 2");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 1.0f;
	button[buttonNum].color[1] = 0.5f;
	button[buttonNum].color[2] = 0.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Round 3 Button position
	button[buttonNum].r.width = 200;
	button[buttonNum].r.height = 100;
	button[buttonNum].r.left = 800;
	button[buttonNum].r.bot = 30;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Hard: Round 3");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 0.5f;
	button[buttonNum].color[1] = 0.3f;
	button[buttonNum].color[2] = 0.9f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;
}

void mouse_over_button(int action, int* menu, int* round1, int* round2
	, int* round3) 
{
	for (int i = 0; i < buttonNum; i++){
		if (action == 1) {
			if (button[i].over) {
				button[i].down = 1;
				button[i].click = 1;
				*menu ^= 1;
				// go to round
				if (i == 0){
					*round1 ^= 1;
				}
				if (i == 1){
					*round2 ^= 1;
				}
				if (i == 2){
					*round3 ^= 1;
				}
				for (int i=0; i<5; i++) {
					for (int j=0; j<8; j++) {
						cards[i][j].flip = 0;
						cards[i][j].match = 0;
					}
				}
			}
		}
	}
}

void hover_button (int x, int y)
{
	for (int i = 0; i < buttonNum; i++) {
		button[i].over = 0;
		button[i].down = 0;
		if (x >= button[i].r.left &&
			x <= button[i].r.right &&
			y >= button[i].r.bot &&
			y <= button[i].r.top) {
				button[i].over = 1;
				break;
		}
	}
}

void draw_buttons() 
{
	Rect r;
	for (int i=0; i<buttonNum; i++) {
		if (button[i].over) {
			glColor3f(1.0f, 0.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
				glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
				glVertex2i(button[i].r.left-2,  button[i].r.top+2);
				glVertex2i(button[i].r.right+2, button[i].r.top+2);
				glVertex2i(button[i].r.right+2, button[i].r.bot-2);
				glVertex2i(button[i].r.left-2,  button[i].r.bot-2);
			glEnd();
			glLineWidth(1);
		}
		if (button[i].down) {
			glColor3fv(button[i].dcolor);
		} else {
			glColor3fv(button[i].color);
		}
		glBegin(GL_QUADS);
			glVertex2i(button[i].r.left,  button[i].r.bot);
			glVertex2i(button[i].r.left,  button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.top);
			glVertex2i(button[i].r.right, button[i].r.bot);
		glEnd();
		r.left = button[i].r.centerx;
		r.bot  = button[i].r.centery-8;
		r.center = 1;
		if (button[i].down) {
			ggprint16(&r, 0, button[i].text_color, "Goodluck!");
		} else {
			ggprint16(&r, 0, button[i].text_color, button[i].text);
		}
	}
}

void show_steven_credits(int x, int y)
{
    Rect r;
    r.bot = y - 40;
    r.left = x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "Steven was def here putting in work");

}
