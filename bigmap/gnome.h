
//{{BLOCK(gnome)

//======================================================================
//
//	gnome, 240x160@4, 
//	+ palette 256 entries, not compressed
//	+ 239 tiles (t|f|p reduced) lz77 compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 512 + 5872 + 1200 = 7584
//
//	Time-stamp: 2023-09-25, 19:32:52
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GNOME_H
#define GRIT_GNOME_H

#define gnomeTilesLen 5872
extern const unsigned short gnomeTiles[2936];

#define gnomeMapLen 1200
extern const unsigned short gnomeMap[600];

#define gnomePalLen 512
extern const unsigned short gnomePal[256];

#endif // GRIT_GNOME_H

//}}BLOCK(gnome)
