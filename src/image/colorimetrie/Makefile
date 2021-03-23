OUT							= colorimetrie


CC							= gcc -fsanitize=address
CFLAGS					= -g -Wall
CPPFLAGS				= -MMD


SOURCE					= colorimetrie.c
OBJ							= $(SOURCE:.c=.o)
DEP							= $(SOURCE:.c=.o)

all: $(OUT)

build: colorimetrie

colorimetrie:	$(CC)	$(CPPFLAGS) -o $@ $^	$(LDLIBS)

.PHONY: clean

clean:
				rm -rf $(OUT) $(DEP) $(OBJ)

-include $(DEP)
