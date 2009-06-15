/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson, RJ
 */

/*
 * A font manage class using freetype,
 * All code except for portions of glDrawFont and GetCharData, and FONT_VBOData By RJ
 */

#include <GL/glew.h>

// Windows/OpenGL
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOSERVICE
#define NOMCX
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include "myext.h"

#include "utils/matrix.h"
#include "utils/vector.h"

// FreeType 2
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library library;
static FT_Face faces[12];

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

struct Font {
	Glyph glyphs[128]; // Fast path ASCII
	unsigned int textureID;
	unsigned int lineheight;
	int ascender;
	int descender; // This will be a negative value usually
	void *vertexdata;
	void *tcoordptr;
	void *indexdata;
};

struct Font_VBOData {
	float x, y;
	float s, t;
	float r, g, b, a;
};

static Font* fonts[12];

void FontMgr_Init() {
	// NULLing all this just in case
	for (int i = 0; i < 12; i++) {
		fonts[i] = NULL;
		faces[i] = NULL;
	}

	FT_Init_FreeType(&library);
}

void FontMgr_Release() {
	FT_Done_FreeType(library);

	for (int i = 0; i < 12; i++) {
		if (fonts[i] != NULL) {
			free(fonts[i]->vertexdata);
			free(fonts[i]->indexdata);
			delete fonts[i];
		}
	}
}

void FontMgr_LoadFont(int fontID, const char *fontname, int fontsize) {
	Font *nFont = new Font;
	char img_buffer[131072];
	int penX = 0;
	int penY = 0;

	//ZeroMemory(img_buffer, 131072);
	memset(img_buffer, 0, 131072);

	FT_New_Face(library, fontname, 0, &faces[fontID]);
	FT_Set_Char_Size(faces[fontID], 0, fontsize << 6, 72, 72);

	int gHighestHeight = 0;
	nFont->lineheight = faces[fontID]->size->metrics.height >> 6;
	nFont->ascender = faces[fontID]->size->metrics.ascender >> 6;
	nFont->descender = faces[fontID]->size->metrics.descender >> 6;

	// Vertex Array
	int vsize = sizeof(float) * 3 * 4 * 128;
	int tsize = sizeof(float) * 2 * 4 * 128;
	int totalsize = vsize + tsize;
	nFont->vertexdata = malloc(totalsize);
	VECTOR3 *vertexptr = (VECTOR3 *) nFont->vertexdata;
	float *texcoordptr = (float *) ((char *) nFont->vertexdata + vsize);
	nFont->tcoordptr = (void *) texcoordptr;

	// Index Array
	int isize = sizeof(unsigned short) * 4 * 128;
	nFont->indexdata = malloc(isize);
	unsigned short *indexptr = (unsigned short *) nFont->indexdata;

	for (int i = 0; i < 128; i++) {
		nFont->glyphs[i].index = FT_Get_Char_Index(faces[fontID], i);
		FT_Load_Glyph(faces[fontID], nFont->glyphs[i].index, FT_LOAD_RENDER);
		// Did it this way so it's readable
		int gPitch = faces[fontID]->glyph->bitmap.pitch;
		int gWidth = nFont->glyphs[i].width = faces[fontID]->glyph->bitmap.width;
		int gHeight = nFont->glyphs[i].height = faces[fontID]->glyph->bitmap.rows;

		// Extra
		nFont->glyphs[i].advance = faces[fontID]->glyph->metrics.horiAdvance >> 6;
		nFont->glyphs[i].bearingX = faces[fontID]->glyph->metrics.horiBearingX >> 6;
		nFont->glyphs[i].bearingY = faces[fontID]->glyph->metrics.horiBearingY >> 6;

		// Check Position
		if (penX + gWidth > 254) {
			penY += gHighestHeight + 2;
			penX = 0;
			gHighestHeight = 0;
		}

		nFont->glyphs[i].x = penX / 256.0f;
		nFont->glyphs[i].y = penY / 256.0f;
		nFont->glyphs[i].x2 = (penX + gWidth) / 256.0f;
		nFont->glyphs[i].y2 = (penY + gHeight) / 256.0f;

		for (int y = 0; y < gHeight; y++) {
			for (int x = 0; x < gWidth; x++) {
				int bPosition = 2 * (((penY + y) * 256) + penX + x);
				img_buffer[bPosition] = img_buffer[bPosition + 1] = faces[fontID]->glyph->bitmap.buffer[(y * gPitch) + x];
			}
		}

		// Check Height
		if (gHeight > gHighestHeight)
			gHighestHeight = gHeight;

		penX += gWidth + 2;

		// Vertex Array
		int vindex = i * 4;
		int tindex = i * 8;
		// Top Left
		vertexptr[vindex].x = nFont->glyphs[i].bearingX;
		vertexptr[vindex].y = 0 - nFont->glyphs[i].bearingY;
		vertexptr[vindex].z = 0;
		texcoordptr[tindex] = nFont->glyphs[i].x;
		texcoordptr[tindex + 1] = nFont->glyphs[i].y;
		indexptr[vindex] = vindex;
		// Bottom Left
		vertexptr[vindex + 1].x = nFont->glyphs[i].bearingX;
		vertexptr[vindex + 1].y = 0 - nFont->glyphs[i].bearingY + nFont->glyphs[i].height;
		vertexptr[vindex + 1].z = 0;
		texcoordptr[tindex + 2] = nFont->glyphs[i].x;
		texcoordptr[tindex + 3] = nFont->glyphs[i].y2;
		indexptr[vindex + 1] = vindex + 1;
		// Bottom Right
		vertexptr[vindex + 2].x = nFont->glyphs[i].bearingX + nFont->glyphs[i].width;
		vertexptr[vindex + 2].y = 0 - nFont->glyphs[i].bearingY + nFont->glyphs[i].height;
		vertexptr[vindex + 2].z = 0;
		texcoordptr[tindex + 4] = nFont->glyphs[i].x2;
		texcoordptr[tindex + 5] = nFont->glyphs[i].y2;
		indexptr[vindex + 2] = vindex + 2;
		// Top Right
		vertexptr[vindex + 3].x = nFont->glyphs[i].bearingX + nFont->glyphs[i].width;
		vertexptr[vindex + 3].y = 0 - nFont->glyphs[i].bearingY;
		vertexptr[vindex + 3].z = 0;
		texcoordptr[tindex + 6] = nFont->glyphs[i].x2;
		texcoordptr[tindex + 7] = nFont->glyphs[i].y;
		indexptr[vindex + 3] = vindex + 3;
	}

	unsigned int glTextureID;
	glGenTextures(1, &glTextureID);
	glBindTexture(GL_TEXTURE_2D, glTextureID);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 256, 256, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, img_buffer);

	nFont->textureID = glTextureID;

	fonts[fontID] = nFont;
}

//get the character data used in FontChar
/*
void FontMgr_GetCharData(int font, char p, FontChar& c) {
	Glyph g = fonts[font]->glyphs[c.c];

	FT_Vector kdelta;

	if (p == 0)
		kdelta.x = 0;
	else
		FT_Get_Kerning(faces[font], g.index, fonts[font]->glyphs[p].index, FT_KERNING_DEFAULT, &kdelta);

	c.s = g.x;
	c.t = g.y;
	c.s2 = g.x2;
	c.t2 = g.y2;
	c.height = g.height;
	c.width = g.width;
	c.x = g.bearingX;
	c.y = -g.bearingY;
	c.advance = g.advance + (kdelta.x >> 6);
}
*/
int FontMgr_GetFontAscender(int fontID) {
	if (fonts[fontID] != NULL)
		return fonts[fontID]->ascender;
	else
		return 0;
}

int FontMgr_GetFontDescender(int fontID) {
	if (fonts[fontID] != NULL)
		return fonts[fontID]->descender;
	else
		return 0;
}

unsigned int FontMgr_GetLineHeight(int fontID) {
	if (fonts[fontID] != NULL)
		return fonts[fontID]->lineheight;
	else
		return 0;
}

unsigned int FontMgr_GetStringWidth(int fontID, const char *text) {
	FT_Vector kdelta;
	if (fonts[fontID] != NULL) {
		Font *font = fonts[fontID];
		unsigned int strWidth = 0;
		size_t length = strlen(text);
		for (unsigned int i = 0; i < length; i++) {
			FT_Get_Kerning(faces[fontID], font->glyphs[*(text + i)].index, font->glyphs[*(text + i + 1)].index, FT_KERNING_DEFAULT, &kdelta);
			strWidth += font->glyphs[*(text + i)].advance + (kdelta.x >> 6);
		}
		return strWidth;
	} else
		return 0;
}

unsigned int FontMgr_GetImage(int fontID) {
	return fonts[fontID]->textureID;
}
