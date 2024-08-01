mainmake: main.c
	gcc main.c `sdl2-config --cflags --libs` -o 2048