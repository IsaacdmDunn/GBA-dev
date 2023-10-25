
//{{BLOCK(worm)

//======================================================================
//
//	worm, 240x160@4, 
//	+ palette 256 entries, not compressed
//	+ 541 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 512 + 11700 + 1200 = 13412
//
//	Time-stamp: 2023-09-25, 19:17:12
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WORM_H
#define GRIT_WORM_H

#define wormTilesLen 11700
extern const unsigned short wormTiles[5850];

#define wormMapLen 1200
extern const unsigned short wormMap[600];

#define wormPalLen 512
extern const unsigned short wormPal[256];

#endif // GRIT_WORM_H

//}}BLOCK(worm)
