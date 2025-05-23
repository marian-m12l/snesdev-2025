/*---------------------------------------------------------------------------------


    Simple window effect in mode 1 
    -- alekmaul


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <string.h>

extern char patternsbg1, patternsbg1_end;
extern char patternsbg2, patternsbg2_end;
extern char palettebg1, palettebg1_end;
extern char palettebg2, palettebg2_end;
extern char mapbg1, mapbg1_end;
extern char mapbg2, mapbg2_end;

extern char gfxpsrite, gfxpsrite_end;
extern char palsprite, palsprite_end;

u8 pada,padb,padx;
u16 pad0,pad1;

u8 p1_pos_x = 50;
u8 p1_pos_y = 50;
u8 p2_pos_x = 150;
u8 p2_pos_y = 150;

// TODO Compute separation line from position of the two players
// TODO Need 2 windows ? one for BG 1 and one for BG2 ? With a black separation line ?
// TODO Draw player sprites
// TODO Position backgrounds ?

// TODO Single HDMA table for both windows's left/right positions
// TODO HDMA Mode 4 : 4 bytes transfered at once on 4 consecutive registers ($2126-$2129)
u8 window_positions_table[]=
{
	// Lines count, Window 1 Left, Window 1 Right, Window 2 Left, Window 2 Right
	1, 0xff, 0x0, 0xff, 0x0,	// Disabled at first

	0x80 | 64,					// 64 lines

	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,

	0x80 | 64,					// 64 lines

	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,
	0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0, 0xff, 0x0,

	// TODO Add more lines
	
	1, 0xff, 0x0, 0xff, 0x0,
	0
};


/*! \fn  setModeHdmaWindow(u8 bgrnd, u8* hdmatableL,u8* hdmatableR)
    \brief Do a window  effect on screen. Use HDMA Channels 4 & 5.
    \param bgrnd background for the windows effect  (value MSWIN_BG1..MSWIN_BG4)
    \param bgrndmask background mask (inside, outside) for the windows effect  (value MSWIN1_BG13MSKIN..MSWIN1_BG13MSKIN)
    \param hdmatableL table with windows effect on the left/right for both windows
*/
void setModeHdmaWindow1And2SingleHdma(u8 bgrnd, u8 bgrndmask,u8* hdmatable);

//---------------------------------------------------------------------------------
int main(void)
{
    // Copy tiles to VRAM
    bgInitTileSet(0, &patternsbg1, &palettebg1, 0, (&patternsbg1_end - &patternsbg1), 16*1*2, BG_16COLORS, 0x4000);
    bgInitTileSet(1, &patternsbg2, &palettebg2, 1, (&patternsbg2_end - &patternsbg2), 16*1*2, BG_16COLORS, 0x6000);

    // Copy Map to VRAM
    bgInitMapSet(0, &mapbg1, (&mapbg1_end - &mapbg1), SC_32x32, 0x0000);
	bgInitMapSet(1, &mapbg2, (&mapbg2_end - &mapbg2), SC_32x32, 0x1000);

    // Init Sprites gfx and palette with default size of 32x32
    oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end - &gfxpsrite), &palsprite, (&palsprite_end - &palsprite), 0, 0x2000, OBJ_SIZE32_L64);

    // Define sprites parameters
    oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0); // Put sprite in 100,100, with maximum priority 3 from tile entry 0, palette 0
    oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
    oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0); // Put sprite in 100,100, with maximum priority 3 from tile entry 0, palette 0
    oamSetEx(4, OBJ_SMALL, OBJ_SHOW);

    // Now Put in 16 color mode and disable other BGs except 1st and 2nd one
    setMode(BG_MODE1, 0);
    bgSetEnable(1);	// Enable BG2
    bgSetDisable(2);	// Disable BG3
    //setScreenOn();

	// Compute window position
	int y;
	u8 offset = 0;
	for (y=0; y<64; y++) {
		*(window_positions_table+6+y*4) = offset + y;				// Window 1 Left
		*(window_positions_table+6+y*4+1) = offset + y + 20;		// Window 1 Right
		*(window_positions_table+6+y*4+2) = offset + 100 + y;		// Window 2 Left
		*(window_positions_table+6+y*4+3) = offset + 100 + y + 20;	// Window 2 Right
	}


	// FIXME PR to PVSNESLib ?? wrong shift
	#define FIXED_MSWIN2_BG2MSKOUT        (1 << 6) /*!< \brief Window 2 area BG2 inside (0) outside(1) */
	#define FIXED_MSWIN2_BG2MSKENABLE     (2 << 6) /*!< \brief Window 2 area BG2 enable */

	WaitForVBlank();
	// TODO Separate bg/window setup and hdma?
	setModeHdmaWindow1And2SingleHdma(0x03, 0xc3, (u8 *) &window_positions_table);

	setScreenOn();

	consoleNocashMessage("Go\n");

	// Wait for nothing :P
    while (1)
    {
		bool playersMoved = false;

        // Get current #0 pad
        pad0 = padsCurrent(0);

		// Handle player 1 movements
		if (pad0 & KEY_DOWN) {
			p1_pos_y += 10;
			if (p1_pos_y > 240) {
				p1_pos_y = 0;
			}
			oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad0 & KEY_UP) {
			p1_pos_y -= 10;
			if (p1_pos_y > 240) {
				p1_pos_y = 240;
			}
			oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad0 & KEY_RIGHT) {
			p1_pos_x += 10;
			oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad0 & KEY_LEFT) {
			p1_pos_x -= 10;
			oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}

        // Get current #1 pad
        pad1 = padsCurrent(1);

		// Handle player 2 movements
		if (pad1 & KEY_DOWN) {
			p2_pos_y += 10;
			if (p2_pos_y > 240) {
				p2_pos_y = 0;
			}
			oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad1 & KEY_UP) {
			p2_pos_y -= 10;
			if (p2_pos_y > 240) {
				p2_pos_y = 240;
			}
			oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad1 & KEY_RIGHT) {
			p2_pos_x += 10;
			oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad1 & KEY_LEFT) {
			p2_pos_x -= 10;
			oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}


		// Wait vblank sync
        WaitForVBlank();

		if (playersMoved) {
			// TODO Update split screen

			// TODO Simple diagonals depending on P1 quadrant
			if (p1_pos_x < 128 && p1_pos_y < 120) {
				// Top-Left quadrant
				// Window 1: L=0; R=256->0
				// Window 2: L=256->0; R=256
				// Variable slope
				u16 slope = 512 - p1_pos_x*4;
				u16 split_x = 255 << 8;
				//u8 split = 255;
				for (y=0; y<64; y++) {
					u8 split = split_x >> 8;
					*(window_positions_table+6+y*4) = 0;			// Window 1 Left
					*(window_positions_table+6+y*4+1) = split;		// Window 1 Right
					*(window_positions_table+6+y*4+2) = split;		// Window 2 Left
					*(window_positions_table+6+y*4+3) = 255;		// Window 2 Right
					//split--;
					split_x -= slope;
				}
			} else if (p1_pos_x >= 128 && p1_pos_y < 120) {
				// Top-Right quadrant
				// Window 1: L=0->256; R=256
				// Window 2: L=0; R=0->256
				// Variable slope
				u16 slope = (p1_pos_x-128)*4;
				u16 split_x = 0;
				//u8 split = 0;
				for (y=0; y<64; y++) {
					u8 split = split_x >> 8;
					*(window_positions_table+6+y*4) = split;		// Window 1 Left
					*(window_positions_table+6+y*4+1) = 255;		// Window 1 Right
					*(window_positions_table+6+y*4+2) = 0;			// Window 2 Left
					*(window_positions_table+6+y*4+3) = split;		// Window 2 Right
					//split++;
					split_x += slope;
				}
			} else if (p1_pos_x < 128 && p1_pos_y >= 120) {
				// Bottom-Left quadrant
				// Window 1: L=0; R=0->256
				// Window 2: L=0->256; R=256
				// Variable slope
				u16 slope = p1_pos_x*4;
				u16 split_x = 0;
				//u8 split = 0;
				for (y=0; y<64; y++) {
					u8 split = split_x >> 8;
					*(window_positions_table+6+y*4) = 0;			// Window 1 Left
					*(window_positions_table+6+y*4+1) = split;		// Window 1 Right
					*(window_positions_table+6+y*4+2) = split;		// Window 2 Left
					*(window_positions_table+6+y*4+3) = 255;		// Window 2 Right
					//split++;
					split_x += slope;
				}
			} else if (p1_pos_x >= 128 && p1_pos_y >= 120) {
				// Bottom-Right quadrant
				// Window 1: L=256->0; R=256
				// Window 2: L=0; R=256->0
				// Variable slope
				u16 slope = 512 - (p1_pos_x-128)*4;
				u16 split_x = 255 << 8;
				//u8 split = 255;
				for (y=0; y<64; y++) {
					u8 split = split_x >> 8;
					*(window_positions_table+6+y*4) = split;		// Window 1 Left
					*(window_positions_table+6+y*4+1) = 255;		// Window 1 Right
					*(window_positions_table+6+y*4+2) = 0;			// Window 2 Left
					*(window_positions_table+6+y*4+3) = split;		// Window 2 Right
					//split--;
					split_x -= slope;
				}
			}
		}

		// TODO Compute players distance + scroll backgrounds + voronoi split ?

		// Update hdma table
		// TODO During VBlank or use double buffer?
		/*u8 offset = (offset + 1) % 60;
		for (y=0; y<64; y++) {
			*(window_positions_table+6+y*4) = offset + y;				// Window 1 Left
			*(window_positions_table+6+y*4+1) = offset + y + 40;		// Window 1 Right
			*(window_positions_table+6+y*4+2) = offset + 100 + y;		// Window 2 Left
			*(window_positions_table+6+y*4+3) = offset + 100 + y + 40;	// Window 2 Right
		}
		for (y=64; y<128; y++) {
			*(window_positions_table+7+y*4) = offset + 128 - y;				// Window 1 Left
			*(window_positions_table+7+y*4+1) = offset + 128 - y + 40;		// Window 1 Right
			*(window_positions_table+7+y*4+2) = offset + 100 + 128 - y;		// Window 2 Left
			*(window_positions_table+7+y*4+3) = offset + 100 + 128 - y + 40;	// Window 2 Right
		}*/
    }
    return 0;
}