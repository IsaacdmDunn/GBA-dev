
//{{BLOCK(titleScreen)

//======================================================================
//
//	titleScreen, 240x160@4, 
//	+ palette 256 entries, not compressed
//	+ 249 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 512 + 3028 + 1200 = 4740
//
//	Time-stamp: 2023-10-21, 18:48:26
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLESCREEN_H
#define GRIT_TITLESCREEN_H

#define titleScreenTilesLen 3028
extern const unsigned short titleScreenTiles[1514];

#define titleScreenMapLen 1200
extern const unsigned short titleScreenMap[600];

#define titleScreenPalLen 512
extern const unsigned short titleScreenPal[256];

#endif // GRIT_TITLESCREEN_H

//}}BLOCK(titleScreen)
