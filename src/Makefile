OUT			= artpad

CC			= gcc
CFLAGS  	= -g -Wall -Wextra
CPPFLAGS	= -MMD
LDLIBS		=

SOURCE		= main.c
OBJ			= $(SOURCE:.c=.o)
DEP			= $(SOURCE:.c=.d)

all: $(OUT)

build: artpad

artpad: $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)

.PHONY: clean

clean:
	rm -rf $(OUT) $(DEP) $(OBJ)

-include $(DEP) 
