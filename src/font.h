/* 
 * File:   font.h
 * Author: brandon
 *
 * Created on June 2, 2009, 9:16 AM
 */

#ifndef _FONT_H
#define	_FONT_H

#include <string>

#include "utils/vector.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class Image;

struct Glyph {
	unsigned int index;
	float x;
	float y;
	float x2;
	float y2;
	int width;
	int height;
	int advance;
	int bearingX;
	int bearingY;
};

struct FontChar {
	float s, t, s2, t2; //texcoords
	float width, height; //width, height of the text
	float advance;
	float bearingX, bearingY;

	union {
		char c; //the actual char
		char character;
	};
};

class Font {
public:
	Font();
	Font(std::string, unsigned int = 18);
	Font(const Font& orig);
	virtual ~Font();

	static void Init();

	void Load(std::string, unsigned int = 18);
	void Resize(unsigned int);

	unsigned int GetHeight();
	unsigned int GetWidth(std::string);

	Image* GetImage();
	unsigned int GetImageID();

	FontChar* GetChar(char c);
	unsigned int GetKerning(char, char);
private:
	static FT_Library library;

	std::string fontName;

	Image* image;
	unsigned int height;
	FT_Face face;

	bool hasKerning;

	unsigned int id;

	Glyph glyphs[128];
	FontChar chars[128];
};

#endif	/* _FONT_H */

