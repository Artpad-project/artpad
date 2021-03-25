OUT = artpad $(wildcard *.out)
CFLAGS = -std=c99 -Wall -Wextra
CC = gcc
CPPFLAGS =

# LIBS
LIBS =
GTKLIBS = $(shell pkg-config --libs gtk+-3.0)
GTKFLAGS = $(shell pkg-config --cflags gtk+-3.0)

IMG_OBJ := 			image/image.o
GUI_OBJ := 			$(IMG_OBJ)
PUISSANCE_N_OBJ := 	puissancen/PuissanceN.o \
					puissancen/Retinex.c \
					$(IMG_OBJ)
MAGIC_WAND_OBJ 	:= 	$(IMG_OBJ)

OBJ = 				$(IMG_OBJ) \
					$(GUI_OBJ) \
					$(PUISSANCE_N_OBJ) \
					$(MAGIC_WAND_OBJ) \
					$(wildcard */main.o) \
					gui/gui.o

all: gui
build: gui
artpad: gui
image: $(IMG_OBJ) image/main.o
	$(CC) $(GTKLIBS) $^ -o image.out
gui: $(GUI_OBJ) gui/gui.o
	$(CC) $(GTKLIBS) $^ -o gui.out
magic_wand: $(GUI_OBJ) rotoscopie/rotoscopie.o
	$(CC) $(GTKLIBS) $^ -o magic_wand.out

%.o: %.c
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(GTKFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUT)

.PHONY: clean all
