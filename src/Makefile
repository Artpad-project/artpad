MAKE = make --no-print-directory
OUT = artpad
BIN_DIR = ../bin

-include variables.mk

ifdef DEBUG
	CFLAGS += #-DNDEBUG
endif

OBJS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SRC))

######## PUBLIC RULES #########

all: $(OUT)
$(OUT): build #$(BIN_DIR)/main.o
	$(CC) $(OBJS) -o ../$@ $(LDLIB)

build: build_libs build_dir $(OBJS)

clean:
	@$(MAKE) -C ../lib clean
	rm -rf ../$(OUT) $(BIN_DIR)

help:
	@echo "List of rules:"
	@echo "-- all : default rule ($(OUT))"
	@echo "-- $(OUT) : build the project and create an executable"
	@echo "-- build : build all source files into $(BIN_DIR)"
	@echo "-- clean : remove executable and $(BIN_DIR)"
	@echo "Available ptions:"
	@echo "-- DEBUG"

.PHONY: clean all build

######## PRIVATE RULES ########

build_dir:
	@[ -d $(BIN_DIR) ] || mkdir $(BIN_DIR)	

$(BIN_DIR)/%.o: %.c
	@echo "-- $< -> $@"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(GTKFLAGS) -c $< -o $@

build_libs:
	@$(MAKE) -C ../lib
