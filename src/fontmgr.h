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

 * 	Copyright 2008,2009 James Brandon Stevenson
 */
#ifndef FONTMGR_H
#define FONTMGR_H

#include <GL/gl.h>
#include <GL/glext.h>
//#include "shader.h"
class Shader;

struct FontChar;

void FontMgr_Init();
void FontMgr_Release();
void FontMgr_LoadFont(int fontID, const char *fontname, int fontsize);
//void FontMgr_GetCharData( int font, char p, FontChar& c );
int FontMgr_GetFontAscender(int fontID);
int FontMgr_GetFontDescender(int fontID);
unsigned int FontMgr_GetLineHeight(int fontID);
unsigned int FontMgr_GetStringWidth(int fontID, const char *text);
unsigned int FontMgr_GetImage( int fontID );

#endif
