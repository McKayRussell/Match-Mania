//3350
//program: rainforest.cpp
//author:  Gordon Griesel
//date:    2013 to 2021
//
//This program demonstrates the use of OpenGL and XWindows
//
//Texture maps are displayed.
//Press B to see bigfoot roaming his forest.
//
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
//#include "ppm.h"
#include "fonts.h"

//defined types
typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                      (c)[1]=(a)[1]-(b)[1]; \
                      (c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define ALPHA 1

//-----------------------------------------------------------------------------
//Setup timers
//clock_gettime(CLOCK_REALTIME, &timePause);
const double physicsRate = 1.0 / 30.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;
unsigned int upause=0;
double timeDiff(struct timespec *start, struct timespec *end) {
	return (double)(end->tv_sec - start->tv_sec ) +
			(double)(end->tv_nsec - start->tv_nsec) * oobillion;
}
void timeCopy(struct timespec *dest, struct timespec *source) {
	memcpy(dest, source, sizeof(struct timespec));
}
//-----------------------------------------------------------------------------

extern void show_mckays_credits(int, int);
extern void show_dat_credits(int, int);
extern void show_steven_credits(int, int);
extern void show_clementes_credits(int, int);
void flipCard(int mx, int my);
void drawCardFront(int row, int col, float w);

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};
Image img[9] = {
"./images/bigfoot.png",
"./images/pic.png",
"./images/front2.PNG",
"./images/umbrella.png",
"./images/credits.png",
"./images/back2.png",
"./images/witch.png",
"./images/queen.jpg",
"./images/Menu.jpg",
};

class Global {
public:
	int done;
	int xres, yres;
	GLuint bigfootTexture;
    GLuint witchTexture;
	GLuint silhouetteTexture;
	GLuint forestTexture;
	GLuint forestTransTexture;
	GLuint umbrellaTexture;
	GLuint creditsTexture;
    GLuint cardTexture;
	GLuint cardFront; 
	GLuint StartTexture;
    int showBigfoot;
	int witch;
    int forest;
	int silhouette;
	int trees;
	int showRain;
	int showUmbrella;
	int deflection;
	int show_credits;
    int round1, round2, round3;
	int flipped;
	int Startscreen;
	// int flippedTwo;
	int lrbutton;
	int cardRow;
	int cardCol;
    Global() {
		logOpen();
		done=0;
		xres=1100;
		yres=800;
        showBigfoot=0;
		forest=0;
		silhouette=1;
		trees=0;
		showRain=0;
		showUmbrella=0;
		deflection=0;
        show_credits=0;
        round1=round2=round3=0;
	    witch=0;
		flipped = 0;
		// flippedTwo = 0;
		cardRow = 0;
		cardCol = 0;
		Startscreen = 1;
		lrbutton = 0; // <--------------
    }
	~Global() {
		logClose();
	}
} g;
//
/* ADDED FOR BUTTON */
class t_button {
public:
	Rect r;
	char text[32];
	int over;
	int down;
	int click;
	float color[3];
	float dcolor[3];
	unsigned int text_color;
} button;

class Witch {
public:
	Vec pos;
	Vec vel;
} witch;

class Bigfoot {
public:
	Vec pos;
	Vec vel;
} bigfoot;

class Raindrop {
public:
	int type;
	int linewidth;
	int sound;
	Vec pos;
	Vec lastpos;
	Vec vel;
	Vec maxvel;
	Vec force;
	float length;
	float color[4];
	Raindrop *prev;
	Raindrop *next;
} *rainhead = NULL;
int ndrops=1;
int totrain=0;
int maxrain=0;
void deleteRain(Raindrop *node);
void cleanupRaindrops(void);
//
#define UMBRELLA_FLAT  0
#define UMBRELLA_ROUND 1
class Umbrella {
public:
	int shape;
	Vec pos;
	Vec lastpos;
	float width;
	float width2;
	float radius;
} umbrella;

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() { }
	X11_wrapper(int w, int h) {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		XSetWindowAttributes swa;
		setup_screen_res(g.xres, g.yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			printf("\n\tcannot connect to X server\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XWindowAttributes getWinAttr;
		XGetWindowAttributes(dpy, root, &getWinAttr);
		int fullscreen=0;
		g.xres = w;
		g.yres = h;
		if (!w && !h) {
			//Go to fullscreen.
			g.xres = getWinAttr.width;
			g.yres = getWinAttr.height;
			//When window is fullscreen, there is no client window
			//so keystrokes are linked to the root window.
			XGrabKeyboard(dpy, root, False,
				GrabModeAsync, GrabModeAsync, CurrentTime);
			fullscreen=1;
		}
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | 
						KeyPressMask | 
						KeyReleaseMask |
						PointerMotionMask | 
						MotionNotify | 
						ButtonPress | 
						ButtonRelease |
						StructureNotifyMask | 
						SubstructureNotifyMask;
		unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
		if (fullscreen) {
			winops |= CWOverrideRedirect;
			swa.override_redirect = True;
		}
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
			vi->depth, InputOutput, vi->visual, winops, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		//show_mouse_cursor(0);
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "OpenGL sample  -  Press Esc");
	}
	void checkResize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		set_title();
	}
	void setup_screen_res(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void set_mouse_position(int x, int y) {
		XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
		if (onoff) {
			//this removes our own blank cursor.
			XUndefineCursor(dpy, win);
			return;
		}
		//vars to make blank cursor
		Pixmap blank;
		XColor dummy;
		char data[1] = {0};
		Cursor cursor;
		//make a blank cursor
		blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
		if (blank == None)
			printf("error: out of memory.\n");
		cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(dpy, blank);
		//this makes the cursor. then set it using this function
		XDefineCursor(dpy, win, cursor);
		//after you do not need the cursor anymore use this function.
		//it will undo the last change done by XDefineCursor
		//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11(g.xres, g.yres);

//function prototypes
void initOpengl(void);
void checkMouse(XEvent *e);
int checkKeys(XEvent *e);
void init();
void physics(void);
void render(void);


int main()
{
	initOpengl();
	init();
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	int done = 0;
	while (!done) {
		while (x11.getXPending()) {
			//XEvent e;
			//XNextEvent(dpy, &e);
			XEvent e = x11.getXNextEvent();
			x11.checkResize(&e);
			checkMouse(&e);
			done = checkKeys(&e);
		}
		//
		//Below is a process to apply physics at a consistent rate.
		//1. Get the current time.
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		//2. How long since we were here last?
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		//3. Save the current time as our new starting time.
		timeCopy(&timeStart, &timeCurrent);
		//4. Add time-span to our countdown amount.
		physicsCountdown += timeSpan;
		//5. Has countdown gone beyond our physics rate? 
		//       if yes,
		//           In a loop...
		//              Apply physics
		//              Reducing countdown by physics-rate.
		//              Break when countdown < physics-rate.
		//       if no,
		//           Apply no physics this frame.
		while (physicsCountdown >= physicsRate) {
			//6. Apply physics
			physics();
			//7. Reduce the countdown by our physics-rate
			physicsCountdown -= physicsRate;
		}
		//Always render every frame.
		render();
		x11.swapBuffers();
	}
	//cleanupXWindows();
	cleanup_fonts();
	return 0;
}

unsigned char *buildAlphaData(Image *img)
{
	//Add 4th component to an RGB stream...
	//RGBA
	//When you do this, OpenGL is able to use the A component to determine
	//transparency information.
	//It is used in this application to erase parts of a texture-map from view.
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		//-----------------------------------------------
		//get largest color component...
		//*(ptr+3) = (unsigned char)((
		//		(int)*(ptr+0) +
		//		(int)*(ptr+1) +
		//		(int)*(ptr+2)) / 3);
		//d = a;
		//if (b >= a && b >= c) d = b;
		//if (c >= a && c >= b) d = c;
		//*(ptr+3) = d;
		//-----------------------------------------------
		//this code optimizes the commented code above.
		//
        //todo: need credits for student work.
        //
        *(ptr+3) = (a|b|c);
		//-----------------------------------------------
		ptr += 4;
		data += 3;
	}
	return newdata;
}

void initOpengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, g.xres, g.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
	//
	//load the images file into a ppm structure.
	//
	//bigfootImage     = ppm6GetImage("./images/bigfoot.ppm");
	//forestImage      = ppm6GetImage("./images/forest.ppm");
	//forestTransImage = ppm6GetImage("./images/forestTrans.ppm");
	//umbrellaImage    = ppm6GetImage("./images/umbrella.ppm");
	//create opengl texture elements    

    glGenTextures(1, &g.bigfootTexture);
	glGenTextures(1, &g.witchTexture); 
    glGenTextures(1, &g.silhouetteTexture);
	glGenTextures(1, &g.forestTexture);
	glGenTextures(1, &g.umbrellaTexture);
    glGenTextures(1, &g.creditsTexture);
    glGenTextures(1, &g.cardTexture); 
	glGenTextures(1, &g.cardFront);
	glGenTextures(1, &g.StartTexture);
    
	//-------------------------------------------------------------------------
	//Startscreen
	//
	
    int w8 = img[8].width;
	int h8 = img[8].height;
	//
	glBindTexture(GL_TEXTURE_2D, g.StartTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w8, h8, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[8].data); 
    glBindTexture(GL_TEXTURE_2D, 0);
	//-------------------------------------------------------------------------
	//back of card
	//
	
    int w2 = img[5].width;
	int h2 = img[5].height;
	//
	glBindTexture(GL_TEXTURE_2D, g.cardTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w2, h2, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[5].data); 
    glBindTexture(GL_TEXTURE_2D, 0);
    /*
	glBindTexture(GL_TEXTURE_2D, g.cardTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[4].width, img[4].height,
									0, GL_RGB, GL_UNSIGNED_BYTE, img[4].data);
	    */
    //-------------------------------------------------------------------------
	//front of card
	//
	
	// if we want to change the width or height to the actual width or height
	// 	of the image for the front then it is img[7]
    int frontW = img[7].width;
	int frontH = img[7].height;
	//
	glBindTexture(GL_TEXTURE_2D, g.cardFront);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, frontW, frontH, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[7].data); 
    glBindTexture(GL_TEXTURE_2D, 0);

	//
    //-------------------------------------------------------------------------
	//bigfoot
	//
	int w = img[0].width;
	int h = img[0].height;
	//
	glBindTexture(GL_TEXTURE_2D, g.bigfootTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[0].data);
	
    //-------------------------------------------------------------------------	
    //
	//silhouette
	//this is similar to a sprite graphic
	//
    
	glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	unsigned char *silhouetteData = buildAlphaData(&img[0]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
								GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);

    free(silhouetteData); 
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);    
    //-------------------------------------------------------------------------
	//witch
    int w3 = img[6].width;
	int h3 = img[6].height;
    glBindTexture(GL_TEXTURE_2D, g.witchTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w3, h3, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[6].data);
    //-------------------------------------------------------------------------
    //
	//witch silhouette
	//this is similar to a sprite graphic
	//
    
	glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
    unsigned char *silData2 = buildAlphaData(&img[6]);	
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w3, h3, 0,
								GL_RGBA, GL_UNSIGNED_BYTE, silData2);
    free(silData2);     
   //-------------------------------------------------------------------------
	
    //
	//umbrella
	//
    
	glBindTexture(GL_TEXTURE_2D, g.umbrellaTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	silhouetteData = buildAlphaData(&img[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
								GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
	free(silhouetteData); 
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//	GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data); 
	//-------------------------------------------------------------------------
    //
	//credits
	glBindTexture(GL_TEXTURE_2D, g.creditsTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[4].width, img[4].height,
									0, GL_RGB, GL_UNSIGNED_BYTE, img[4].data);
    //-------------------------------------------------------------------------
	//
	//forest
	glBindTexture(GL_TEXTURE_2D, g.forestTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, img[1].width, img[1].height,
									0, GL_RGB, GL_UNSIGNED_BYTE, img[1].data);
	//-------------------------------------------------------------------------
	//
	//forest transparent part
	//
	glBindTexture(GL_TEXTURE_2D, g.forestTransTexture);
	//
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//
	//must build a new set of data...
	w = img[2].width;
	h = img[2].height;
	unsigned char *ftData = buildAlphaData(&img[2]);	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
											GL_RGBA, GL_UNSIGNED_BYTE, ftData);
	free(ftData);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
	//GL_RGB, GL_UNSIGNED_BYTE, bigfootImage->data);
	//-------------------------------------------------------------------------
}

void initSounds()
{

}

void init() {
	umbrella.pos[0] = 220.0;
	umbrella.pos[1] = (double)(g.yres-200);
	VecCopy(umbrella.pos, umbrella.lastpos);
	umbrella.width = 200.0;
	umbrella.width2 = umbrella.width * 0.5;
	umbrella.radius = (float)umbrella.width2;
	umbrella.shape = UMBRELLA_FLAT;
	MakeVector(-150.0,180.0,0.0, bigfoot.pos);
	MakeVector(6.0,0.0,0.0, bigfoot.vel);
    MakeVector(-150.0,180.0,0.0, witch.pos);
	MakeVector(6.0,0.0,0.0, witch.vel);

	//initialize buttons...
	button.r.width = 200;
	button.r.height = 100;
	button.r.left = g.xres/2 - button.r.width/2;
	button.r.bot = 50;
	button.r.right =
		button.r.left + button.r.width;
	button.r.top = button.r.bot + button.r.height;
	button.r.centerx =
		(button.r.left + button.r.right) / 2;
	button.r.centery =
		(button.r.bot + button.r.top) / 2;
	strcpy(button.text, "Press to Play");
	button.down = 0;
	button.click = 0;
	button.color[0] = 0.4f;
	button.color[1] = 0.4f;
	button.color[2] = 0.7f;
	button.dcolor[0] = button.color[0] * 0.5f;
	button.dcolor[1] = button.color[1] * 0.5f;
	button.dcolor[2] = button.color[2] * 0.5f;
	button.text_color = 0x00ffffff;

}
// Mouse Button Clicked
void mouse_click(int action)
{
	if (action == 1) {
		if (button.over) {
			button.down = 1;
			button.click = 1;
			g.Startscreen ^= 1;
			g.round1 ^= 1;
		}
	}		

}

void checkMouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;

	g.lrbutton = 0;
	//
	// if (e->type != ButtonRelease &&
	// 	e->type != ButtonPress &&
	// 	e->type != MotionNotify) {
	// 	return;
	// }
	int mx = e->xbutton.x;
	int my = e->xbutton.y;
	if (e->type == ButtonRelease) {
		// return;
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			g.lrbutton = 1;
			flipCard(mx, my);
			// drawCardFront(0, 0, 45.0);
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	my = g.yres - my;
	if (e->type == MotionNotify) {
		if (savex != mx || savey != my) {
			//Mouse moved
			savex = mx;
			savey = my;
		}
	}

	// is mouse over square?
	button.over = 0;
	if (savex >= button.r.left &&
		savex <= button.r.right &&
		savey >= button.r.bot &&
		savey <= button.r.top) {
			button.over = 1;
	}
	if (g.lrbutton)
		mouse_click(1);
}

int checkKeys(XEvent *e)
{
	//keyboard input?
	static int shift=0;
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	if (e->type == KeyRelease) {
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (key == XK_Shift_L || key == XK_Shift_R) {
		shift=1;
		return 0;
	}
	switch (key) {
        case XK_1:
            g.round2 = g.round3 = 0;
			g.Startscreen ^= 1;
            g.round1 ^= 1;
            break;
        case XK_2:
            g.round1 = g.round3 = 0;
			g.Startscreen ^= 1;
            g.round2 ^= 1;
            break;
        case XK_3:
            g.round1 = g.round2 = 0;
			g.Startscreen ^= 1;
            g.round3 ^= 1;
            break; 
        case XK_c:
            g.show_credits ^= 1;
            break;
		case XK_b:
			g.showBigfoot ^= 1;
			if (g.showBigfoot) {
				bigfoot.pos[0] = -250.0;
			}
			break;
	    case XK_6:
			g.witch ^= 1;
			if (g.witch) {
				witch.pos[0] = -250.0;
			}
			break;
        case XK_d:
			g.deflection ^= 1;
			break;
		case XK_f:
			g.forest ^= 1;
			break;
		case XK_s:
			g.silhouette ^= 1;
			printf("silhouette: %i\n", g.silhouette);
			break;
		case XK_t:
			g.trees ^= 1;
			break;
		case XK_u:
			g.showUmbrella ^= 1;
			break;
		case XK_p:
			umbrella.shape ^= 1;
			break;
		case XK_r:
			g.showRain ^= 1;
			break;
		case XK_Left:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[0] -= 10.0;
			break;
		case XK_Right:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[0] += 10.0;
			break;
		case XK_Up:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[1] += 10.0;
			break;
		case XK_Down:
			VecCopy(umbrella.pos, umbrella.lastpos);
			umbrella.pos[1] -= 10.0;
			break;
		case XK_equal:
			if (++ndrops > 40)
				ndrops=40;
			break;
		case XK_minus:
			if (--ndrops < 0)
				ndrops = 0;
			break;
		case XK_n:
			break;
		case XK_w:
			if (shift) {
				//shrink the umbrella
				umbrella.width *= (1.0 / 1.05);
			} else {
				//enlarge the umbrella
				umbrella.width *= 1.05;
			}
			//half the width
			umbrella.width2 = umbrella.width * 0.5;
			umbrella.radius = (float)umbrella.width2;
			break;
		case XK_Escape:
			return 1;
			//break;
	}
	return 0;
}

Flt VecNormalize(Vec vec)
{
	Flt len, tlen;
	Flt xlen = vec[0];
	Flt ylen = vec[1];
	Flt zlen = vec[2];
	len = xlen*xlen + ylen*ylen + zlen*zlen;
	if (len == 0.0) {
		MakeVector(0.0,0.0,1.0,vec);
		return 1.0;
	}
	len = sqrt(len);
	tlen = 1.0 / len;
	vec[0] = xlen * tlen;
	vec[1] = ylen * tlen;
	vec[2] = zlen * tlen;
	return(len);
}

void cleanupRaindrops()
{
	Raindrop *s;
	while (rainhead) {
		s = rainhead->next;
		free(rainhead);
		rainhead = s;
	}
	rainhead=NULL;
}

void deleteRain(Raindrop *node)
{
	//remove a node from linked list
	//Log("deleteRain()...\n");
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//Log("only 1 item in list.\n");
			rainhead = NULL;
		} else {
			//Log("at beginning of list.\n");
			node->next->prev = NULL;
			rainhead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//Log("at end of list.\n");
			node->prev->next = NULL;
		} else {
			//Log("in middle of list.\n");
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	free(node);
	node = NULL;
}

void moveWitch()
{
	//move bigfoot...
	int addgrav = 1;
	//Update position
	witch.pos[0] += witch.vel[0];
	witch.pos[1] += witch.vel[1];
	//Check for collision with window edges
	if ((witch.pos[0] < -140.0 && witch.vel[0] < 0.0) ||
		(witch.pos[0] >= (float)g.xres+140.0 &&
		witch.vel[0] > 0.0))
	{
		witch.vel[0] = -witch.vel[0];
		addgrav = 0;
	}
	if ((witch.pos[1] < 150.0 && witch.vel[1] < 0.0) ||
		(witch.pos[1] >= (float)g.yres && witch.vel[1] > 0.0)) {
	    witch.vel[1] = -witch.vel[1];
		addgrav = 0;
	}
	//Gravity?
	if (addgrav)
		witch.vel[1] -= 0.75;
}

void moveBigfoot()
{
	//move bigfoot...
	int addgrav = 1;
	//Update position
	bigfoot.pos[0] += bigfoot.vel[0];
	bigfoot.pos[1] += bigfoot.vel[1];
	//Check for collision with window edges
	if ((bigfoot.pos[0] < -140.0 && bigfoot.vel[0] < 0.0) ||
		(bigfoot.pos[0] >= (float)g.xres+140.0 &&
		bigfoot.vel[0] > 0.0))
	{
		bigfoot.vel[0] = -bigfoot.vel[0];
		addgrav = 0;
	}
	if ((bigfoot.pos[1] < 150.0 && bigfoot.vel[1] < 0.0) ||
		(bigfoot.pos[1] >= (float)g.yres && bigfoot.vel[1] > 0.0)) {
		bigfoot.vel[1] = -bigfoot.vel[1];
		addgrav = 0;
	}
	//Gravity?
	if (addgrav)
		bigfoot.vel[1] -= 0.75;
}


void createRaindrop(const int n)
{
	//create new rain drops...
	int i;
	for (i=0; i<n; i++) {
		Raindrop *node = (Raindrop *)malloc(sizeof(Raindrop));
		if (node == NULL) {
			Log("error allocating node.\n");
			exit(EXIT_FAILURE);
		}
		node->prev = NULL;
		node->next = NULL;
		node->sound=0;
		node->pos[0] = rnd() * (float)g.xres;
		node->pos[1] = rnd() * 100.0f + (float)g.yres;
		VecCopy(node->pos, node->lastpos);
		node->vel[0] = 
		node->vel[1] = 0.0f;
		node->color[0] = rnd() * 0.2f + 0.8f;
		node->color[1] = rnd() * 0.2f + 0.8f;
		node->color[2] = rnd() * 0.2f + 0.8f;
		node->color[3] = rnd() * 0.5f + 0.3f; //alpha
		node->linewidth = random(8)+1;
		//larger linewidth = faster speed
		node->maxvel[1] = (float)(node->linewidth*16);
		node->length = node->maxvel[1] * 0.2f + rnd();
		//put raindrop into linked list
		node->next = rainhead;
		if (rainhead != NULL)
			rainhead->prev = node;
		rainhead = node;
		++totrain;
	}
}

void checkRaindrops()
{
	if (random(100) < 50) {
		createRaindrop(ndrops);
	}
	//
	//move rain droplets
	Raindrop *node = rainhead;
	while (node) {
		//force is toward the ground
		node->vel[1] += gravity;
		VecCopy(node->pos, node->lastpos);
		node->pos[0] += node->vel[0] * timeslice;
		node->pos[1] += node->vel[1] * timeslice;
		if (fabs(node->vel[1]) > node->maxvel[1])
			node->vel[1] *= 0.96;
		node->vel[0] *= 0.999;
		node = node->next;
	}
	//}
	//
	//check rain droplets
	int n=0;
	node = rainhead;
	while (node) {
		n++;
		#ifdef USE_SOUND
		if (node->pos[1] < 0.0f) {
			//raindrop hit ground
			if (!node->sound && play_sounds) {
				//small chance that a sound will play
				int r = random(50);
				if (r==1) {
					//play sound here...


				}
				//sound plays once per raindrop
				node->sound=1;
			}
		}
		#endif //USE_SOUND
		//collision detection for raindrop on umbrella
		if (g.showUmbrella) {
			if (umbrella.shape == UMBRELLA_FLAT) {
				if (node->pos[0] >= (umbrella.pos[0] - umbrella.width2) &&
					node->pos[0] <= (umbrella.pos[0] + umbrella.width2)) {
					if (node->lastpos[1] > umbrella.lastpos[1] ||
						node->lastpos[1] > umbrella.pos[1]) {
						if (node->pos[1] <= umbrella.pos[1] ||
							node->pos[1] <= umbrella.lastpos[1]) {
							if (node->linewidth > 1) {
								Raindrop *savenode = node->next;
								deleteRain(node);
								node = savenode;
								continue;
							}
						}
					}
				}
			}
			if (umbrella.shape == UMBRELLA_ROUND) {
				float d0 = node->pos[0] - umbrella.pos[0];
				float d1 = node->pos[1] - umbrella.pos[1];
				float distance = sqrt((d0*d0)+(d1*d1));
				//Log("distance: %f  umbrella.radius: %f\n",
				//							distance,umbrella.radius);
				if (distance <= umbrella.radius &&
										node->pos[1] > umbrella.pos[1]) {
					if (node->linewidth > 1) {
						if (g.deflection) {
							//deflect raindrop
							double dot;
							Vec v, up = {0,1,0};
							VecSub(node->pos, umbrella.pos, v);
							VecNormalize(v);
							node->pos[0] =
								umbrella.pos[0] + v[0] * umbrella.radius;
							node->pos[1] =
								umbrella.pos[1] + v[1] * umbrella.radius;
							dot = VecDot(v,up);
							dot += 1.0;
							node->vel[0] += v[0] * dot * 1.0;
							node->vel[1] += v[1] * dot * 1.0;
						} else {
							Raindrop *savenode = node->next;
							deleteRain(node);
							node = savenode;
							continue;
						}
					}
				}
			}
		}
		if (node->pos[1] < -20.0f) {
			//rain drop is below the visible area
			Raindrop *savenode = node->next;
			deleteRain(node);
			node = savenode;
			continue;
		}
		node = node->next;
	}
	if (maxrain < n)
		maxrain = n;
}

void physics()
{
	if (g.showBigfoot)
		moveBigfoot();
	if (g.witch)
		moveWitch();	
    if (g.showRain)
		checkRaindrops();
}

void drawUmbrella()
{
	//Log("drawUmbrella()...\n");
	if (umbrella.shape == UMBRELLA_FLAT) {
		glColor4f(1.0f, 0.2f, 0.2f, 0.5f);
		glLineWidth(8);
		glBegin(GL_LINES);
			glVertex2f(umbrella.pos[0]-umbrella.width2, umbrella.pos[1]);
			glVertex2f(umbrella.pos[0]+umbrella.width2, umbrella.pos[1]);
		glEnd();
		glLineWidth(1);
	} else {
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glPushMatrix();
		glTranslatef(umbrella.pos[0],umbrella.pos[1],umbrella.pos[2]);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBindTexture(GL_TEXTURE_2D, g.umbrellaTexture);
		glBegin(GL_QUADS);
			float w = umbrella.width2;
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  w);
			glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  w);
			glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -w);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -w);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
	}
}

void drawRaindrops()
{
	Raindrop *node = rainhead;
	while (node) {
		glPushMatrix();
		glTranslated(node->pos[0],node->pos[1],node->pos[2]);
		glColor4fv(node->color);
		glLineWidth(node->linewidth);
		glBegin(GL_LINES);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(0.0f, node->length);
		glEnd();
		glPopMatrix();
		node = node->next;
	}
	glLineWidth(1);
}

//Dat Pham
//inputs: row, column, size of card (width)
void drawCardBack(int row, int col, float w)
{
	// STARTING PTS
	// (top/left alignment)
	// float x = 100.0;
	// float y = g.yres-100.0; 
	
	//(top/middle alignment)
	float x = g.xres/2;
	float y = g.yres-100.0;    

	for (int i=0; i<col; i++) {
		for (int j=0; j<row; j++) {
			glPushMatrix();
			glTranslatef(x+(i*(w*2+10)), y-(j*(w*2+40)), 0);
			glBindTexture(GL_TEXTURE_2D, g.cardTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w+30);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w+30);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
			glEnd();
			glPopMatrix();
		}
	}
}

//McKay Russell
//inputs: mouse x coordinate, mouse y coordinate
void flipCard(int mx, int my)
{
	float x = g.xres/2 - 40;
	float y = 25;
	int rows = 0;
	int cols = 0;

	if (g.round1) {
		rows = 3;
		cols = 4;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (mx > x+(100 * j) && mx < x+(100 * j)+85 && my > y+(130 * i)
						&& my < y+(130 * i)+120)
				{
					g.cardRow = i;
					g.cardCol = j;
					g.flipped ^= 1;
				}
			}
		}
	}

	if (g.round2) {
		rows = 4;
		cols = 4;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (mx > x+(100 * j) && mx < x+(100 * j)+85 && my > y+(130 * i)
						&& my < y+(130 * i)+120)
				{
					g.cardRow = i;
					g.cardCol = j;
					g.flipped ^= 1;
				}
			}
		}
	}

	if (g.round3) {
		rows = 5;
		cols = 5;
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (mx > x+(100 * j) && mx < x+(100 * j)+85 && my > y+(130 * i)
						&& my < y+(130 * i)+120)
				{
					g.cardRow = i;
					g.cardCol = j;
					g.flipped ^= 1;
				}
			}
		}
	}
}

void render()
{
	Rect r;

	//Clear the screen
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//
	//draw a quad with texture
	float wid = 120.0f;
	glColor3f(1.0, 1.0, 1.0);
   
    // if (g.forest) {
	// 	glBindTexture(GL_TEXTURE_2D, g.forestTexture);
	// 	glBegin(GL_QUADS);
	// 		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
	// 		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
	// 		glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
	// 		glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
	// 	glEnd(); 
	// }

    if (g.showBigfoot) {
		glPushMatrix();
		glTranslatef(bigfoot.pos[0], bigfoot.pos[1], bigfoot.pos[2]);
		if (!g.silhouette) {
			glBindTexture(GL_TEXTURE_2D, g.bigfootTexture);
		} else {
			glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		}
		glBegin(GL_QUADS);
			if (bigfoot.vel[0] > 0.0) {
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
			} else {
				glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
				glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
			}
		glEnd();
		glPopMatrix();
		//
		if (g.trees && g.silhouette) {
			glBindTexture(GL_TEXTURE_2D, g.forestTransTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(0.5, 0);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(0.5, g.yres);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
			glEnd();
		}
		glDisable(GL_ALPHA_TEST);
	}

    if (g.witch) {
		glPushMatrix();
		glTranslatef(witch.pos[0], witch.pos[1], witch.pos[2]);
		
        if (!g.silhouette) {
			glBindTexture(GL_TEXTURE_2D, g.witchTexture);
		} else {
			glBindTexture(GL_TEXTURE_2D, g.silhouetteTexture);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(255,255,255,255);
		} 
		glBegin(GL_QUADS);
			if (witch.vel[0] < 0.0) {
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
			} else {
				glTexCoord2f(1.0f, 1.0f); glVertex2i(-wid,-wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i( wid, wid);
				glTexCoord2f(0.0f, 1.0f); glVertex2i( wid,-wid);
			}
		glEnd();
		glPopMatrix();
		//
        
		if (g.trees && g.silhouette) {
			glBindTexture(GL_TEXTURE_2D, g.forestTransTexture);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 1.0f); glVertex2i(0.5, 0);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(0.5, g.yres);
				glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
			glEnd();
		} 
		glDisable(GL_ALPHA_TEST);
	}


    if (g.round1) {
		glBindTexture(GL_TEXTURE_2D, g.forestTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
		glEnd(); 
		glPopMatrix();

        drawCardBack(3,4,45.0);	
		unsigned int c = 0x00ffff44;
		r.bot = g.yres - 60;
		r.left = 250;
		r.center = 0;
		int h = 12;
		ggprint16(&r, h, c, "Press 1 for Menu");
	}
    
    if (g.round2) {
		glBindTexture(GL_TEXTURE_2D, g.forestTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
		glEnd(); 
        drawCardBack(4,4,45.0);	
		unsigned int c = 0x00ffff44;
		r.bot = g.yres - 60;
		r.left = 250;
		r.center = 0;
		int h = 12;
		ggprint16(&r, h, c, "Press 2 for Menu");
	}
    
    if (g.round3) {
		glBindTexture(GL_TEXTURE_2D, g.forestTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
		glEnd(); 
        drawCardBack(5,5,45.0);	
		unsigned int c = 0x00ffff44;
		r.bot = g.yres - 60;
		r.left = 250;
		r.center = 0;
		int h = 12;
		ggprint16(&r, h, c, "Press 3 for Menu");
	}

	if (g.flipped == 1) {
		float x = g.xres/2;
		float y = g.yres-100.0;
		int w = 45.0;
		// printf("cardCol: %d, cardRow: %d ", g.cardCol, g.cardRow);
		glPushMatrix();
		glTranslatef(x+(g.cardCol*(w*2+10)), y-(g.cardRow*(w*2+40)), 0);
		glBindTexture(GL_TEXTURE_2D, g.cardFront);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w+30);
			glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w+30);
			glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
		glEnd();
		glPopMatrix();
	}

	if (g.Startscreen){
		glBindTexture(GL_TEXTURE_2D, g.StartTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
		glEnd();
		unsigned int c = 0x00ffff44;
		r.bot = g.yres - 20;
		r.left = 10;
		r.center = 0;
		int h = 12;
		ggprint8b(&r, h, c, "1 - Round 1");
		ggprint8b(&r, h, c, "2 - Round 2");    
		ggprint8b(&r, h, c, "3 - Round 3");
		ggprint8b(&r, h, c, "6 - witch");
		ggprint8b(&r, h, c, "B - Bigfoot");
		ggprint8b(&r, h, c, "F - Forest");
		ggprint8b(&r, h, c, "S - Silhouette");
		ggprint8b(&r, h, c, "T - Trees");
		ggprint8b(&r, h, c, "U - Umbrella");
		ggprint8b(&r, h, c, "R - Rain");
		ggprint8b(&r, h, c, "D - Deflection");
		ggprint8b(&r, h, c, "N - Sounds");
		ggprint8b(&r, h, 0x00ff0000, "Press c for credits");

		// Draw Buttons
		if (button.over) {
			glColor3f(1.0f, 0.0f, 0.0f);
			//draw a highlight around button
			glLineWidth(2);
			glBegin(GL_LINE_LOOP);
				glVertex2i(button.r.left-2,  button.r.bot-2);
				glVertex2i(button.r.left-2,  button.r.top+2);
				glVertex2i(button.r.right+2, button.r.top+2);
				glVertex2i(button.r.right+2, button.r.bot-2);
				glVertex2i(button.r.left-2,  button.r.bot-2);
			glEnd();
			glLineWidth(1);
		}
		if (button.down) {
			glColor3fv(button.dcolor);
		} else {
			glColor3fv(button.color);
		}
		glBegin(GL_QUADS);
			glVertex2i(button.r.left,  button.r.bot);
			glVertex2i(button.r.left,  button.r.top);
			glVertex2i(button.r.right, button.r.top);
			glVertex2i(button.r.right, button.r.bot);
		glEnd();
		r.left = button.r.centerx;
		r.bot  = button.r.centery-8;
		r.center = 1;
		if (button.down) {
			ggprint16(&r, 0, button.text_color, "Pressed!");
		} else {
			ggprint16(&r, 0, button.text_color, button.text);
		}
	}

	// if (g.flippedTwo == 1) {
	// 	float x = g.xres/2;
	// 	float y = g.yres-100.0;
	// 	int w = 45.0;
	// 	// printf("cardCol: %d, cardRow: %d ", g.cardCol, g.cardRow);
	// 	glPushMatrix();
	// 	glTranslatef(x+(g.cardCol*(w*2+10)), y-(g.cardRow*(w*2+40)), 0);
	// 	glBindTexture(GL_TEXTURE_2D, g.cardFront);
	// 	glBegin(GL_QUADS);
	// 		glTexCoord2f(0.0f, 1.0f); glVertex2i(-w,-w);
	// 		glTexCoord2f(0.0f, 0.0f); glVertex2i(-w, w+30);
	// 		glTexCoord2f(1.0f, 0.0f); glVertex2i( w, w+30);
	// 		glTexCoord2f(1.0f, 1.0f); glVertex2i( w,-w);
	// 	glEnd();
	// 	glPopMatrix();
	// }

    if (g.show_credits){
        glBindTexture(GL_TEXTURE_2D, g.creditsTexture);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(0, g.yres);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(g.xres, g.yres);
			glTexCoord2f(1.0f, 1.0f); glVertex2i(g.xres, 0);
		glEnd();

		show_mckays_credits(g.xres / 2, g.yres / 2);
        show_dat_credits(g.xres / 2, g.yres / 2);
        show_steven_credits(g.xres / 2, g.yres / 2);
        show_clementes_credits(g.xres / 2, g.yres / 2);
    }

	glDisable(GL_TEXTURE_2D);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glBegin(GL_QUADS);
	//	glVertex2i(10,10);
	//	glVertex2i(10,60);
	//	glVertex2i(60,60);
	//	glVertex2i(60,10);
	//glEnd();
	//return;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	if (g.showRain)
		drawRaindrops();
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	//
	if (g.showUmbrella)
		drawUmbrella();
	glBindTexture(GL_TEXTURE_2D, 0);
	//
}

