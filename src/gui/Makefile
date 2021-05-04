OUT           = gui

CC            = gcc -fsanitize=address -lm -ldl

CFLAGS        = -g -Wall -Wextra -Wno-unused-parameter $(shell pkg-config --cflags gtk+-3.0)
CPPFLAGS      = -MMD
LDLIBS        = $(shell pkg-config --libs gtk+-3.0)

SOURCE        = gui.c ../image/image.c ../image/image_scaling.c ../puissancen/PuissanceN.c ../saturation/Saturation.c ../colorimetrie/stack.c ../colorimetrie/colorimetrie.c ../balanceauto/BalanceAuto.c ../contrastsimple/ContrastSimple.c ../flip/Flip.c
OBJ           = $(SOURCE:.c=.o)
DEP           = $(SOURCE:.c=.d)

all: $(OUT)

gui: $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
	rm -rf $(OUT) $(DEP) $(OBJ)

-include $(DEP)

