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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define XXX 0
#define XOX 1
#define OOX 2
#define XOO 3
#define A(a,b,c,d,e) a|b<<2|c<<4|d<<6,e
#define B(a,b,c,d,e) |a<<2|b<<4|c<<6,d|e<<2
#define C(a,b,c,d,e) |a<<4|b<<6,c|d<<2|e<<4
#define D(a,b,c,d,e) |a<<6,b|c<<2|d<<4|e<<6,
const unsigned char col[]={0,255,0,255,255,255,0,0,255,96,162,255,192,200,128,0,128,128,0,0},win[]={70,71,86,87,73,74,89,90,118,122,135,137,152};
#define GRE 0
#define TUR GRE+2
#define WHT TUR+1
#define YEL WHT+1
#define RED YEL+1
#define BLU RED+1
#define PK1 BLU+2
#define PK2 PK1+3
#define PUR PK2+3
#define DUR PUR+1
#define DEL DUR+1
#define DED DEL+2
const unsigned char colid[]={WHT,GRE,RED,TUR,DED,DUR,DEL,PUR};
const uint8_t abc[]={
	A(
	XXX,
	XOX,
	XOX,
	XOX,
	XXX)
	B(
	OOX,
	OOX,
	OOX,
	OOX,
	OOX)
	C(
	XXX,
	OOX,
	XXX,
	XOO,
	XXX)
	D(
	XXX,
	OOX,
	XXX,
	OOX,
	XXX)
	A(
	XOX,
	XOX,
	XXX,
	OOX,
	OOX)
	B(
	XXX,
	XOO,
	XXX,
	OOX,
	XXX)
	C(
	XOO,
	XOO,
	XXX,
	XOX,
	XXX)
	D(
	XXX,
	OOX,
	OOX,
	OOX,
	OOX)
	A(
	XXX,
	XOX,
	XXX,
	XOX,
	XXX)
	B(
	XXX,
	XOX,
	XXX,
	OOX,
	OOX)
};
#define case(x) break;case x:;
char F[256];
int T,lose=1;
int getxy(int x,int y){
	return F[(x&15)+(y&15)*16]&1;
}
int getmar(int x,int y){
	return(F[(x&15)+(y&15)*16]&4)==4;
}
int getmark(int x,int y){
	return getmar(x-1,y-1)+getmar(x-1,y)+getmar(x-1,y+1)+getmar(x,y-1)+getmar(x,y+1)+getmar(x+1,y-1)+getmar(x+1,y)+getmar(x+1,y+1);
}
int getdot(int x,int y){
	return getxy(x-1,y-1)+getxy(x-1,y)+getxy(x-1,y+1)+getxy(x,y-1)+getxy(x,y+1)+getxy(x+1,y-1)+getxy(x+1,y)+getxy(x+1,y+1);
}
void tfChar(int x,int y,int c){
	glBegin(GL_POINTS);
	for(int j=0;j<5;j++)
		for(int i=0;i<5;i++){
			int bit=c*10+j*2;
			if((49727>>5*((abc[bit>>3]>>(bit&7))&3))&1<<i)glVertex2i(x+i,y+j);
		}
	glEnd();
}
void allzero(int x,int y){
	int z=x+y*16;
	if(F[z]&2)return;
	F[z]|=2;
	for(int xx=-1;xx<2;xx++)
		for(int yy=-1;yy<2;yy++){
			int X=x+xx&15,Y=y+yy&15;
			if(!getdot(X,Y))allzero(X,Y);
			else F[X+Y*16]|=2;
		}
}
void click(int x,int y){
	int q=x+y*16;
	if(lose){
		F[q]=1;
		do{
			int m=0;
			memset(F,0,256);
			for(;;){
				int x=rand()&255;
				if(!F[x]){
					F[x]=1;
					if(++m==40)break;
				}
			}
		}while(getdot(x,y));
		F[q]=0;
		lose=0;
		T=0;
		allzero(x,y);
	}else if(F[q]==1)lose=1;
	else if(!getdot(x,y))allzero(x,y);
	F[q]|=2;
}
int main(int argc,char**argv){
	#ifndef GLX
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface*dpy=SDL_SetVideoMode(256,256,0,SDL_OPENGL);
	#else
	Display*dpy=XOpenDisplay(0);
	XVisualInfo*vi=glXChooseVisual(dpy,DefaultScreen(dpy),(int[]){GLX_DOUBLEBUFFER,GLX_RGBA,None});
	Window Wdo=XCreateWindow(dpy,RootWindow(dpy,vi->screen),0,0,256,256,0,vi->depth,InputOutput,vi->visual,CWColormap|CWEventMask,(XSetWindowAttributes[]){{.colormap=XCreateColormap(dpy,RootWindow(dpy,vi->screen),vi->visual,AllocNone),.event_mask=ButtonPressMask}});
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
			XNextEvent(dpy,&ev);
		#else
		SDL_GL_SwapBuffers();
		SDL_Event ev;
		while(SDL_PollEvent(&ev)){
		#endif
			if(ev.type==ButtonPress){
				int q=EV(button.x)>>4|EV(button.y)&240,x=q&15,y=q>>4;
				switch(EV(button.button)){
				case(1)
					if(!(F[q]&4))click(x,y);
				case(2)
					if(F[q]==2&&getmark(x,y)==getdot(x,y))
						for(int xx=-1;xx<2;xx++)
							for(int yy=-1;yy<2;yy++){
								int X=x+xx&15,Y=y+yy&15;
								if(!(F[X+Y*16]&4)){
									click(X,Y);
									if(lose)goto lost;
								}
							}
				case(3)
					if(!(F[q]&2))F[q]^=4;
				}
			}
			#ifndef GLX
			else if(ev.type==ClientMessage)return 0;
			#endif
		}
		lost:glClear(GL_COLOR_BUFFER_BIT);
		glColor3ubv(col+WHT);
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
				glColor3ubv(col+(F[z]==4&&lose?PK1:PK2));
				glRecti(x*16,y*16,x*16+16,y*16+16);
			}else if(F[z]==2){
				int dots=getdot(x,y);
				if(!dots){
					glColor3ubv(col+BLU);
					glRecti(x*16,y*16,x*16+16,y*16+16);
				}else{
					glColor3ubv(col+colid[dots-1]);
					tfChar(x*16+5,y*16+5,dots);
				}
			}else if(F[z]&1){
				if(!lose&&F[z]==1)continue;
				glColor3ubv(col+(F[z]==1?YEL:RED));
				glRecti(x*16,y*16,x*16+16,y*16+16);
			}
		}
		if(on==432){
			lose=1;
			memset(F,2,256);
			for(int i=0;i<13;i++)F[win[i]]=3;
		}
		if(lose){
			int t=T,x=270;
			do tfChar(x-=7,40,t%10); while(t/=10);
		}else T+=3;
		#ifdef GLX
		usleep(30000);
		#else
		SDL_Delay(30);
		#endif
	}
}