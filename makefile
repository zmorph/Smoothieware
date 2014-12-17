#!/usr/bin/make

DIRS = mbed src
DIRSCLEAN = $(addsuffix .clean,$(DIRS))
LANG = EN

pl:	LANG = PL
pl: all

en:	LANG = EN
en: all

all:
	@ $(MAKE) -C mbed
	@echo Building Smoothie
	@ $(MAKE) -C src LANG=$(LANG)

clean: $(DIRSCLEAN)

$(DIRSCLEAN): %.clean:
	@echo Cleaning $*
	@ $(MAKE) -C $*  clean

debug-store:
	@ $(MAKE) -C src debug-store

flash:
	@ $(MAKE) -C src flash

dfu:
	@ $(MAKE) -C src dfu

upload:
	@ $(MAKE) -C src upload

debug:
	@ $(MAKE) -C src debug

console:
	@ $(MAKE) -C src console

.FORCE:

.PHONY: all $(DIRS) $(DIRSCLEAN) debug-store flash upload debug console dfu .FORCE
