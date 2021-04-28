BIN_DIR := ../bin
TEST_BIN_DIR := bin

TESTS_SRC := $(shell ls src/*.c)
TESTS_OBJ := $(patsubst src/%.c,$(TEST_BIN_DIR)/%.o,$(TESTS_SRC))
TESTS := $(patsubst $(TEST_BIN_DIR)/%.o,test_%,$(TESTS_OBJ))

GTKLIB = $(shell pkg-config --libs gtk+-3.0)
LDLIB = -ld -lm $(GTKLIBS)

OBJS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SRC))

all: tests
tests: $(TESTS)

test_%: $(TEST_BIN_DIR)/%.o build
	@$(CC) $(CFLAGS) $(LDLIB) $(OBJS) $< -o $@

build:
	$(MAKE) -C ../src BIN_DIR=$(BIN_DIR) build

$(TEST_BIN_DIR)/%.o: src/%.c
	@echo "-- Compiling test: $< -> $@"
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(GTKFLAGS) -c $< -o $@

clean:
	@rm -rf $(TESTS) $(TEST_BIN_DIR)

help:
	@echo "Available tests:"
	@for test in $(TESTS);do \
		echo "-- $${test}"; \
	done

include ../src/variables.mk

