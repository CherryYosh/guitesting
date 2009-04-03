#ifndef FONTMGR_H
#define FONTMGR_H

void FontMgr_Init();
void FontMgr_Release();
void FontMgr_LoadFont(int fontID, const char *fontname, int fontsize);
void FontMgr_glDrawText(int fontID, int tw, int th, const char *text);
int FontMgr_GetFontAscender(int fontID);
int FontMgr_GetFontDescender(int fontID);
unsigned int FontMgr_GetLineHeight(int fontID);
unsigned int FontMgr_GetStringWidth(int fontID, const char *text);

#endif