# Makefile

CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags sdl` -Wall -O3
LDFLAGS =
LDLIBS = `pkg-config --libs gtk+-3.0` `pkg-config --libs sdl` -lm -lSDL_image

OBJ =  gui.o
DEP = ${OBJ:.o=.d}

all: gui

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} main

-include ${DEP}

# END
