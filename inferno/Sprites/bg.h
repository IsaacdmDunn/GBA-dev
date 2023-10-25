
//{{BLOCK(bg)

//======================================================================
//
//	bg, 512x512@4, 
//	+ palette 256 entries, not compressed
//	+ 1980 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 64x64 
//	Total size: 512 + 38856 + 8192 = 47560
//
//	Time-stamp: 2023-09-25, 20:53:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BG_H
#define GRIT_BG_H

#define bgTilesLen 38856
extern const unsigned short bgTiles[19428];

#define bgMapLen 8192
extern const unsigned short bgMap[4096];

#define bgPalLen 512
extern const unsigned short bgPal[256];

#endif // GRIT_BG_H

//}}BLOCK(bg)
