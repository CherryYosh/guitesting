/* 
 * File:   font.cpp
 * Author: brandon
 * 
 * Created on June 2, 2009, 9:16 AM
 */

#include "font.h"

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "renderer/image.h"

#if USEOGL
#if USEDEVIL
#include "renderer/ogl/devilImage.h"
#define IMAGETYPE devilImage
#endif
#endif

FT_Library Font::library;

Font::Font() : image(), fontName("") {
	Init();
}

Font::Font(std::string font, unsigned int size) : fontName(font) {
	Init();
	Load(font, size);
}

Font::Font(const Font& orig) : image(orig.image), fontName(orig.fontName) {
	Init();
}

Font::~Font() { }

void Font::Init() {
	FT_Init_FreeType(&library);
}

void Font::Load(std::string font, unsigned int size) {
	char img_buffer[131072];
	unsigned int penX = 0;
	unsigned int penY = 0;

	memset(img_buffer, 0, 131072);

	FT_New_Face(library, font.c_str(), 0, &face);
	//FT_Select_Charmap(face, FT_ENCODING_UNICODE);
	FT_Set_Char_Size(face, 0, size << 6, 72, 72);

	int gHighestHeight = 0;
	height = face->size->metrics.height >> 6;

	for (int i = 0; i < 128; i++) {
		glyphs[i].index = FT_Get_Char_Index(face, i);
		FT_Load_Glyph(face, glyphs[i].index, FT_LOAD_RENDER);

		int gPitch = face->glyph->bitmap.pitch;
		int gWidth = glyphs[i].width = face->glyph->bitmap.width;
		int gHeight = glyphs[i].height = face->glyph->bitmap.rows;

		glyphs[i].advance = face->glyph->metrics.horiAdvance >> 6;
		glyphs[i].bearingX = face->glyph->metrics.horiBearingX >> 6;
		glyphs[i].bearingY = face->glyph->metrics.horiBearingY >> 6;

		// Check Position
		if (penX + gWidth > 254) {
			penY += gHighestHeight + 2;
			penX = 0;
			gHighestHeight = 0;
		}

		glyphs[i].x = penX / 256.0;
		glyphs[i].y = penY / 256.0f;
		glyphs[i].x2 = (penX + gWidth) / 256.0f;
		glyphs[i].y2 = (penY + gHeight) / 256.0f;

		for (int y = 0; y < gHeight; y++) {
			for (int x = 0; x < gWidth; x++) {
				int bPosition = 2 * (((penY + y) * 256) + penX + x);
				img_buffer[bPosition] = img_buffer[bPosition + 1] = face->glyph->bitmap.buffer[(y * gPitch) + x];
			}
		}

		// Check Height
		if (gHeight > gHighestHeight)
			gHighestHeight = gHeight;

		penX += gWidth + 2;

		chars[i].c = i;
		chars[i].s = glyphs[i].x;
		chars[i].s2 = glyphs[i].x2;
		chars[i].t = glyphs[i].y;
		chars[i].t2 = glyphs[i].y2;
		chars[i].width = glyphs[i].width;
		chars[i].height = glyphs[i].height;
		chars[i].advance = glyphs[i].advance;
		chars[i].bearingX = glyphs[i].bearingX;
		chars[i].bearingY = glyphs[i].bearingY;
	}

	hasKerning = FT_HAS_KERNING(face);

	//TODO: replace this
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, img_buffer);
}

/**
 * Resizes the font, reloading it with the new size
 */
void Font::Resize(unsigned int size) {
	Load(fontName, size);
}

unsigned int Font::GetHeight() {
	return height;
}

unsigned int Font::GetWidth(std::string) { }

Image* Font::GetImage() { }

unsigned int Font::GetImageID() {
	return id;
}

FontChar* Font::GetChar(char c) {
	return &chars[c];
}

/**
 * Returns the kerning between the left and right characters
 */
unsigned int Font::GetKerning(char left, char right){
	if(hasKerning){
		FT_Vector delta;
		unsigned int error;
		if(error = FT_Get_Kerning(face, glyphs[left].index, glyphs[right].index, FT_KERNING_DEFAULT, &delta)){
			printf("Error getting the kerning between %i and %i : %i", left, right, error);
			return 0;
		}
		return delta.x >> 6;
	} else {
		return 0;
	}
}
