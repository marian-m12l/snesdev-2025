ifeq ($(strip $(PVSNESLIB_HOME)),)
$(error "Please create an environment variable PVSNESLIB_HOME by following this guide: https://github.com/alekmaul/pvsneslib/wiki/Installation")
endif

include ${PVSNESLIB_HOME}/devkitsnes/snes_rules

.PHONY: bitmaps all

#---------------------------------------------------------------------------------
# ROMNAME is used in snes_rules file
export ROMNAME := voronoi

all: bitmaps $(ROMNAME).sfc

clean: cleanBuildRes cleanRom cleanGfx

map_512_512.pic: map_512_512.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -t bmp -e 0 -p -m -i $<

sprites.pic: sprites.bmp
	@echo convert bitmap ... $(notdir $@)
	$(GFXCONV) -s 32 -o 16 -u 16 -t bmp -i $<

bitmaps : map_512_512.pic sprites.pic

