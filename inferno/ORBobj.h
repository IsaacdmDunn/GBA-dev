//
//  Sprite orb movement
//
//! \file orb.h
//! \author J Vijn
//! \date 20070216 - 20070216
//
// === NOTES ===


#ifndef __ORB__
#define __ORB__





// === CLASSES ========================================================

typedef struct TSpriteORB
{
	FIXED		x, y;		//!< Position
	FIXED		vx, vy;		//!< Velocity
	u8			objId;		//!< Object index
	int			palette;
} TSpriteORB;

#endif // __orb__


// -------------------------------------------------------------------- 
// PROTOTYPES
// -------------------------------------------------------------------- 

void orb_init(TSpriteORB* orb, int x, int y, int obj_id);

void orb_move(TSpriteORB* orb);



// EOF
