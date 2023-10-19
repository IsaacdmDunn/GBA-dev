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

#include "gnome.h"
#include "link_gfx.h" 
#include "thing.h"
#include "Music/seashanty.h"
#include "Music/piracy.h"
#include "Music/zelda_treasure_16K_mono.h"

#define kakarikoMapPitch 128
bool resetMusic = true;
int j;

//all pallettes
unsigned int GFXPal[sizeof(link_gfxPal) / sizeof(link_gfxPal[0]) + sizeof(thingPal) / sizeof(thingPal[0])];
unsigned int GFXPalLen = link_gfxPalLen + thingPalLen;

//all tiles
unsigned int GFXTiles[sizeof(link_gfxTiles) / sizeof(link_gfxTiles[0]) + sizeof(thingTiles) / sizeof(thingTiles[0])];
unsigned int GFXTilesLen = link_gfxTilesLen + thingTilesLen;

// === CONSTANTS ======================================================
// 
// 




// === Music classes ========================================================
/*
made by ian finlayson
https://ianfinlayson.net/
modified to work with libtonc
*/
/* define the timer control registers */
volatile unsigned short* timer0_data = (volatile unsigned short*)0x4000100;
volatile unsigned short* timer0_control = (volatile unsigned short*)0x4000102;
// /* pointers to the DMA source/dest locations and control registers */
volatile unsigned int* dma1_source = (volatile unsigned int*)0x40000BC;
volatile unsigned int* dma1_destination = (volatile unsigned int*)0x40000C0;
volatile unsigned int* dma1_control = (volatile unsigned int*)0x40000C4;
volatile unsigned int* dma2_source = (volatile unsigned int*)0x40000C8;
volatile unsigned int* dma2_destination = (volatile unsigned int*)0x40000CC;
volatile unsigned int* dma2_control = (volatile unsigned int*)0x40000D0;
volatile unsigned int* dma3_source = (volatile unsigned int*)0x40000D4;
volatile unsigned int* dma3_destination = (volatile unsigned int*)0x40000D8;
volatile unsigned int* dma3_control = (volatile unsigned int*)0x40000DC;
/* copy data using DMA channel 3 (normal memory transfers) */
void memcpy16_dma(unsigned short* dest, unsigned short* source, int amount) {
	*dma3_source = (unsigned int)source;
	*dma3_destination = (unsigned int)dest;
	*dma3_control = DMA_ENABLE | 0x00000000 | amount;
}
/* allows turning on and off sound for the GBA altogether */
volatile unsigned short* master_sound = (volatile unsigned short*)0x4000084;
/* has various bits for controlling the direct sound channels */
volatile unsigned short* sound_control = (volatile unsigned short*)0x4000082;
/* the location of where sound samples are placed for each channel */
volatile unsigned char* fifo_buffer_a = (volatile unsigned char*)0x40000A0;
volatile unsigned char* fifo_buffer_b = (volatile unsigned char*)0x40000A4;
/* global variables to keep track of how much longer the sounds are to play */
unsigned int channel_a_vblanks_remaining = 0;
unsigned int channel_a_total_vblanks = 0;
unsigned int channel_b_vblanks_remaining = 0;
/* the global interrupt enable register */
volatile unsigned short* interrupt_enable = (unsigned short*)0x4000208;
/* this register stores the individual interrupts we want */
volatile unsigned short* interrupt_selection = (unsigned short*)0x4000200;
/* this registers stores which interrupts if any occured */
volatile unsigned short* interrupt_state = (unsigned short*)0x4000202;
/* the address of the function to call when an interrupt occurs */
volatile unsigned int* interrupt_callback = (unsigned int*)0x3007FFC;
/* this register needs a bit set to tell the hardware to send the vblank interrupt */
volatile unsigned short* display_interrupts = (unsigned short*)0x4000004;
/* play a sound with a number of samples, and sample rate on one channel 'A' or 'B' */
void play_sound(const signed char* sound, int total_samples, int sample_rate, char channel) {
	/* start by disabling the timer and dma controller (to reset a previous sound) */
	*timer0_control = 0;
	if (channel == 'A') {
		*dma1_control = 0;
	}
	else if (channel == 'B') {
		*dma2_control = 0;
	}

	/* output to both sides and reset the FIFO */
	if (channel == 'A') {
		*sound_control |= SOUND_A_RIGHT_CHANNEL | SOUND_A_LEFT_CHANNEL | SOUND_A_FIFO_RESET;
	}
	else if (channel == 'B') {
		*sound_control |= SOUND_B_RIGHT_CHANNEL | SOUND_B_LEFT_CHANNEL | SOUND_B_FIFO_RESET;
	}

	/* enable all sound */
	*master_sound = SOUND_MASTER_ENABLE;

	/* set the dma channel to transfer from the sound array to the sound buffer */
	if (channel == 'A') {
		*dma1_source = (unsigned int)sound;
		*dma1_destination = (unsigned int)fifo_buffer_a;
		*dma1_control = 0x400000 | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;
	}
	else if (channel == 'B') {
		*dma2_source = (unsigned int)sound;
		*dma2_destination = (unsigned int)fifo_buffer_b;
		*dma2_control = 0x400000 | DMA_REPEAT | DMA_32 | DMA_SYNC_TO_TIMER | DMA_ENABLE;
	}

	/* set the timer so that it increments once each time a sample is due
	 * we divide the clock (ticks/second) by the sample rate (samples/second)
	 * to get the number of ticks/samples */
	unsigned short ticks_per_sample = CLOCK / sample_rate;

	/* the timers all count up to 65536 and overflow at that point, so we count up to that
	 * now the timer will trigger each time we need a sample, and cause DMA to give it one! */
	*timer0_data = 65536 - ticks_per_sample;

	/* determine length of playback in vblanks
	 * this is the total number of samples, times the number of clock ticks per sample,
	 * divided by the number of machine cycles per vblank (a constant) */
	if (channel == 'A') {
		channel_a_vblanks_remaining = total_samples * ticks_per_sample * (1.0 / CYCLES_PER_BLANK);
		channel_a_total_vblanks = channel_a_vblanks_remaining;
	}
	else if (channel == 'B') {
		channel_b_vblanks_remaining = total_samples * ticks_per_sample * (1.0 / CYCLES_PER_BLANK);
	}


	/* enable the timer */
	*timer0_control = TIMER_ENABLE | TIMER_FREQ_1;
}
/* this function is called each vblank to get the timing of sounds right */
void on_vblank() {


	/* look for vertical refresh */
	//if ((*interrupt_state & INTERRUPT_VBLANK) == INTERRUPT_VBLANK) {

	//VBlankIntrWait();
	/* update channel A */
	if (channel_a_vblanks_remaining == 0) {
		/* restart the sound again when it runs out */
		channel_a_vblanks_remaining = channel_a_total_vblanks;
		*dma1_control = 0;
		*dma1_source = (unsigned int)zelda_treasure_16K_mono;
		*dma1_control = 0x400000 | DMA_REPEAT | DMA_32 |
			IRQ_TIMER0 | DMA_ENABLE;

		/**sound_control &= ~(SOUND_A_RIGHT_CHANNEL | SOUND_A_LEFT_CHANNEL | SOUND_A_FIFO_RESET);
		*dma1_control = 0;*/
	}
	else {
		channel_a_vblanks_remaining--;
	}

	/* update channel B */
	if (channel_b_vblanks_remaining == 0) {
		resetMusic = true;
		/* disable the sound and DMA transfer on channel B */
		*sound_control &= ~(SOUND_B_RIGHT_CHANNEL | SOUND_B_LEFT_CHANNEL | SOUND_B_FIFO_RESET);
		*dma2_control = 0;

	}
	else {
		channel_b_vblanks_remaining--;
	}
	//}

}

// TODO: rename structs and members

typedef struct VIEWPORT
{
	int x, xmin, xmax, xpage;
	int y, ymin, ymax, ypage;
} VIEWPORT;


typedef struct TMapInfo
{
	union
	{
		u32 state;			//!< Background state
		struct
		{
			u16 flags;
			u16 cnt;
		};
	};
	// Destination data
	SCR_ENTRY *dstMap;		//!< Screenblock pointer	
	// Source data
	SCR_ENTRY *srcMap;		//!< Source map address
	u32 srcMapWidth;		//!< Source map width
	u32 srcMapHeight;		//!< Source map height
	FIXED mapX;			//!< X-coord on map (.8f)
	FIXED mapY;			//!< Y-coord on map (.8f)
} TMapInfo;


// === GLOBALS ========================================================


VIEWPORT g_vp= 
{
	0, 0, 1024, 240, 
	0, 0, 1024, 160, 
};

TMapInfo g_bg;

OBJ_ATTR obj_buffer[128];

TSprite g_link[5];
TSpriteDVD g_dvd;


// === PROTOTYPES =====================================================

INLINE void vp_center(VIEWPORT *vp, FIXED x, FIXED y);
void vp_set_pos(VIEWPORT *vp, FIXED x, FIXED y);

// === MACROS =========================================================
// === INLINES=========================================================
// === FUNCTIONS ======================================================
u8 txt_scrolly = 8;



// --- VIEWPORT ---

INLINE void vp_center(VIEWPORT *vp, int x, int y)
{	vp_set_pos(vp, x - vp->xpage/2, y - vp->ypage/2);	}


void vp_set_pos(VIEWPORT *vp, int x, int y)
{
	vp->x= clamp(x, vp->xmin, vp->xmax - vp->xpage);
	vp->y= clamp(y, vp->ymin, vp->ymax - vp->ypage);
}


// --- TMapInfo ---

void bgt_init(TMapInfo *bgt, int bgnr, u32 ctrl, 
	const void *map, u32 map_width, u32 map_height)
{
	memset(bgt, 0, sizeof(TMapInfo));

	bgt->flags= bgnr;
	bgt->cnt= ctrl;
	bgt->dstMap= se_mem[BFN_GET(ctrl, BG_SBB)];

	REG_BGCNT[bgnr]= ctrl;
	REG_BG_OFS[bgnr].x= 0;
	REG_BG_OFS[bgnr].y= 0;


	bgt->srcMap= (SCR_ENTRY*)map;
	bgt->srcMapWidth= map_width;
	bgt->srcMapHeight= map_height;

	int ix, iy;
	SCR_ENTRY *dst= bgt->dstMap, *src= bgt->srcMap;
	for(iy=0; iy<32; iy++)
		for(ix=0; ix<32; ix++)
			dst[iy*32+ix]= src[	iy*bgt->srcMapWidth+ix];
}

void bgt_colcpy(TMapInfo *bgt, int tx, int ty)
{
	int iy, y0= ty&31;

	int srcP= bgt->srcMapWidth;	
	SCR_ENTRY *srcL= &bgt->srcMap[ty*srcP + tx];
	SCR_ENTRY *dstL= &bgt->dstMap[y0*32 + (tx&31)];

	for(iy=y0; iy<32; iy++)
	{	*dstL= *srcL;	dstL += 32;	srcL += srcP;	}

	dstL -= 1024;

	for(iy=0; iy<y0; iy++)
	{	*dstL= *srcL;	dstL += 32;	srcL += srcP;	}
}

void bgt_rowcpy(TMapInfo *bgt, int tx, int ty)
{
	int ix, x0= tx&31;

	int srcP= bgt->srcMapWidth;	
	SCR_ENTRY *srcL= &bgt->srcMap[ty*srcP + tx];
	SCR_ENTRY *dstL= &bgt->dstMap[(ty&31)*32 + x0];

	for(ix=x0; ix<32; ix++)
		*dstL++= *srcL++;

	dstL -= 32;

	for(ix=0; ix<x0; ix++)
		*dstL++= *srcL++;
}

void bgt_update(TMapInfo *bgt, VIEWPORT *vp)
{
	// Pixel coords
	int vx= vp->x, vy= vp->y;
	int bx= bgt->mapX, by= bgt->mapY;

	// Tile coords
	int tvx= vx>>3, tvy= vy>>3;
	int tbx= bx>>3, tby= by>>3;

	// Basically, we need another row/col when the viewport
	// exposes another row/col, i.e., if the tile coords
	// have changed

	if(tvx < tbx)		// add on left
		bgt_colcpy(bgt, tvx, tvy);
	else if(tvx > tbx)	// add on right
		bgt_colcpy(bgt, tvx+31, tvy);

	if(tvy < tby)		// add on top
		bgt_rowcpy(bgt, tvx, tvy); 
	else if(tvy > tby)	// add on bottom
		bgt_rowcpy(bgt, tvx, tvy+31);

	// Update TMapInfo and reg-offsets
	int bgnr= bgt->flags;
	REG_BG_OFS[bgnr].x= bgt->mapX= vx;
	REG_BG_OFS[bgnr].y= bgt->mapY= vy;
}

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
	for (int i = 0; i < sizeof(g_link) / sizeof(g_link[0]); i++)
	{
		link_init(&g_link[i],  (i*8) << 8, (i*8) << 8, i*3); 
	}
	/*
	GRIT_CPY(pal_obj_mem, thingPal);
	GRIT_CPY(tile_mem[4], thingTiles); */
	dvd_init(&g_dvd, 30 << 8, 30 << 8, 23);

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
				//play_sound(piracy, piracy_bytes, 44100, 'B');
				//music = 0;
				break;
			case 0:
				//play_sound(seashanty, seashanty_bytes, 44100, 'B');
				//music = 1;
				break;
			}
			resetMusic = false;
			//music++;
		}

		dvd_move(&g_dvd);
		for (int i = 0; i < sizeof(g_link) / sizeof(g_link[0]); i++)
		{
			link_input(&g_link[i]);
			link_animate(&g_link[i]);
			link_move(&g_link[i]);


			x = g_link[i].x >> 8, y = g_link[i].y >> 8;
		}

		oam_copy(oam_mem, obj_buffer, 128);

		

		vp_center(&g_vp, x, y);
		

		bgt_update(&g_bg, &g_vp);

		tte_printf("#{es;P}( x,y) = (%d,%d,%d)", 
			j, sizeof(GFXTiles) / sizeof(GFXTiles[0]), sizeof(GFXPal) / sizeof(GFXPal[0]));
		pal_bg_mem[0]= REG_VCOUNT;
		//frame++;
	}
	return 0;
}

// EOF
