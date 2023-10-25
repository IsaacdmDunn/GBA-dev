//
//  Sprite Link movement
//
//! \file link.h
//! \author J Vijn
//! \date 20070216 - 20070216
//
// === NOTES ===


#ifndef __LINK__
#define __LINK__

#include "object.h"

// === CONSTANTS ======================================================


#endif // __LINK__


// -------------------------------------------------------------------- 
// PROTOTYPES
// -------------------------------------------------------------------- 

void link_init(TSprite *link, int x, int y, int obj_id); 
void link_set_state(TSprite *link, u32 state);
void link_input(TSprite *link);

void link_move(TSprite *link); 
void link_animate(TSprite *link);



// EOF
