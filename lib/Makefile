LIBALGO_DIR = libalgo-c/build
BUILD_DIRS = $(LIBALGO_DIR)

all: build_lib
build_lib: update_submodules libalgo

update_submodules:
	@git submodule update --init --recursive --quiet

libalgo:
	@[ -d $(LIBALGO_DIR) ] || mkdir $(LIBALGO_DIR)
	@cd $(LIBALGO_DIR) && cmake .. && make

clean:
	rm -rf $(BUILD_DIRS)
