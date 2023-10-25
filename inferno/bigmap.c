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
#include "ORBobj.h"

#include "sound.h"
#include "background.h" 

#include "GameOverScreen.h"
#include "gameScreen.h"
#include "titleScreen.h"

#include "peter.h" 
#include "rect.h"
#include "orb.h"

#include "Music/gameOverFFF.h"
#include "Music/gamePlasticWastelandt.h"

#define kakarikoMapPitch 128
int j;

//all pallettes
unsigned int GFXPal[sizeof(peterPal) / sizeof(peterPal[0]) + sizeof(rectPal) / sizeof(rectPal[0])];
unsigned int GFXPalLen = peterPalLen + rectPalLen;

//all tiles
unsigned int GFXTiles[sizeof(peterTiles) / sizeof(peterTiles[0]) + sizeof(rectTiles) / sizeof(rectTiles[0]) + sizeof(orbTiles) / sizeof(orbTiles[0])];
unsigned int GFXTilesLen = peterTilesLen + rectTilesLen + orbTilesLen;

OBJ_ATTR obj_buffer[128];

TSprite g_link;
TSpriteDVD g_dvd;
TSpriteORB g_orb[16];

u8 txt_scrolly = 8;

u8 orbs = 0;

u32 score = 0;
u32 hiscore = 0;
u32 sec = -1;
u32 timerReset = 0;

unsigned int transitionEffect = 128;
unsigned int transitionSpeed = 8;
bool fadein = false;

enum levelState
{
	titleScreen = 0, game = 1, gameover = 2
};
u16 level = titleScreen;

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

void initGame()
{
	fadein = true;
	transitionEffect = 128;

	REG_TM2CNT = TM_ENABLE;

	for (int i = 0; i < 16; i++)
	{
		g_orb[i].vx = 0;
		g_orb[i].vy = 0;
		g_orb[i].x = 1000;
		g_orb[i].y = 1000;
	}

	*sound_control = 0;
	// Basic setups
	resetMusic = true;
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);
	oam_init(obj_buffer, 6);

	// Bigmap setup
	LZ77UnCompVram(gameScreenTiles, tile_mem[0]);
	GRIT_CPY(pal_bg_mem, gameScreenPal);
	bgt_init(&g_bg, 1, BG_CBB(0)|BG_SBB(29), gameScreenMap,30, 20);


	int i;
	for (i = 0; i < sizeof(peterPal) / sizeof(peterPal[0]); i++)
	{
		GFXPal[i] = peterPal[i];
	}
	

	for (i = 0; i < sizeof(peterTiles) / sizeof(peterTiles[0]); i++)
	{
		GFXTiles[i] = peterTiles[i];
	}
	for (j = i; j < sizeof(rectTiles) / sizeof(rectTiles[0]) + i; j++)
	{
		GFXTiles[j] = rectTiles[j - i]; 
	} 
	i = j;
	
	for (j = i; j < sizeof(orbTiles) / sizeof(orbTiles[0]) + i; j++)
	{
		GFXTiles[j] = orbTiles[j - i];
	}
	
	
	
	
	// Object setup
	GRIT_CPY(pal_obj_mem, GFXPal);
	GRIT_CPY(tile_mem[4], GFXTiles);
	link_init(&g_link,  120 << 8, 80 << 8, 0); 
	/*
	GRIT_CPY(pal_obj_mem, rectPal);
	GRIT_CPY(tile_mem[4], rectTiles); */
	dvd_init(&g_dvd, 176, 128, 3);
	orb_init(&g_orb[0], 0, 0, 11);
	

	//# NOTE: erasing and rendering text flows over into the VDRAW period.
	//# Using the ASM renderer and placing the text at the bottom limits its effects.
	tte_init_chr4c_b4_default(0, BG_CBB(2)|BG_SBB(28));
	tte_set_drawg(chr4c_drawg_b4cts_fast);
	tte_init_con();

	//init_textbox(0, 8, 8, SCR_W-8, 8+2*12);

	init_textbox(0, 8, 0, SCR_W-8, 16);
	REG_DISPCNT= DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ | 
		DCNT_OBJ_1D | DCNT_WIN0;

	
}

void initTitle() {
	*sound_control = 0;
	// Basic setups
	resetMusic = true;


	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	// Bigmap setup
	LZ77UnCompVram(titleScreenTiles, tile_mem[0]);
	GRIT_CPY(pal_bg_mem, titleScreenPal);
	bgt_init(&g_bg, 1, BG_CBB(0) | BG_SBB(29), titleScreenMap, 30, 20); 

	//# NOTE: erasing and rendering text flows over into the VDRAW period.
	//# Using the ASM renderer and placing the text at the bottom limits its effects.
	tte_init_chr4c_b4_default(0, BG_CBB(2) | BG_SBB(28));
	tte_set_drawg(chr4c_drawg_b4cts_fast);
	tte_init_con();

	//init_textbox(0, 8, 8, SCR_W-8, 8+2*12);

	init_textbox(0, 8, 0, SCR_W - 8, 16);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ |
		DCNT_OBJ_1D | DCNT_WIN0;

}

void initGameOver() {
	*sound_control = 0;
	resetMusic = true;
	fadein = true;
	transitionEffect = 128;
	// Basic setups
	irq_init(NULL);
	irq_add(II_VBLANK, NULL);

	

	for (int i = 0; i < sizeof(obj_buffer) / sizeof(obj_buffer[0]); i++)
	{
		obj_hide(&obj_buffer[i]);
	}





	// Bigmap setup
	LZ77UnCompVram(GameOverScreenTiles, tile_mem[0]);
	GRIT_CPY(pal_bg_mem, GameOverScreenPal);
	bgt_init(&g_bg, 1, BG_CBB(0) | BG_SBB(29), GameOverScreenMap, 30, 20);

	

	//# NOTE: erasing and rendering text flows over into the VDRAW period.
	//# Using the ASM renderer and placing the text at the bottom limits its effects.
	tte_init_chr4c_b4_default(0, BG_CBB(2) | BG_SBB(28));
	tte_set_drawg(chr4c_drawg_b4cts_fast);
	tte_init_con();

	//init_textbox(0, 8, 8, SCR_W-8, 8+2*12);

	init_textbox(0, 8, 0, SCR_W - 8, 16);
	REG_DISPCNT = DCNT_MODE0 | DCNT_BG0 | DCNT_BG1 | DCNT_OBJ |
		DCNT_OBJ_1D | DCNT_WIN0;
}

void updateGameOver() {

	if (fadein)
	{
		transitionEffect -= 8;
	}

	if (transitionEffect == 0)
	{
		fadein = false;
	}

	if (resetMusic)
	{

		play_sound(GameOverFFF, GameOverFFF_bytes, 16000, 'B');
		resetMusic = false;
	}

	if (key_is_down(KEY_ACCEPT))
	{
		resetMusic = true;
		initGame();
		level = 1;
	}

	

	tte_printf("#{es;P} Press the start button to try again"); 
}
void updateGame() {
	if (fadein)
	{
		transitionEffect-=8;
	}

	if (transitionEffect == 0)
	{
		fadein = false;
	}

	if (REG_TM3D != sec)
	{
		sec = REG_TM3D;
	}

	if (sec > ((10000 * (orbs + 1)) + timerReset) && orbs < 16)
	{
		sec = 0;
		orbs++;
		orb_init(&g_orb[orbs], 0, 0, orbs + 12);
	}

	int x, y;
	
	if (resetMusic)
	{ 
		
		play_sound(gamePlasticWastelandt, gamePlasticWastelandt_bytes, 16000, 'B');
		resetMusic = false; 
	}

	dvd_move(&g_dvd);
	for (int i = 0; i < orbs + 1; i++)
	{
		
		orb_move(&g_orb[i]);
	}
	

	link_input(&g_link);
	link_animate(&g_link);
	link_move(&g_link);

	x = g_link.x >> 8, y = g_link.y >> 8;

	if (g_dvd.x < x + 16 && g_dvd.x + 64 > x &&
		g_dvd.y < y + 24 && g_dvd.y + 32 > y)
	{
		g_dvd.palette = 0;
		score += 1;
	}
	else
	{
		g_dvd.palette = 1;
	}
	for (int i = 0; i < orbs + 1; i++)
	{
		if (g_orb[i].x < x + 16 && g_orb[i].x + 10 > x &&
			g_orb[i].y < y + 24 && g_orb[i].y + 10 > y)
		{
			timerReset = sec;
			resetMusic = true;
			orbs = 0;
			initGameOver();
			level = 2;
			if (score > hiscore)
			{
				hiscore = score;
			}
			score = 0;
		}
	}
	
	

	oam_copy(oam_mem, obj_buffer, 128);

	//vp_center(&g_vp, x, y);


	bgt_update(&g_bg, &g_vp);
	tte_printf("#{es;P} Score = %d     HiScore = %d",
		score, hiscore);


}
void updateTitle() {
	if (resetMusic)
	{

		play_sound(GameOverFFF, GameOverFFF_bytes, 16000, 'B');
		resetMusic = false;
	}

	if (key_is_down(KEY_ANY))
	{
		level = 1;
		initGame();
	}

	
	tte_printf("#{es;P} Press the any button to play");
}

int main()
{
	
	
	initTitle();
	/* clear the sound control initially */
	*sound_control = 0;

	REG_TM2D = -0x4000;          // 0x4000 ticks till overflow
	REG_TM2CNT = TM_FREQ_1024;   // we're using the 1024 cycle timer

	// cascade into tm3
	REG_TM3CNT = TM_ENABLE | TM_CASCADE;
	
	
	
	
	while (1)
	{
		
		VBlankIntrWait();

		on_vblank();

		key_poll();

		switch (level)
		{
		case titleScreen:
			updateTitle();
			break;
		case game:
			updateGame();
			break;
		case gameover:
			updateGameOver();
			break;
		default:
			break;
		}
		
		

		REG_MOSAIC = MOS_BUILD(transitionEffect >> 3, transitionEffect >> 3, transitionEffect >> 3, transitionEffect >> 3);
		REG_BG1CNT = BG_CBB(0) | BG_SBB(29) | BG_MOSAIC;
		pal_bg_mem[0] = REG_VCOUNT;
	}
	return 0;
}

// EOF
