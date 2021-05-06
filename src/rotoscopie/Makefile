OUT			= test video

CC			= gcc #-fsanitize=address
CFLAGS  	= -g -Wall -Wextra `pkg-config --cflags gtk+-3.0 gmodule-2.0` -Wno-unknown-pragmas
CPPFLAGS	= -MMD
LDLIBS		= `pkg-config --libs gtk+-3.0 gmodule-2.0` -pthread

SOURCE		= ../image/image.c ../image/mask.c rotoscopie.c ../utils/utils.c ../colorimetrie/stack.c ./rotoscopie_video.c
OBJ			= $(SOURCE:.c=.o)
DEP			= $(SOURCE:.c=.d)

all: $(OUT)

test: $(OBJ) main.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)
video: $(OBJ) ../video/video.o main_video.o
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)


.PHONY: clean

clean:
	rm -rf $(OUT) $(DEP) $(OBJ) "output.png" *.o *.d

-include $(DEP) 
