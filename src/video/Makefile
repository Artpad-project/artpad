OUT			= video

CC			= gcc -fsanitize=address
CFLAGS  	= -g -Wall -Wextra -fno-stack-protector `pkg-config --cflags gtk+-3.0 gmodule-2.0`
CPPFLAGS	= -MMD
LDLIBS		= `pkg-config --libs gtk+-3.0 gmodule-2.0`

SOURCE		= ../image/image.c video.c

OBJ			= $(SOURCE:.c=.o)
DEP			= $(SOURCE:.c=.d)

all: $(OUT)

$(OUT): $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
	rm -rf $(OUT) $(DEP) $(OBJ) "output.png" frames

-include $(DEP) 
