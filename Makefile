SRC_DIR := src
BIN_DIR := bin
TEST_DIR := test
OUT := artpad

all: $(OUT)

.PHONY: build $(OUT) test%
build:
	@echo "Compiling executable into $(BIN_DIR)..."
	@$(MAKE) -C $(SRC_DIR) build BIN_DIR=../$(BIN_DIR)
	@echo "Built sucessfuly !"

$(OUT):
	@$(MAKE) -C $(SRC_DIR) BIN_DIR=../$(BIN_DIR)
	@echo "Compiled sucessfuly !"

test%:
	$(MAKE) -C $(TEST_DIR) $@ BIN_DIR=../$(BIN_DIR)

.PHONY: clean clean_tests cleanall
clean:
	@$(MAKE) -C $(SRC_DIR) clean BIN_DIR=../$(BIN_DIR)

clean_tests:
	@$(MAKE) -C $(TEST_DIR) clean

clean_all: clean clean_tests

help:
	@$(MAKE) -C $(SRC_DIR) help -s
	@$(MAKE) -C $(TEST_DIR) help -s
