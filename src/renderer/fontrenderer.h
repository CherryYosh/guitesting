/* 
 * File:   font.h
 * Author: brandon
 *
 * Created on June 2, 2009, 2:07 PM
 */

#ifndef _FONTRENDERER_H
#define	_FONTRENDERER_H

#include <string>
#include "../font.h"
#include "renderer.h"

//#ifdef _WIN32
#define DEFAULTFONT "C:\\Windows\\Fonts\\arial.ttf"
//#else
//#define DEFAULTFONT "/usr/share/fonts/corefonts/arial.ttf"
//#endif

class FontRenderer : public Renderer {
public:
    FontRenderer() : font(new Font(DEFAULTFONT)), Renderer() {};
    virtual ~FontRenderer(){};
protected:
    Font* font;
};

#endif	/* _FONT_H */

