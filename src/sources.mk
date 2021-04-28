### SOURCE FILES ###

IMG_SRC := 			image/image.c \
					image/filter.c \
					image/image_scaling.c \
					image/mask.c \
					rotation/Rotation.c \
					saturation/Saturation.c \
					puissancen/PuissanceN.c \
					rotoscopie/rotoscopie.c

DRAWING_SRC :=		colorimetrie/stack.c \
					colorimetrie/colorimetrie.c

GUI_SRC :=			gui/basics_gui.c \
					gui/drawing_tools.c \
					gui/gui.c \
					gui/image_tools.c

VIDEO_SRC :=		

SRC :=	$(IMG_SRC) \
		$(GUI_SRC) \
		$(DRAWING_SRC) \
		$(VIDEO_SRC) \
		utils/utils.c

### COMPILER FLAGS #########

## GTK
GTKLIBS = $(shell pkg-config --libs gtk+-3.0)
GTKFLAGS = $(shell pkg-config --cflags gtk+-3.0)

## FLAGS
CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -pthread $(GTKFLAGS)
LDLIB = -ldl -lm $(GTKLIBS)

