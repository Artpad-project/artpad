BUILD_DIR := build

TESTS_SRC := `cd src && ls *.c`
TESTS := $(patsubst %.c,test_%,$TESTS_SRC)
TEST := 

all: build_tests $(TEST)

tests: build_tests $(TESTS)
$(TESTS):
	@cd $(BUILD_DIR) && $(MAKE) $<
	@cd ..
	@echo "Succesfully compiled all tests!"
	@cp $(BUILD_DIR)/test/test_* .

test: $(TEST)
$(TEST):
	cd $(BUILD_DIR) && $(MAKE) $(TEST)
	@cd ..
	@echo "Succesfully compiled '$(TEST)'!"
	@cp $(BUILD_DIR)/test/$(TEST) .

build_tests:
	@[ -f `which cmake` ] || { echo "Install cmake first"; exit 1; }
	@[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	@[ -d out ] || mkdir out
	@echo "Compiling into $(BUILD_DIR)..."
	@cd $(BUILD_DIR) && cmake ../..
	@cd ../..
	@echo "Built sucessfuly !"

.PHONY: cleanall clean_tests clean
clean:
	@[ -d $(BUILD_DIR) ] || { echo "'$(BUILD_DIR)': Nothing to clean."; exit 1; }
	@cd $(BUILD_DIR) && $(MAKE) --silent clean
	@cd ..
	@echo "Cleaned all targets inside of '$(BUILD_DIR)'."
	@rm -rfv test_*

check_cleanall:
	@echo -n "Do you really want to delete '$(BUILD_DIR)' directory? [y/N] "
	@read ans && [ $${ans:-N} == y ] || exit 1

cleanall: clean clean_tests check_cleanall
	@rm -rf $(BUILD_DIR)
	@echo "$(BUILD_DIR) has been completely cleared."
	@rm -rfv out
