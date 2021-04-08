OUT			= colorimetrie

CC			= gcc -fsanitize=address
CFLAGS  	= -g -Wall -Wextra  `pkg-config --cflags gtk+-3.0 gmodule-2.0`
CPPFLAGS	= -MMD
LDLIBS		= `pkg-config --libs gtk+-3.0 gmodule-2.0`

SOURCE		= main.c ../image/image.c stack.c colorimetrie.c
OBJ			= $(SOURCE:.c=.o)
DEP			= $(SOURCE:.c=.d)

all: $(OUT)

colorimetrie: $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
	rm -rf $(OUT) $(DEP) $(OBJ) "output.png"

-include $(DEP) 
