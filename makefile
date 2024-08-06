mainmake: main.c
	gcc main.c `sdl2-config --cflags --libs` -lSDL2_ttf -o 2048