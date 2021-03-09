OUT			  = artpad

CC			  = gcc -fsanitize=address
CFLAGS  	= -g -Wall -Wextra $(shell pkg-config --cflags gtk+-3.0)
CPPFLAGS	= -MMD
LDLIBS		= $(shell pkg-config --libs gtk+-3.0)

SOURCE		= main.c
OBJ			  = $(SOURCE:.c=.o)
DEP			  = $(SOURCE:.c=.d)

all: $(OUT)

build: artpad

artpad: $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
	rm -rf $(OUT) $(DEP) $(OBJ)

-include $(DEP) 
