### SOURCE FILES ###

IMG_SRC := 			image/image.c \
					image/image_scaling.c \
					image/mask.c \
					balanceauto/BalanceAuto.c \
					contrastsimple/ContrastSimple.c \
					saturation/Saturation.c \
					flip/Flip.c \
					puissancen/PuissanceN.c \
					rotation/Rotation.c \
					utils/utils.c \
					utils/queue.c \
					rotoscopie/rotoscopie.c 
					


DRAWING_SRC :=		colorimetrie/stack.c \
					colorimetrie/flood_fill_stack.c\
					colorimetrie/colorimetrie.c

GUI_SRC :=			gui/gui.c \
					gui/temp_layers.c \
					gui/Layers.c \
					gui/image_tools.c

VIDEO_SRC :=		video/video.c	\
					video/smooth.c \
					rotoscopie/rotoscopie_video.c \

SRC :=	$(IMG_SRC) \
		$(GUI_SRC) \
		$(DRAWING_SRC) \
		$(VIDEO_SRC) 

### LIBRARIES #########

L_LIBALGO = ../lib/libalgo-c/build/liblibalgo

### COMPILER FLAGS #########

## GTK
GTKLIBS = $(shell pkg-config --libs gtk+-3.0)
GTKFLAGS = $(shell pkg-config --cflags gtk+-3.0)

## FLAGS
CC = gcc -g -fsanitize=address
CPPFLAGS =
CFLAGS_IGNORE = -Wno-unused-parameter -Wno-unknown-pragmas -Wno-format
CFLAGS = -Wall -Wextra $(CFLAGS_IGNORE) $(GTKFLAGS)
LDLIB =  $(GTKLIBS) -I$(L_LIBALGO) -ldl -lm

