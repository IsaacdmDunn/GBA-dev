
//{{BLOCK(GameOverScreen)

//======================================================================
//
//	GameOverScreen, 240x160@4, 
//	+ palette 256 entries, not compressed
//	+ 126 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 512 + 1592 + 1200 = 3304
//
//	Time-stamp: 2023-10-20, 17:30:30
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GAMEOVERSCREEN_H
#define GRIT_GAMEOVERSCREEN_H

#define GameOverScreenTilesLen 1592
extern const unsigned short GameOverScreenTiles[796];

#define GameOverScreenMapLen 1200
extern const unsigned short GameOverScreenMap[600];

#define GameOverScreenPalLen 512
extern const unsigned short GameOverScreenPal[256];

#endif // GRIT_GAMEOVERSCREEN_H

//}}BLOCK(GameOverScreen)
