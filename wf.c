#ifdef GLX
#include <GL/glx.h>
#include <sys/unistd.h>
#define EV(y) ev.x##y
#else
#include <SDL.h>
#include <SDL_opengl.h>
#define ButtonPress SDL_MOUSEBUTTONDOWN
#define ButtonRelease SDL_MOUSEBUTTONUP
#define MotionNotify SDL_MOUSEMOTION
#define ClientMessage SDL_QUIT
#define EV(y) ev.y
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#define OOXOO 12
#define OOOXX 11
#define OOOOX 10
#define OXOXO 9
#define XOOOO 8
#define XOOOX 7
#define XOOXO 6
#define XOOXX 5
#define XOXOX 4
#define XXOOX 3
#define XXOXX 2
#define XXXXO 1
#define XXXXX 0
#define A(a,b,c,d,e) a|b<<4,c|d<<4,e
#define B(a,b,c,d,e) |a<<4,b|c<<4,d|e<<4,
int T;
static const char win[]={
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,3,3,2,3,3,2,2,2,2,2,2,
	2,2,2,2,2,3,3,2,3,3,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,3,2,2,2,3,2,2,2,2,2,2,
	2,2,2,2,2,2,3,2,3,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,3,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
};
static const uint8_t abc[]={
	A(
	XXXXX,
	XOOOX,
	XOXOX,
	XOOOX,
	XXXXX)
	B(
	OOXOO,
	OOXOO,
	OOXOO,
	OOXOO,
	OOXOO)
	A(
	XXXXX,
	OOOOX,
	XXXXX,
	XOOOO,
	XXXXX)
	B(
	XXXXX,
	OOOOX,
	XXXXX,
	OOOOX,
	XXXXX)
	A(
	XOOOX,
	XOOOX,
	XXXXX,
	OOOOX,
	OOOOX)
	B(
	XXXXX,
	XOOOO,
	XXXXX,
	OOOOX,
	XXXXX)
	A(
	XOOOO,
	XOOOO,
	XXXXX,
	XOOOX,
	XXXXX)
	B(
	XXXXX,
	OOOOX,
	OOOOX,
	OOOOX,
	OOOOX)
	A(
	XXXXX,
	XOOOX,
	XXXXX,
	XOOOX,
	XXXXX)
	B(
	XXXXX,
	XOOOX,
	XXXXX,
	OOOOX,
	OOOOX)
};
static void tfChar(int x,int y,char c){
	for(int j=0;j<5;j++)
		for(int i=0;i<5;i++)
			if((0x4c41418a7359edff>>5*(abc[c*5+j>>1]>>(((c^j)&1)<<2)&15))&1<<i)glVertex2i(x+i,y+j);
}
#define case(x) break;case x:;
unsigned char F[256];
int lose;
void makefield(){
	int m=0;
	memset(F,0,256);
	for(;;){
		int x=rand()&255;
		if(!F[x]){
			F[x]=1;
			if(++m==40)
				return;
		}
	}
}
int getxy(int x,int y){
	return F[x&15|(y&15)<<4]&1;
}
int getmar(int x,int y){
	return(F[x&15|(y&15)<<4]&4)==4;
}
int getmark(int x,int y){
	return getmar(x-1,y-1)+getmar(x-1,y)+getmar(x-1,y+1)+getmar(x,y-1)+getmar(x,y+1)+getmar(x+1,y-1)+getmar(x+1,y)+getmar(x+1,y+1);
}
int getdot(int x,int y){
	return getxy(x-1,y-1)+getxy(x-1,y)+getxy(x-1,y+1)+getxy(x,y-1)+getxy(x,y+1)+getxy(x+1,y-1)+getxy(x+1,y)+getxy(x+1,y+1);
}
void allzero(int x,int y){
	x&=15;
	y&=15;
	int z=x|y<<4;
	if(F[z]&2)return;
	F[z]|=2;
	for(int xx=x-1;xx<x+2;xx++)
		for(int yy=y-1;yy<y+2;yy++){
			int zz=(xx&15)|(yy&15)<<4;
			if(!getdot(xx,yy))
				allzero(xx,yy);
			else F[zz]|=2;
		}
}
int click(int x,int y){
	x&=15;
	y&=15;
	int q=x|y<<4;
	if(lose){
		F[q]=1;
		makefield();
		F[q]=0;
		lose=0;
		T=0;
	}else if(F[q]==1)
		lose=1;
	else if(!getdot(x,y))
		allzero(x,y);
	F[q]|=2;
}
int main(int argc,char**argv){
	#ifdef SDL
	if(SDL_Init(SDL_INIT_VIDEO)==-1){
		fputs(SDL_GetError(),stderr);
		return 1;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_Surface*dpy=SDL_SetVideoMode(256,256,0,SDL_OPENGL);
	#else
	Display*dpy=XOpenDisplay(0);
	XVisualInfo*vi=glXChooseVisual(dpy,DefaultScreen(dpy),(int[]){GLX_RGBA,GLX_DOUBLEBUFFER,None});
	Window Wdo=XCreateWindow(dpy,RootWindow(dpy,vi->screen),0,0,256,256,0,vi->depth,InputOutput,vi->visual,CWColormap|CWEventMask,(XSetWindowAttributes[]){{.colormap=XCreateColormap(dpy,RootWindow(dpy,vi->screen),vi->visual,AllocNone),.event_mask=PointerMotionMask|ButtonPressMask|ButtonReleaseMask}});
	XSetWMProtocols(dpy,Wdo,(Atom[]){XInternAtom(dpy,"WM_DELETE_WINDOW",False)},1);
	XMapWindow(dpy,Wdo);
	glXMakeCurrent(dpy,Wdo,glXCreateContext(dpy,vi,0,GL_TRUE));
	#endif
	srand(time(0));
	glOrtho(0,256,256,0,1,-1);
	glColor3ub(255,255,255);
	makefield();
	for(;;){
		#ifdef GLX
		glXSwapBuffers(dpy,Wdo);
		XEvent ev;
		while(XPending(dpy)){
			KeySym ks;
			XNextEvent(dpy,&ev);
		#else
		SDL_GL_SwapBuffers();
		SDL_Event ev;
		while(SDL_PollEvent(&ev)){
			SDLKey ks;
		#endif
			switch(ev.type){
			case(ButtonPress)
				int q=EV(button.x)>>4|EV(button.y)&0xF0,x=q&15,y=q>>4;
				if(EV(button.button)==3){
					if(!(F[q]&2))
						F[q]^=4;
				}else if(EV(button.button)==2){
					printf("%d %d %d\n",getmark(x,y),getdot(x,y),F[q]);
					if(getmark(x,y)==getdot(x,y)&&F[q]==2){
						for(int xx=-1;xx<2;xx++)
							for(int yy=-1;yy<2;yy++)
								if(!(F[x+xx&15|(y+yy&15)<<4]&4)){
									click(x+xx,y+yy);
									if(lose)goto lost;
								}
					}
				}else if(!(F[q]&4)){
					click(x,y);
				}
			case(ClientMessage)return 0;
			}
		}
		lost:glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_LINES);
		for(int x=16;x<256;x+=16){
			glVertex2i(x,0);
			glVertex2i(x,256);
			glVertex2i(0,x);
			glVertex2i(256,x);
		}
		glEnd();
		int on=0;
		for(int z=0;z<256;z++){
			int x=z&15,y=z>>4;
			on+=F[z]&2;
			if(F[z]&4){
				glColor3ub(255,192,203);
				glRecti(x*16,y*16,x*16+16,y*16+16);
				glColor3ub(255,255,255);
			}else if(F[z]==2){
				int dots = getdot(x,y);
				if(!dots){
					glColor3ub(0,0,255);
					glRecti(x*16,y*16,x*16+16,y*16+16);
					glColor3ub(255,255,255);
				}else{
					glBegin(GL_POINTS);
					tfChar(x*16+5,y*16+5,dots);
					glEnd();
				}
			}else if(F[z]&1){
				if(!lose&&F[z]==1)continue;
				glColor3ub(255,F[z]==1?255:0,0);
				glRecti(x<<4,y<<4,x<<4|15,y<<4|15);
				glColor3ub(255,255,255);
			}
		}
		printf("%d\n",on);
		if(on==432){
			printf("Won %d",T*3/100);
			lose=1;
			memcpy(F,win,256);
		}
		T++;
		#ifdef GLX
		usleep(60000-en/5);
		#else
		SDL_Delay(30);
		#endif
	}
}