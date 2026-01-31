
//{{BLOCK(maptiles)

//======================================================================
//
//	maptiles, 160x160@4, 
//	+ palette 16 entries, not compressed
//	+ 26 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 20x20 
//	Total size: 32 + 376 + 800 = 1208
//
//	Time-stamp: 2023-10-30, 14:52:39
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MAPTILES_H
#define GRIT_MAPTILES_H

#define maptilesTilesLen 376
extern const unsigned short maptilesTiles[188];

#define maptilesMapLen 800
extern const unsigned short maptilesMap[400];

#define maptilesPalLen 32
extern const unsigned short maptilesPal[16];

#endif // GRIT_MAPTILES_H

//}}BLOCK(maptiles)
