
//{{BLOCK(gameScreen)

//======================================================================
//
//	gameScreen, 240x160@4, 
//	+ palette 256 entries, not compressed
//	+ 599 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 512 + 5848 + 1200 = 7560
//
//	Time-stamp: 2023-10-20, 17:33:39
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAMESCREEN_H
#define GRIT_GAMESCREEN_H

#define gameScreenTilesLen 5848
extern const unsigned short gameScreenTiles[2924];

#define gameScreenMapLen 1200
extern const unsigned short gameScreenMap[600];

#define gameScreenPalLen 512
extern const unsigned short gameScreenPal[256];

#endif // GRIT_GAMESCREEN_H

//}}BLOCK(gameScreen)
