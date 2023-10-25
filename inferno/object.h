
#include <tonc_types.h>
enum ELookDir
{
	LOOK_RIGHT = 0, LOOK_DOWN, LOOK_LEFT, LOOK_UP,
};

#define SPR_STATE_STAND		0x0100
#define SPR_STATE_WALK		0x0200

// === CLASSES ========================================================

typedef struct TSprite
{
	FIXED		x, y;		//!< Position
	FIXED		vx, vy;		//!< Velocity
	u16			state;		//!< Sprite state
	u8			dir;		//!< Look direction
	u8			objId;		//!< Object index
	FIXED		aniFrame;	//!< Animation frame counter
} TSprite;

