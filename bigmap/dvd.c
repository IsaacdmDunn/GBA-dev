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

#define dvd_SPEED	0x0040

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
	dvd->vx= dvd->vy= 0;

	dvd->objId= obj_id;

	oam_copy(&obj_buffer[obj_id], cdvdObjs, 8);
}




void dvd_move(TSpriteDVD*dvd)
{
	POINT pt = { (dvd->x >> 8) - g_vp.x, (dvd->y >> 8) - g_vp.y };
	OBJ_ATTR* obj = &obj_buffer[dvd->objId];
	// TODO : collision testing here?
	if (dvd->x > 240)
	{
		dvd->x += dvd_SPEED;
		dvd->y += dvd_SPEED / 3 * 2;
	}
	else {
		dvd->x -= dvd_SPEED;
		dvd->y -= dvd_SPEED;
	}
	//obj_set_pos(obj, pt.x, pt.y);
}




// EOF
