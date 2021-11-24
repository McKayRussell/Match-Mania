/*Author: Steven Merino
* FILE: smerino.cpp
*/


#include "fonts.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <GL/glx.h>
#include <iostream>
#include "global.h"
#include <unistd.h>
using namespace std;

#define BUTTON_NUM 10

extern struct Card cards[5][8];
void resetCards(int, int, Card cards[][8]);

int clock_square;

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
Button button [BUTTON_NUM];
int buttonNum = 0;

class t_square {
public:
	Rect r;
	char time[32];
	int blink;
	float clock_color[3];
	float clock_dcolor[3];
	unsigned int clock_text_color;
} square;

void initialize_buttons() 
{
	//initialize buttons...
	// int left = 100;
	// // char r_number[32];
	// for (int i= 0; i < BUTTON_NUM; i++) {
	// 	button[i].r.width = 200;
	// 	button[i].r.height = 100;
	// 	button[i].r.left = left + 200;
	// 	button[i].r.bot = 30;
	// 	button[i].r.right =
	// 		button[i].r.left + button[i].r.width;
	// 	button[i].r.top = button[i].r.bot + button[i].r.height;
	// 	button[i].r.centerx =
	// 		(button[i].r.left + button[i].r.right) / 2;
	// 	button[i].r.centery =
	// 		(button[i].r.bot + button[i].r.top) / 2;
	// 	strcpy(button[i].text, "Easy: Round: ");
	// 	// sprintf(r_number, "%d", i);
	// 	// strcpy(button[i].text, r_number);
	// 	button[i].down = 0;
	// 	button[i].click = 0;
	// 	button[i].color[0] = 1.0f;
	// 	button[i].color[1] = 1.0f;
	// 	button[i].color[2] = 1.0f;
	// 	button[i].dcolor[0] = button[i].color[0] * 0.5f;
	// 	button[i].dcolor[1] = button[i].color[1] * 0.5f;
	// 	button[i].dcolor[2] = button[i].color[2] * 0.5f;
	// 	button[i].text_color = 0x00ffffff;
	// }

	//Round 1 Button position
	buttonNum = 0;
	int width = 200;
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 100;
	button[buttonNum].r.bot = 205;
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
	button[buttonNum].color[0] = 0.0f;
	button[buttonNum].color[1] = 0.0f;
	button[buttonNum].color[2] = 0.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.6f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.7f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Round 2 Button position
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 450;
	button[buttonNum].r.bot = 205;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Easy: Round 2");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 0.0f;
	button[buttonNum].color[1] = 0.0f;
	button[buttonNum].color[2] = 0.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Round 3 Button position
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 800;
	button[buttonNum].r.bot = 205;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Easy: Round 3");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 0.9f;
	button[buttonNum].color[1] = 0.58f;
	button[buttonNum].color[2] = 0.5f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Medium Round 1 Button
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 100;
	button[buttonNum].r.bot = 125;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Medium: Round 1");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 1.0f;
	button[buttonNum].color[1] = 0.3f;
	button[buttonNum].color[2] = 1.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Medium Round 2 Button
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 450;
	button[buttonNum].r.bot = 125;
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
	button[buttonNum].color[1] = 0.3f;
	button[buttonNum].color[2] = 1.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Medium Round 3 Button
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 800;
	button[buttonNum].r.bot = 125;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Medium: Round 3");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 1.0f;
	button[buttonNum].color[1] = 0.3f;
	button[buttonNum].color[2] = 1.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Hard Round 1 Button
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 100;
	button[buttonNum].r.bot = 45;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Hard: Round 1");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 1.0f;
	button[buttonNum].color[1] = 0.3f;
	button[buttonNum].color[2] = 1.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Hard Round 2 Button
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 450;
	button[buttonNum].r.bot = 45;
	button[buttonNum].r.right =
		button[buttonNum].r.left + button[buttonNum].r.width;
	button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	button[buttonNum].r.centerx =
		(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	button[buttonNum].r.centery =
		(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	strcpy(button[buttonNum].text, "Hard: Round 2");
	button[buttonNum].down = 0;
	button[buttonNum].click = 0;
	button[buttonNum].color[0] = 1.0f;
	button[buttonNum].color[1] = 0.3f;
	button[buttonNum].color[2] = 1.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

	// Hard Round 3 Button
	button[buttonNum].r.width = width;
	button[buttonNum].r.height = 75;
	button[buttonNum].r.left = 800;
	button[buttonNum].r.bot = 45;
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
	button[buttonNum].color[0] = 1.0f;
	button[buttonNum].color[1] = 0.3f;
	button[buttonNum].color[2] = 1.0f;
	button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	button[buttonNum].text_color = 0x00ffffff;
	buttonNum++;

/*********************************************************************
 * 
 * Exits Buttons
 * NOTE : Working on condensing this into for loops but this unable to
 * 			do so
 * 
 * *******************************************************************/
	// Easy Round 1 Exit Button
	// button[buttonNum].r.width = width;
	// button[buttonNum].r.height = 75;
	// button[buttonNum].r.left = 800;
	// button[buttonNum].r.bot = 25;
	// button[buttonNum].r.right =
	// 	button[buttonNum].r.left + button[buttonNum].r.width;
	// button[buttonNum].r.top = button[buttonNum].r.bot + button[buttonNum].r.height;
	// button[buttonNum].r.centerx =
	// 	(button[buttonNum].r.left + button[buttonNum].r.right) / 2;
	// button[buttonNum].r.centery =
	// 	(button[buttonNum].r.bot + button[buttonNum].r.top) / 2;
	// strcpy(button[buttonNum].text, "Back to Menu");
	// button[buttonNum].down = 0;
	// button[buttonNum].click = 0;
	// button[buttonNum].color[0] = 1.0f;
	// button[buttonNum].color[1] = 0.3f;
	// button[buttonNum].color[2] = 1.0f;
	// button[buttonNum].dcolor[0] = button[buttonNum].color[0] * 0.5f;
	// button[buttonNum].dcolor[1] = button[buttonNum].color[1] * 0.5f;
	// button[buttonNum].dcolor[2] = button[buttonNum].color[2] * 0.5f;
	// button[buttonNum].text_color = 0x00ffffff;
	// buttonNum++;
}

void initialize_clock() 
{
	square.r.width = 200;
	square.r.height = 75;
	square.r.left = 100;
	square.r.bot = 205;
	square.r.right =
		square.r.left + square.r.width;
	square.r.top = square.r.bot + square.r.height;
	square.r.centerx = (square.r.left + square.r.right) / 2;
	square.r.centery = (square.r.bot + square.r.top) / 2;
	strcpy(square.time, "Ready!");
	square.clock_color[0] = 0.0f;
	square.clock_color[1] = 0.0f;
	square.clock_color[2] = 0.0f;
	square.clock_dcolor[0] = square.clock_color[0] * 0.0f;
	square.clock_dcolor[1] = square.clock_color[1] * 0.0f;
	square.clock_dcolor[2] = square.clock_color[2] * 0.0f;
	square.clock_text_color = 0x00ffffff;
	square.blink = 0;
}

void mouse_over_button(int action, int* menu, int* round1, int* round2
	, int* round3, int* med1, int* med2, int* med3, int* hard1,
	int* hard2, int* hard3, int* rand, float* time)
{
	if (*menu == 1) {
		for (int i = 0; i < buttonNum; i++) {
			if (action == 1) {
				if (button[i].over) {
					button[i].down = 1;
					button[i].click = 1;
					*menu ^= 1;
					*rand = 1;
					// go to round
					if (i == 0){
						*round1 ^= 1;
                        *time = 60;
						resetCards(3, 4, cards);
						break;
					}
					if (i == 1) {
						*round2 ^= 1;
                        *time = 75;
					}
					if (i == 2) {
						*round3 ^= 1;
				        *time = 90;
						resetCards(5, 8, cards);
						break;
                    }
					if (i == 3) {
						*med1 ^= 1;
					    *time = 90;
						resetCards(4, 5, cards);
						break;
                    }
					if (i == 4) {
						*med2 ^= 1;
					    *time = 105;
						resetCards(4, 5, cards);
						break;
					
                    }
					if (i == 5) {
						*med3 ^= 1;
					    *time = 120;
						resetCards(4, 5, cards);
						break;
					
                    }
					if (i == 6) {
						*hard1 ^= 1;
					    *time = 120;
						resetCards(4, 5, cards);
						break;
					
                    }
					if (i == 7) {
						*hard2 ^= 1;
					    *time = 150;
						resetCards(4, 5, cards);
						break;
                    }
					if (i == 8) {
						*hard3 ^= 1;
                        *time = 180;
						resetCards(4, 5, cards);
						break;
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

// 	if (*round1 && action && button[9].over) { 
// 		button[9].down = 1;
// 		button[9].click = 1;
// 		*key_press = XK_1;
// 		cout << *key_press << " and " << *round1 << endl;
// 	}
}


void hover_button (int x, int y, int* menu)
{
	if (*menu == 1) {
		for (int i = 0; i < 9; i++) {
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

	// Exit button in rounds
	// if (*menu == 0) {
	// 	for (int i = 9; i < BUTTON_NUM; i++) {
	// 		button[i].over = 0;
	// 		button[i].down = 0;
	// 		if (x >= button[i].r.left &&
	// 			x <= button[i].r.right &&
	// 			y >= button[i].r.bot &&
	// 			y <= button[i].r.top) {
	// 				button[i].over = 1;
	// 				break;
	// 		}
	// 	}
	// }
}

void draw_start_screen(int yres, int xres, GLuint start)
{
	glPushMatrix();
	glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
	glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
	glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
	glBindTexture(GL_TEXTURE_2D, start);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, yres);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(xres, yres);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(xres, 0);
	glEnd();
	glPopMatrix();
}

void draw_menu_buttons() 
{
	Rect r;
	for (int i=0; i< buttonNum; i++) {
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
		r.bot  = button[i].r.centery - 8;
		r.center = 1;
		if (button[i].down) {
			ggprint16(&r, 0, button[i].text_color, "Goodluck!");
		} else {
			ggprint16(&r, 0, button[i].text_color, button[i].text);
		}
	}
}

/*********************************************************************
 * 
 * Trying to create a timer
 * NOTE : Tried to make this task multithreaded: 1 thread handles the clock
 * 	so the rest of the application doesnt wait wehn calling sleep(); 
 *  Did not work. Will come back to it. 
 * 
 * 
 * *******************************************************************/

// void draw_clock() 
// {
// 	Rect r;
// 	for (int i = 0; i < 10; i++) {
// 		if (i < 35) {
// 			square.blink = 1;
// 		}
// 		if (square.blink) {
// 			glColor3f(1.0f, 0.0f, 0.0f);
// 			//draw a highlight around button
// 			glLineWidth(2);
// 			glBegin(GL_LINE_LOOP);
// 				glVertex2i(square.r.left-2,  square.r.bot-2);
// 				glVertex2i(square.r.left-2,  square.r.top+2);
// 				glVertex2i(square.r.right+2, square.r.top+2);
// 				glVertex2i(square.r.right+2, square.r.bot-2);
// 				glVertex2i(square.r.left-2,  square.r.bot-2);
// 			glEnd();
// 			glLineWidth(1);
// 		}
// 		sleep(1);
// 		// ggprint16(&r, 0, square.clock_text_color, ("%d seconds remaining!", i));
// 	}

// 	glBegin(GL_QUADS);
// 		glVertex2i(square.r.left,  square.r.bot);
// 		glVertex2i(square.r.left,  square.r.top);
// 		glVertex2i(square.r.right, square.r.top);
// 		glVertex2i(square.r.right, square.r.bot);
// 	glEnd();
// 	r.left = square.r.centerx;
// 	r.bot  = square.r.centery - 8;
// 	r.center = 1;
// }

void draw_exit_button(int round)
{
	Rect r;
	round = round + 8;
	if (button[round].over) {
		glColor3f(1.0f, 0.0f, 0.0f);
		//draw a highlight around button
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
			glVertex2i(button[round].r.left-2,  button[round].r.bot-2);
			glVertex2i(button[round].r.left-2,  button[round].r.top+2);
			glVertex2i(button[round].r.right+2, button[round].r.top+2);
			glVertex2i(button[round].r.right+2, button[round].r.bot-2);
			glVertex2i(button[round].r.left-2,  button[round].r.bot-2);
		glEnd();
		glLineWidth(1);
	}
	if (button[round].down) {
		glColor3fv(button[round].dcolor);
	} else {
		glColor3fv(button[round].color);
	}
	glBegin(GL_QUADS);
		glVertex2i(button[round].r.left,  button[round].r.bot);
		glVertex2i(button[round].r.left,  button[round].r.top);
		glVertex2i(button[round].r.right, button[round].r.top);
		glVertex2i(button[round].r.right, button[round].r.bot);
	glEnd();
	r.left = button[round].r.centerx;
	r.bot  = button[round].r.centery - 8;
	r.center = 1;
	if (button[round].down) {
		ggprint16(&r, 0, button[round].text_color, "Goodluck!");
	} else {
		ggprint16(&r, 0, button[round].text_color, button[round].text);
	}
}

void show_steven_credits(int x, int y)
{
    Rect r;
    r.bot = y - 40;
    r.left = x;
    r.center = 1;
    ggprint16(&r, 16, 0x00000000, "Steven made the buttons work.");

}
