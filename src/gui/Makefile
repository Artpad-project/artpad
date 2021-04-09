OUT           = gui

CC            = gcc -fsanitize=address -lm -ldl

CFLAGS        = -g -Wall -Wextra $(shell pkg-config --cflags gtk+-3.0)
CPPFLAGS      = -MMD
LDLIBS        = $(shell pkg-config --libs gtk+-3.0)

GUI_SRC       = gui.c basics_gui.c drawing_tools.c image_tools.c
IMG_SRC       = ../image/image.c ../image/image_scaling.c
IMG_SRC       += ../puissancen/PuissanceN.c ../saturation/Saturation.c ../rotation/Rotation.c
VIDEO_SRC     =
DRAWING_SRC   =

SOURCE        = $(GUI_SRC) $(IMG_SRC) $(DRAWING_SRC) $(VIDEO_SRC)
OBJ           = $(SOURCE:.c=.o)
DEP           = $(SOURCE:.c=.d)

all: $(OUT)

gui: $(OBJ)
    $(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
    rm -rf $(OUT) $(DEP) $(OBJ)

-include $(DEP)
