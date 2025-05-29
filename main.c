#include <snes.h>
#include <string.h>

#define SCREEN_WIDTH (256)
#define SCREEN_HALF_WIDTH (128)
#define SCREEN_HEIGHT (224)
#define SCREEN_HALF_HEIGHT (112)

extern const char patternsmap_512_512, patternsmap_512_512_end;
extern const char palettemap_512_512, palettemap_512_512_end;
extern const char map_512_512, map_512_512_end;

extern const char gfxpsrite, gfxpsrite_end;
extern const char palsprite, palsprite_end;

u8 pada,padb,padx;
u16 pad0,pad1;

u16 p1_pos_x = 64;
u16 p1_pos_y = 120;
u16 p2_pos_x = 192;
u16 p2_pos_y = 120;

extern const u16 camera_lut_lo[64][128];
extern const u16 camera_lut_hi[64][128];


// Single HDMA table for both windows's left/right positions
// HDMA Mode 4 : 4 bytes transfered at once on 4 consecutive registers ($2126-$2129)
u8 window_positions_table[]=
{
	// Lines count, Window 1 Left, Window 1 Right, Window 2 Left, Window 2 Right
	1, 0xff, 0x0, 0xff, 0x0,	// Disabled at first

	0x80 | 60,					// 60 lines

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

	0x80 | 60,					// 60 lines

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

	0x80 | 60,					// 60 lines

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

	0x80 | 44,					// 44 lines

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
	// Copy tiles to VRAM (shared tiles for both background maps)
	// VRAM address is 4K-aligned word-address (8KiB chunk)
	// Background tiles @ 0x2000
    bgInitTileSet(0, &patternsmap_512_512, &palettemap_512_512, 0, (&patternsmap_512_512_end - &patternsmap_512_512), 16 * 2, BG_16COLORS, 0x2000);
	bgSetGfxPtr(1, 0x2000);
	
	// Copy Map to VRAM
	// Background tilemap @ 0x1000
    bgInitMapSet(0, &map_512_512, (&map_512_512_end - &map_512_512), SC_64x64, 0x1000);
	bgSetMapPtr(1, 0x1000, SC_64x64);

    // Init Sprites gfx and palette with default size of 32x32
	// VRAM address is 8K-aligned word-address (16KiB chunk)
	// Sprite tiles @ 0x0000
    oamInitGfxSet(&gfxpsrite, (&gfxpsrite_end - &gfxpsrite), &palsprite, (&palsprite_end - &palsprite), 0, 0x0000, OBJ_SIZE32_L64);

    // Define sprites parameters
    oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0); // Put sprite in 100,100, with maximum priority 3 from tile entry 0, palette 0
    oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
    oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 4, 0); // Put sprite in 100,100, with maximum priority 3 from tile entry 0, palette 0
    oamSetEx(4, OBJ_SMALL, OBJ_SHOW);

    // Now Put in 16 color mode and disable other BGs except 1st and 2nd one
    setMode(BG_MODE1, 0);
    bgSetEnable(1);	// Enable BG2
    bgSetDisable(2);	// Disable BG3

	// TODO Scroll map relative to player position
	// TODO Player position difference based on scroll
	// TODO BG and player are moved depending on split screen position ?!

	// TODO pN_pos_* are absolute position in the whole 512x512 tilemap
	// TODO if position difference is below threshold, display only background 1, centered on the middle of both players
	// TODO if position difference is above threshold, display both backgrounds with split screen (each centered on its player). Player position on-screen based on the split screen slope ??

	oamSetXY(0, 64, 120);
	oamSetXY(4, 192, 120);

	bgSetScroll(0, p1_pos_x-64, p1_pos_y-120);
	bgSetScroll(1, p2_pos_x-64, p2_pos_y-120);


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
		// FIXME Player positions SHOULD NOT directly update OAM sprite positions !!
		// Update background scrolling instead !!
		if (pad0 & KEY_DOWN) {
			p1_pos_y += 10;
			if (p1_pos_y > 512) {
				p1_pos_y = 0;
			}
			//oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad0 & KEY_UP) {
			p1_pos_y -= 10;
			if (p1_pos_y > 512) {
				p1_pos_y = 512;
			}
			//oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad0 & KEY_RIGHT) {
			p1_pos_x += 10;
			if (p1_pos_x > 512) {
				p1_pos_x = 0;
			}
			//oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad0 & KEY_LEFT) {
			p1_pos_x -= 10;
			if (p1_pos_x > 512) {
				p1_pos_x = 512;
			}
			//oamSet(0, p1_pos_x, p1_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}

        // Get current #1 pad
        pad1 = padsCurrent(1);

		// Handle player 2 movements
		if (pad1 & KEY_DOWN) {
			p2_pos_y += 10;
			if (p2_pos_y > 512) {
				p2_pos_y = 0;
			}
			//oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad1 & KEY_UP) {
			p2_pos_y -= 10;
			if (p2_pos_y > 512) {
				p2_pos_y = 512;
			}
			//oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad1 & KEY_RIGHT) {
			p2_pos_x += 10;
			if (p2_pos_x > 512) {
				p2_pos_x = 0;
			}
			//oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}
		if (pad1 & KEY_LEFT) {
			p2_pos_x -= 10;
			if (p2_pos_x > 512) {
				p2_pos_x = 512;
			}
			//oamSet(4, p2_pos_x, p2_pos_y, 3, 0, 0, 0, 0);
			playersMoved = true;
		}


		// Wait vblank sync
        WaitForVBlank();

		if (playersMoved) {


			// Update BG scroll depending on players positions and split slope
			// TODO "camera" (BG scroll) for each player is positioned on the path between players, at an offset from the player's position (or in the middle when both players are close to each other)
			// TODO player sprite should also be moved by this offset so it appears in the correct window
			//bgSetScroll(0, p1_pos_x-64, p1_pos_y-120);
			//bgSetScroll(1, p2_pos_x-64, p2_pos_y-120);
			// TODO _compute_horizontal_length = vector length
			// TODO max_separation = max players distance on a common bg

			// TODO Alternative approach : find camera position by walking the slope perpendicular to split line until 'distance' pixels have been walked ?

			// FIXME Assembly routine to compute (hardware) multiplications and square root (with LUT)

			// TODO LUT to find vectore2d distance directly
			// FIXME LUT can be slashed in two if first dimension is always the longest segment


			// FIXME ABSOLUTE VALUE OF DX AND DY !!!
			// FIXME RE-APPLY SIGN TO diff_x_camera AND diff_y_camera !!

			s16 dy = (p2_pos_y - p1_pos_y);
			s16 dx = (p2_pos_x - p1_pos_x);

			u16 abs_dx = dx >= 0 ? dx : -(dx+1)+1;
			u16 abs_dy = dy >= 0 ? dy : -(dy+1)+1;

			u16 lut_w = abs_dx >> 2;
			u16 camera_diff = lut_w < 64 ? camera_lut_lo[lut_w][abs_dy>>2] : camera_lut_hi[lut_w-64][abs_dy>>2];
			if (camera_diff == 0) {
				// TODO single bg, no split screen
				//consoleNocashMessage("should show bg 1 only\n");

				// TODO Compute middle point + bg scroll + sprite positions
				u16 camera_x = (p1_pos_x + p2_pos_x) >> 1;
				u16 camera_y = (p1_pos_y + p2_pos_y) >> 1;

				u16 bg1_scroll_x = camera_x < SCREEN_HALF_WIDTH ? 0 : camera_x - SCREEN_HALF_WIDTH;
				u16 bg1_scroll_y = camera_y < SCREEN_HALF_HEIGHT ? 0 : camera_y - SCREEN_HALF_HEIGHT;
				u16 bg2_scroll_x = bg1_scroll_x;
				u16 bg2_scroll_y = bg1_scroll_y;

				// TODO sprite half-size ?
				u16 p1_sprite_x = p1_pos_x - bg1_scroll_x;
				u16 p1_sprite_y = p1_pos_y - bg1_scroll_y;
				u16 p2_sprite_x = p2_pos_x - bg2_scroll_x;
				u16 p2_sprite_y = p2_pos_y - bg2_scroll_y;

				// TODO Apply scroll and sprite positions
				bgSetScroll(0, bg1_scroll_x, bg1_scroll_y);
				bgSetScroll(1, bg2_scroll_x, bg2_scroll_y);

				// TODO Mirror sprites to look at each other ?
				oamSetXY(0, p1_sprite_x, p1_sprite_y);
				oamSetXY(4, p2_sprite_x, p2_sprite_y);

				// TODO Display only BG1 --> disable windows ?? update hdma table to always show bg1 ?

				u8* ptr = window_positions_table + 5;
				u8 y;
				for (y=0; y<SCREEN_HEIGHT; y++) {
					if ((y%60) == 0) {
						ptr++;
					}
					// Hide W2 altogether
					*(ptr++) = 0;			// Window 1 Left
					*(ptr++) = 255;		// Window 1 Right
					*(ptr++) = 255;		// Window 2 Left
					*(ptr++) = 0;			// Window 2 Right
				}
			} else {
				u16 diff_x_camera = (camera_diff >> 8) << 2;
				u16 diff_y_camera = (camera_diff & 0xff) << 2;

				//u16 length = vector2d_distance(dx, dy); //sqrt(dx*dx + dy*dy);
				//u16 distance = (length > max_separation) ? max_separation : length;
				// FIXME always "max separation" when displaying split screen ?? otherwise single BG is displayed ?
				// --> LUT could provide the vector directly ((dx * max_separation) / length and (dx * max_separation) / length) ?
				/*if (length <= max_separation) {
					// TODO Use single BG / no split screen
				}*/
				// TODO if (length <= max_separation) --> single window, centered camera --> TODO Check before computing slope and drawing split line...
				// TODO otherwise, the 2 backgrounds must be placed apart from the split line (at half distance, unless BG offset hits the tilemap border)
				// FIXME cannot normalize without floats --> multiply first ?
				//u16 diff_x_normalized = dx / length;	// FIXME unused?
				//u16 diff_y_normalized = dy / length;	// FIXME unused?
				//u16 diff_x_camera = (dx * distance) / length;	//diff_x_normalized * distance;
				//u16 diff_y_camera = (dy * distance) / length;	//diff_y_normalized * distance;
				s16 half_diff_x = dx < 0 ? -(diff_x_camera >> 1) : (diff_x_camera >> 1);
				s16 half_diff_y = dy < 0 ? -(diff_y_camera >> 1) : (diff_y_camera >> 1);
				// FIXME Adjust sprite position so that its CENTER is at the given position (depends on dx/dy sign)
				s16 p1_sprite_offset_x = dx < 0 ? 16 : -16;
				s16 p1_sprite_offset_y = dy < 0 ? 16 : -16;
				s16 p2_sprite_offset_x = dx < 0 ? -16 : 16;
				s16 p2_sprite_offset_y = dy < 0 ? -16 : 16;
				u16 bg1_scroll_x = p1_pos_x + half_diff_x + p1_sprite_offset_x;
				u16 bg1_scroll_y = p1_pos_y + half_diff_y + p1_sprite_offset_y;
				u16 bg2_scroll_x = p2_pos_x - half_diff_x + p2_sprite_offset_x;
				u16 bg2_scroll_y = p2_pos_y - half_diff_y + p2_sprite_offset_y;
				// TODO Clamp bg scroll !!
				// TODO bg_scroll = camera position - half-screen (scroll is top-left !!)
				bg1_scroll_x = bg1_scroll_x < SCREEN_HALF_WIDTH ? 0 : bg1_scroll_x - SCREEN_HALF_WIDTH;
				bg1_scroll_y = bg1_scroll_y < SCREEN_HALF_HEIGHT ? 0 : bg1_scroll_y - SCREEN_HALF_HEIGHT;
				bg2_scroll_x = bg2_scroll_x < SCREEN_HALF_WIDTH ? 0 : bg2_scroll_x - SCREEN_HALF_WIDTH;
				bg2_scroll_y = bg2_scroll_y < SCREEN_HALF_HEIGHT ? 0 : bg2_scroll_y - SCREEN_HALF_HEIGHT;
				// TODO sprite position: p1_sprite_x = ???
				u16 p1_sprite_x = p1_pos_x - bg1_scroll_x;
				u16 p1_sprite_y = p1_pos_y - bg1_scroll_y;
				u16 p2_sprite_x = p2_pos_x - bg2_scroll_x;
				u16 p2_sprite_y = p2_pos_y - bg2_scroll_y;
				//consoleNocashMessage("dx=%d dy=%d len=%d dist=%d dxn=%d dyn=%d dxc=%d dyc=%d scx1=%d scy1=%d scx2=%d scy2=%d\n", dx, dy, length, distance, diff_x_normalized, diff_y_normalized, diff_x_camera, diff_y_camera, bg1_scroll_x, bg1_scroll_y, bg2_scroll_x, bg2_scroll_y);
				//consoleNocashMessage("dx=%d dy=%d dc=0x%04x dxc=%d dyc=%d scx1=%d scy1=%d scx2=%d scy2=%d p1x=%d p1y=%d p2x=%d p2y=%d\n", dx, dy, camera_diff, diff_x_camera, diff_y_camera, bg1_scroll_x, bg1_scroll_y, bg2_scroll_x, bg2_scroll_y, p1_sprite_x, p1_sprite_y, p2_sprite_x, p2_sprite_y);

				// TODO Apply scroll and sprite positions
				bgSetScroll(0, bg1_scroll_x, bg1_scroll_y);
				bgSetScroll(1, bg2_scroll_x, bg2_scroll_y);

				// TODO Mirror sprites to look at each other ?
				oamSetXY(0, p1_sprite_x, p1_sprite_y);
				oamSetXY(4, p2_sprite_x, p2_sprite_y);

				// TODO Update split screen
			
				// TODO Compute slope from players position difference ?
				// slope = (255*(x2-x1)) / (y2-y1)
				// direction of slope (sign) : sign(x2-x1) * sign(y2-y1) ?

				// TODO Use hardware division !!!
				// TODO Use unsigned ??
				//consoleNocashMessage("Voronoi p1(%d,%d) p2(%d,%d)\n", p1_pos_x, p1_pos_y, p2_pos_x, p2_pos_y);
				// FIXME if dy == 0 --> slope is 0 and window is half-screen (top / right)
				s16 slope = - ((((dy << 5)) / dx) << 3);	// Works up to dy == 1023 ? TODO Could remove to bits to both dy and dx too ?
				//consoleNocashMessage("Voronoi dx=%d dy=%d slope = %d\n", dx, dy, slope);

				// TODO Hardware division !!!
				// TODO Handle slope == 0
				u16 abs_slope = slope < 0 ? -slope : slope;
				u16 lines = (255<<8)/abs_slope;
				//u16 columns = abs_slope > SCREEN_HEIGHT ? SCREEN_WIDTH : abs_slope;
				//u16 columns = (abs_slope * SCREEN_HEIGHT) >> 8;	// FIXME OVERFLOW
				//u16 columns = ((abs_slope>>1) * (SCREEN_HEIGHT>>1)) >> 6;	// FIXME sometimes wrong
				//columns = columns > 256 ? 256 : columns;
				u16 columns = abs_slope > 292 ? SCREEN_WIDTH : (abs_slope * SCREEN_HEIGHT) >> 8;
				//consoleNocashMessage("Voronoi split lines=%d columns=%d\n", lines, columns);
				u16 offset_x = columns < SCREEN_WIDTH ? SCREEN_HALF_WIDTH - (columns >> 1) : 0;	// FIXME wrong with positive slope ??
				u16 offset_y = lines < SCREEN_HEIGHT ? SCREEN_HALF_HEIGHT - (lines >> 1) : 0;	// FIXME offset hdma --> need to handle full-height hdma with 4 streaks of 60 lines ?
				//consoleNocashMessage("Voronoi hdma offsets x=%d y=%d\n", offset_x, offset_y);
				u16 split_x = slope == 0
					? (SCREEN_HALF_WIDTH << 8)
					: (slope > 0 ? (offset_x << 8) : ((255 - offset_x) << 8));
				//consoleNocashMessage("Voronoi starting at X = 0x%04x\n", split_x);
				

				// FIXME if dy == 0 --> slope is 0 and window is half-screen (top / right) --> handle special case ?!

				u8* ptr = window_positions_table + 5;
				u8 split = split_x >> 8;
				u8 y;
				for (y=0; y<SCREEN_HEIGHT; y++) {
					if ((y%60) == 0) {
						ptr++;
					}
					// FIXME simplify this mess...
					if (dx > 0) {	// W1 to the left
						if (split == 255) {	// Hide W2 altogether
							*(ptr++) = 0;			// Window 1 Left
							*(ptr++) = split;		// Window 1 Right
							*(ptr++) = split;		// Window 2 Left
							*(ptr++) = 0;			// Window 2 Right
						} else if (split == 0) { // Hide W1 altogether
							*(ptr++) = 255;			// Window 1 Left
							*(ptr++) = split;		// Window 1 Right
							*(ptr++) = split;		// Window 2 Left
							*(ptr++) = 255;			// Window 2 Right
						} else {
							*(ptr++) = 0;			// Window 1 Left
							*(ptr++) = split-1;		// Window 1 Right
							*(ptr++) = split+1;		// Window 2 Left
							*(ptr++) = 255;			// Window 2 Right
						}
					} else {	// W1 to the right
						if (split == 255) {	// Hide W1 altogether
							*(ptr++) = split;		// Window 1 Left
							*(ptr++) = 0;			// Window 1 Right
							*(ptr++) = 0;			// Window 2 Left
							*(ptr++) = split;		// Window 2 Right
						} else if (split == 0) { // Hide W2 altogether
							*(ptr++) = split;		// Window 1 Left
							*(ptr++) = 255;			// Window 1 Right
							*(ptr++) = 255;			// Window 2 Left
							*(ptr++) = split;		// Window 2 Right
						} else {
							*(ptr++) = split+1;		// Window 1 Left
							*(ptr++) = 255;			// Window 1 Right
							*(ptr++) = 0;			// Window 2 Left
							*(ptr++) = split-1;		// Window 2 Right
						}

						
					}
					if (y >= offset_y && y < (SCREEN_HEIGHT-offset_y)) {
						split_x += slope;
						split = split_x >> 8;
					}
					if (y >= (SCREEN_HEIGHT-offset_y)) {
						split_x = (slope > 0) ? (255 << 8) : 0;
						split = split_x >> 8;
					}
				}
			}

		}

		// TODO Compute players distance + scroll backgrounds + voronoi split ?

		// TODO During VBlank or use double buffer?
    }
    return 0;
}