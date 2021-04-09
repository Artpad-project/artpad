BUILD_DIR = build

all: build_tests

.PHONY: test
build_tests:
	@[ -f `which cmake` ] || { echo "Install cmake first"; exit 1; }
	@[ -d $(BUILD_DIR) ] || mkdir $(BUILD_DIR)
	@echo "Compiling executable into $(BUILD_DIR)..."
	@cd $(BUILD_DIR) && cmake ../.. && make
	@cd ../..
	@echo "Compiled sucessfuly !"

.PHONY: cleanall clean_tests clean
clean:
	@[ -d $(BUILD_DIR) ] || { echo "'$(BUILD_DIR)': Nothing to clean."; exit 1; }
	@cd $(BUILD_DIR) && make clean
	@cd ..
	@echo "Cleaned all targets inside of '$(BUILD_DIR)'."

check_cleanall:
	@echo -n "Do you really want to delete '$(BUILD_DIR)' directory? [y/N] "
	@read ans && [ $${ans:-N} == y ] || exit 1

cleanall: clean clean_tests check_cleanall
	@rm -rv $(BUILD_DIR)
	@echo "$(BUILD_DIR) has been completely cleared."
