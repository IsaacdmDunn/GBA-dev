//
//  orb movement
//
//! \file orb.c
//! \author J Vijn
//! \date 20070216 - 20070216
//
// === NOTES ===
// * Code is done for shortness and ease of use, 
//   not necessarily speed.

#include <tonc.h>

#include "ORBobj.h"

#define orb_SPEED	0x0001

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

static void orb_ani_stand(TSpriteORB* orb);
static void orb_ani_walk(TSpriteORB* orb);

// --------------------------------------------------------------------
// LUTS
// --------------------------------------------------------------------




// Default orb attributes
const OBJ_ATTR corbObjs[1] =
{
	{	0, ATTR1_SIZE_16,  ATTR2_BUILD(128,0,0), 0},
};


// --------------------------------------------------------------------
// FUNCTIONS
// --------------------------------------------------------------------

void orb_init(TSpriteORB* orb, int x, int y, int obj_id)
{
	orb->x = x;
	orb->y = y;
	orb->vx = qran_range(-3, 3);
	orb->vy = qran_range(-3, 3);

	if (orb->vx == 0)
	{
		orb->vx = 1;
	}
	if (orb->vy == 0)
	{
		orb->vy = 1;
	}

	orb->objId = obj_id;

	oam_copy(&obj_buffer[obj_id], corbObjs, 1);
}




void orb_move(TSpriteORB* orb)
{


	// TODO : collision testing here?
	if (orb->x < 0)
	{
		orb->vx = -orb->vx;
	}
	else if (orb->x > 226) {
		orb->vx = -orb->vx;
	}

	if (orb->y < 0)
	{
		orb->vy = -orb->vy;
	}
	else if (orb->y > 144) {
		orb->vy = -orb->vy;
	}

	orb->x += orb->vx;
	orb->y += orb->vy;
	//meta sprite move

	POINT pt = { (orb->x) - g_vp.x, (orb->y) - g_vp.y };
	OBJ_ATTR* obj = &obj_buffer[orb->objId];
	obj->attr2 = ATTR2_BUILD(128, orb->palette, 0);
	obj_set_pos(obj, pt.x, pt.y);
	
}




// EOF
