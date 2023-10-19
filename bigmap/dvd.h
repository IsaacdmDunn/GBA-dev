//
//  Sprite dvd movement
//
//! \file dvd.h
//! \author J Vijn
//! \date 20070216 - 20070216
//
// === NOTES ===


#ifndef __DVD__
#define __DVD__





// === CLASSES ========================================================

typedef struct TSpriteDVD
{
	FIXED		x, y;		//!< Position
	FIXED		vx, vy;		//!< Velocity
	u8			objId;		//!< Object index
} TSpriteDVD;

#endif // __dvd__


// -------------------------------------------------------------------- 
// PROTOTYPES
// -------------------------------------------------------------------- 

void dvd_init(TSpriteDVD*dvd, int x, int y, int obj_id);

void dvd_move(TSpriteDVD*dvd);



// EOF
