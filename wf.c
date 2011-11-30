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
#define XOOOO 3
#define OOOOX 2
#define XOOOX 1
#define XXXXX 0
#define A(a,b,c,d,e) a|b<<2|c<<4|d<<6,e
#define B(a,b,c,d,e) |a<<2|b<<4|c<<6,d|e<<2
#define C(a,b,c,d,e) |a<<4|b<<6,c|d<<2|e<<4
#define D(a,b,c,d,e) |a<<6,b|c<<2|d<<4|e<<6,
int T;
const uint8_t abc[]={
	A(
	OOOOX,
	OOOOX,
	OOOOX,
	OOOOX,
	OOOOX)
	B(
	XXXXX,
	OOOOX,
	XXXXX,
	XOOOO,
	XXXXX)
	C(
	XXXXX,
	OOOOX,
	XXXXX,
	OOOOX,
	XXXXX)
	D(
	XOOOX,
	XOOOX,
	XXXXX,
	OOOOX,
	OOOOX)
	A(
	XXXXX,
	XOOOO,
	XXXXX,
	OOOOX,
	XXXXX)
	B(
	XOOOO,
	XOOOO,
	XXXXX,
	XOOOX,
	XXXXX)
	C(
	XXXXX,
	OOOOX,
	OOOOX,
	OOOOX,
	OOOOX)
	D(
	XXXXX,
	XOOOX,
	XXXXX,
	XOOOX,
	XXXXX)
	A(
	XXXXX,
	XOOOX,
	XXXXX,
	OOOOX,
	OOOOX)
};
#define case(x) break;case x:;
char F[256];
int lose=1;
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
	int z=x|y<<4;
	if(F[z]&2)return;
	F[z]|=2;
	for(int xx=-1;xx<2;xx++)
		for(int yy=-1;yy<2;yy++){
			int X=x+xx&15,Y=y+yy&15;
			if(!getdot(X,Y))
				allzero(X,Y);
			else F[X|Y<<4]|=2;
		}
}
int click(int x,int y){
	int q=x|y<<4;
	if(lose){
		F[q]=1;
		makefield();
		F[q]=0;
		lose=0;
		T=0;
		if(!getdot(x,y))
			allzero(x,y);
	}else if(F[q]==1){
		printf("Lost %d\n",T*3/100);
		lose=1;
	}else if(!getdot(x,y))
		allzero(x,y);
	F[q]|=2;
}
const unsigned char col[][3]={{255,255,255},{255,255,0},{0,0,255},{255,0,0},{255,192,200}};
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
					if(getmark(x,y)==getdot(x,y)&&F[q]==2){
						for(int xx=-1;xx<2;xx++)
							for(int yy=-1;yy<2;yy++){
								int X=x+xx&15,Y=y+yy&15;
								if(!(F[X|Y<<4]&4)){
									click(X,Y);
									if(lose)goto lost;
								}
							}
					}
				}else if(!(F[q]&4)){
					click(x,y);
				}
			case(ClientMessage)return 0;
			}
		}
		lost:glClear(GL_COLOR_BUFFER_BIT);
		glColor3ubv(col[0]);
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
				glColor3ubv(col[4]);
				glRecti(x*16,y*16,x*16+16,y*16+16);
			}else if(F[z]==2){
				int dots = getdot(x,y);
				if(!dots){
					glColor3ubv(col[2]);
					glRecti(x*16,y*16,x*16+16,y*16+16);
				}else{
					glColor3ubv(col[0]);
					glBegin(GL_POINTS);
					for(int j=0;j<5;j++)
						for(int i=0;i<5;i++){
							int bit=dots*10+j*2-10;
							if((49727>>5*((abc[bit>>3]>>(bit&7))&3))&1<<i)glVertex2i(x*16+i+5,y*16+j+5);
						}
					glEnd();
				}
			}else if(F[z]&1){
				if(!lose&&F[z]==1)continue;
				glColor3ubv(col[F[z]]);
				glRecti(x*16,y*16,x*16+16,y*16+16);
			}
		}
		if(on==432){
			printf("Won %d\n",T*3/100);
			lose=1;
			memset(F,2,256);
			memset(F+70,3,2);
			memset(F+86,3,2);
			memset(F+73,3,2);
			memset(F+89,3,2);
			int j=101;
			for(int i=0;i<5;i++){
				F[j+=i<3?17:-15]=3;
			}
		}
		T++;
		#ifdef GLX
		usleep(60000-en/5);
		#else
		SDL_Delay(30);
		#endif
	}
}