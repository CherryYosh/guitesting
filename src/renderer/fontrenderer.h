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

class FontRenderer : public Renderer {
public:
    FontRenderer() : font(new Font("/usr/share/fonts/corefonts/arial.ttf")), Renderer() {};
    virtual ~FontRenderer(){};
protected:
    Font* font;
};

#endif	/* _FONT_H */

