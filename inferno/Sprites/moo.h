
//{{BLOCK(moo)

//======================================================================
//
//	moo, 512x512@4, 
//	+ palette 256 entries, not compressed
//	+ 2008 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 64x64 
//	Total size: 512 + 42456 + 8192 = 51160
//
//	Time-stamp: 2023-09-25, 20:50:42
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_MOO_H
#define GRIT_MOO_H

#define mooTilesLen 42456
extern const unsigned short mooTiles[21228];

#define mooMapLen 8192
extern const unsigned short mooMap[4096];

#define mooPalLen 512
extern const unsigned short mooPal[256];

#endif // GRIT_MOO_H

//}}BLOCK(moo)
