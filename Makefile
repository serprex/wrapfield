ifneq ($(GLX),)
CFLAGS=-DGLX
else
CFLAGS=-DSDL `sdl-config --cflags` `sdl-config --libs`
endif
wf:wf.c
	gcc -std=gnu99 -Os -march=native -s -ffast-math -fwhole-program -ftree-vectorize -o wrapfield wf.c ../tnyfnt/tf5x5.o ${CFLAGS} -lGL