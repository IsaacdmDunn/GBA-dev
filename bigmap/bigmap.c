//
//  Big map scrolling and sprite animation
//
//! \file bigmap.c
//! \author J Vijn
//! \date 20060508 - 20070216
//
// === NOTES ===


#include <stdio.h>
#include <string.h>
#include <tonc.h>

#include "link.h"
#include "dvd.h"

#include "sound.h"
#include "background.h"

#include "gnome.h"
#include "link_gfx.h" 
#include "thing.h"
#include "Music/seashanty.h"
#include "Music/piracy.h"

#define kakarikoMapPitch 128
int j;

//all pallettes
unsigned int GFXPal[sizeof(link_gfxPal) / sizeof(link_gfxPal[0]) + sizeof(thingPal) / sizeof(thingPal[0])];
unsigned int GFXPalLen = link_gfxPalLen + thingPalLen;

//all tiles
unsigned int GFXTiles[sizeof(link_gfxTiles) / sizeof(link_gfxTiles[0]) + sizeof(thingTiles) / sizeof(thingTiles[0])];
unsigned int GFXTilesLen = link_gfxTilesLen + thingTilesLen;

OBJ_ATTR obj_buffer[128];

TSprite g_link;
TSpriteDVD g_dvd;

u8 txt_scrolly = 8;



void init_textbox(int bgnr, int left, int top, int right, int bottom)
{	
	tte_set_margins(left, top, right, bottom);

	REG_DISPCNT |= DCNT_WIN0;

	REG_WIN0H= left<<8 | right;
	REG_WIN0V= top<<8 | bottom;
	REG_WIN0CNT= WIN_ALL | WIN_BLD;
	REG_WINOUTCNT= WIN_ALL;

	REG_BLDCNT= (BLD_ALL&~BIT(bgnr)) | BLD_BLACK;
	REG_BLDY= 5;
}

void init_main()
{
	// Basic setups
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	oam_init(obj_buffer, 6);

	// Bigmap setup
	LZ77UnCompVram(gnomeTiles, tile_mem[0]);
	GRIT_CPY(pal_bg_mem, gnomePal);
	bgt_init(&g_bg, 1, BG_CBB(0)|BG_SBB(29), gnomeMap,30, 20);
	int i;
	for (i = 0; i < sizeof(link_gfxPal) / sizeof(link_gfxPal[0]); i++)
	{
		GFXPal[i] = link_gfxPal[i];
	}
	

	for (i = 0; i < sizeof(link_gfxTiles) / sizeof(link_gfxTiles[0]); i++)
	{
		GFXTiles[i] = link_gfxTiles[i];
	}
	for (j = i; j < sizeof(thingTiles) / sizeof(thingTiles[0]) + i; j++)
	{
		GFXTiles[j] = thingTiles[j - i]; 
	} 
	
	
	// Object setup
	GRIT_CPY(pal_obj_mem, GFXPal);
	GRIT_CPY(tile_mem[4], GFXTiles);
	link_init(&g_link,  1 << 8, 1 << 8, 3); 
	/*
	GRIT_CPY(pal_obj_mem, thingPal);
	GRIT_CPY(tile_mem[4], thingTiles); */
	dvd_init(&g_dvd, 30, 30, 11);

	//# NOTE: erasing and rendering text flows over into the VDRAW period.
	//# Using the ASM renderer and placing the text at the bottom limits its effects.
	tte_init_chr4c_b4_default(0, BG_CBB(2)|BG_SBB(28));
	tte_set_drawg(chr4c_drawg_b4cts_fast);
	tte_init_con();

	//init_textbox(0, 8, 8, SCR_W-8, 8+2*12);

	init_textbox(0, 8, SCR_H-(8+2*12), SCR_W-8, SCR_H-8);
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | 
		DCNT_OBJ_1D | DCNT_WIN0;

	
}

int main()
{

	int music = 1;
	int x, y;
	init_main();
	/* clear the sound control initially */
	*sound_control = 0;

	
	
	while(1)
	{

		VBlankIntrWait();
		
		on_vblank();

		key_poll();
		if (key_is_down(KEY_A))
		{
			
			play_sound(zelda_treasure_16K_mono, zelda_treasure_16K_mono_bytes, 16000, 'A');
		}
		if (resetMusic)
		{
			switch (music)
			{
			case 1:
				play_sound(piracy, piracy_bytes, 44100, 'B');
				//music = 0;
				break;
			case 0:
				play_sound(seashanty, seashanty_bytes, 44100, 'B');
				//music = 1;
				break;
			}
			resetMusic = false;
			//music++;
		}

		dvd_move(&g_dvd);

		link_input(&g_link);
		link_animate(&g_link);
		link_move(&g_link);

		x = g_link.x >> 8, y = g_link.y >> 8;

		int j = 0;
		if (g_dvd.x < x + 16 && g_dvd.x + 64 > x &&
			g_dvd.y < y + 24 && g_dvd.y + 32 > y)
		{
			g_dvd.palette = 0;
		}
		else
		{
			g_dvd.palette = 1;
		}

		oam_copy(oam_mem, obj_buffer, 128);

		//vp_center(&g_vp, x, y);
		

		bgt_update(&g_bg, &g_vp);

		tte_printf("#{es;P}( x,y) = (%d,%d,%d)", 
			j, g_dvd.x, x);
		pal_bg_mem[0]= REG_VCOUNT;
		//frame++;
	}
	return 0;
}

// EOF
