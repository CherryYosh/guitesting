#ifndef FONTMGR_H
#define FONTMGR_H

#include <GL/gl.h>
#include <GL/glext.h>
#include "shader.h"

struct FontChar;

void FontMgr_Init();
void FontMgr_Release();
void FontMgr_LoadFont(int fontID, const char *fontname, int fontsize);
void FontMgr_glDrawText(int fontID, int tx, int ty, Shader* shader, const char *text);
void FontMgr_GetCharData( int font, char p, FontChar& c );
int FontMgr_GetFontAscender(int fontID);
int FontMgr_GetFontDescender(int fontID);
unsigned int FontMgr_GetLineHeight(int fontID);
unsigned int FontMgr_GetStringWidth(int fontID, const char *text);
unsigned int FontMgr_GetImage( int fontID );

#endif
