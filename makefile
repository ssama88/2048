mainmake: main.c
	gcc main.c `sdl2-config --cflags --libs` -lSDL2_ttf color_map.c -o 2048