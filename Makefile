files=src/main.c src/engine/global.c src/engine/entities/entity.c src/engine/entities/david.c src/engine/entities/post.c

libs=-lm `sdl2-config --cflags --libs` -lSDL2_mixer -lSDL2_image


build:
	gcc -g3 $(files) $(libs) -o game