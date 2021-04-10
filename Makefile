BUILD_DIR := build
TEST_DIR := test

all: artpad

.PHONY: build make_all artpad
build:
	@[ -f `which cmake` ] || { echo "Install cmake first"; exit 1; }
	@echo "Compiling executable into $(BUILD_DIR)..."
	@[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	@cd $(BUILD_DIR); cmake ..
	@cd ..
	@echo "Build sucessfull !"

make_all: build
	@cd $(BUILD_DIR) && $(MAKE)
	@cd ..
	@echo "Compiled sucessfuly !"

artpad: build
	@cd $(BUILD_DIR) && $(MAKE) artpad
	@cd ..
	@echo "Compiled sucessfuly !"

TEST_RULES := build_tests test tests
TEST :=
.PHONY: $(TEST_RULES)
$(TEST_RULES):
	$(MAKE) -C $(TEST_DIR) $@ BUILD_DIR=$(BUILD_DIR) TEST=$(TEST)

.PHONY: clean clean_tests check_cleanall cleanall
clean:
	@cd $(BUILD_DIR) && $(MAKE) --silent clean
	@cd ..
	@echo "Cleaned all targets inside of '$(BUILD_DIR)'."

clean_tests:
	@$(MAKE) -C $(TEST_DIR) clean BUILD_DIR=$(BUILD_DIR)

check_cleanall:
	@echo -n "Do you really want to delete '$(BUILD_DIR)' directory? [y/N] "
	@read ans && [ $${ans:-N} == y ] || exit 1

cleanall: clean clean_tests check_cleanall
	@rm -rf $(BUILD_DIR)
	@echo "$(BUILD_DIR) has been completely cleared."
	@$(MAKE) -C $(TEST_DIR) cleanall BUILD_DIR=$(BUILD_DIR) --silent
