wf:wf.c
	gcc -std=gnu99 -Os -march=native -s -ffast-math -fwhole-program -o wrapfield wf.c -lglfw -lGL