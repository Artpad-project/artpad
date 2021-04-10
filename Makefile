BUILD_DIR := build
TEST_DIR := test

all: build

.PHONY: build
build:
	@[ -f `which cmake` ] || { echo "Install cmake first"; exit 1; }
	@echo "Compiling executable into $(BUILD_DIR)..."
	@[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	@cd $(BUILD_DIR); cmake ..; make
	@cd ..
	@echo "Compiled sucessfuly !"

.PHONY: build_tests test
test: build_tests
build_tests:
	$(MAKE) -C $(TEST_DIR) build_tests BUILD_DIR=$(BUILD_DIR)

.PHONY: clean clean_tests check_cleanall cleanall
.SILENT: clean clean_tests
clean:
	@[ -d $(BUILD_DIR) ] || { echo "'$(BUILD_DIR)': Nothing to clean."; exit 1; }
	@cd $(BUILD_DIR) && $(MAKE) clean -s
	@cd ..
	@echo "Cleaned all targets inside of '$(BUILD_DIR)'."

clean_tests:
	$(MAKE) -C $(TEST_DIR) clean BUILD_DIR=$(BUILD_DIR)

check_cleanall:
	@echo -n "Do you really want to delete '$(BUILD_DIR)' directory? [y/N] "
	@read ans && [ $${ans:-N} == y ] || exit 1

cleanall: clean clean_tests check_cleanall
	@rm -rv $(BUILD_DIR)
	@echo "$(BUILD_DIR) has been completely cleared."
	$(MAKE) -C $(TEST_DIR) cleanall BUILD_DIR=$(BUILD_DIR)
