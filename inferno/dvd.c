//
//  dvd movement
//
//! \file dvd.c
//! \author J Vijn
//! \date 20070216 - 20070216
//
// === NOTES ===
// * Code is done for shortness and ease of use, 
//   not necessarily speed.

#include <tonc.h>

#include "dvd.h"

#define dvd_SPEED	0x0001

// --------------------------------------------------------------------
// EXTERNAL
// --------------------------------------------------------------------

typedef struct VIEWPORT
{
	int x, xmin, xmax, xpage;
	int y, ymin, ymax, ypage;
} VIEWPORT;

extern VIEWPORT g_vp; 
extern OBJ_ATTR obj_buffer[];

// --------------------------------------------------------------------
// DECLARATIONS
// --------------------------------------------------------------------

static void dvd_ani_stand(TSpriteDVD*dvd);
static void dvd_ani_walk(TSpriteDVD*dvd);

// --------------------------------------------------------------------
// LUTS
// --------------------------------------------------------------------




// Default dvd attributes
const OBJ_ATTR cdvdObjs[8]= 
{
	{	0, ATTR1_SIZE_16,  ATTR2_BUILD(96,1,0), 0}, 
	{0, ATTR1_SIZE_16 + 16,  ATTR2_BUILD(100,1,0), 0}, 
	{0, ATTR1_SIZE_16 + 32,  ATTR2_BUILD(104,1,0), 0}, 
	{0, ATTR1_SIZE_16 + 48,  ATTR2_BUILD(108,1,0), 0},

	{	16, ATTR1_SIZE_16,  ATTR2_BUILD(112,1,0), 0	}, 
	{	16, ATTR1_SIZE_16+16,  ATTR2_BUILD(116,1,0), 0	}, 
	{	16, ATTR1_SIZE_16 + 32,  ATTR2_BUILD(120,1,0), 0	}, 
	{	16, ATTR1_SIZE_16 + 48,  ATTR2_BUILD(124,1,0), 0	}
};


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

void dvd_init(TSpriteDVD*dvd, int x, int y, int obj_id)
{
	dvd->x= x;
	dvd->y= y;
	dvd->vx=dvd_SPEED;
	dvd->vy=-dvd_SPEED;

	dvd->objId= obj_id;

	oam_copy(&obj_buffer[obj_id], cdvdObjs, 8);
}




void dvd_move(TSpriteDVD*dvd)
{
	
	
	// TODO : collision testing here?
	if (dvd->x < 0)
	{
		dvd->vx = dvd_SPEED;
	}
	else if (dvd->x > 172){
		dvd->vx = -dvd_SPEED;
	}

	if (dvd->y < 0)
	{
		dvd->vy = dvd_SPEED;
	}
	else if (dvd->y > 128) {
		dvd->vy = -dvd_SPEED;
	}

	dvd->x += dvd->vx;
	dvd->y += dvd->vy;
	//meta sprite move
	for (int i = 0; i < 8; i++)
	{
		POINT pt = { (dvd->x) - g_vp.x, (dvd->y) - g_vp.y };
		OBJ_ATTR* obj = &obj_buffer[dvd->objId + i];
		obj->attr2 = ATTR2_BUILD((i*4)+96, dvd->palette, 0);
		if (i < 4)
		{
			obj_set_pos(obj, pt.x + (16 * i), pt.y);
		}
		else {
			obj_set_pos(obj, pt.x + (16 * (i - 4)), pt.y + 16);
		}
		
	}
	
}




// EOF
