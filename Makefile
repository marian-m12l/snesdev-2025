ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := Window

all: bitmaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx

pvsneslibbg1.pic: pvsneslibbg1.png
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -m -i $<

pvsneslibbg2.pic: pvsneslibbg2.png
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -e 1 -p -m -i $<

bitmaps : pvsneslibbg1.pic pvsneslibbg2.pic

