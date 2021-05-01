all:
	$(MAKE) -C src/
artpad:
	$(MAKE) -C src/ artpad
image:
	$(MAKE) -C src/ image
gui:
	$(MAKE) -C src/ gui
magic_wand:
	$(MAKE) -C src/ magic_wand
clean:
	$(MAKE) -C src/ clean

.POHNY: clean all
