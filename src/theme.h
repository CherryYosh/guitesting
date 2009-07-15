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
#ifndef THEME_H
#define THEME_H

//one theme loaded at a time
//static class
//outside classes use ids..

#include <string>
#include <vector>
#include <map>

#include "renderer/ogl/devilImage.h"

class Window;

struct ThemeData{
	int s, t, width, height, orientation;

	ThemeData(){}
	ThemeData(int a, int b, int c, int d, int e) : s(a), t(b), width(c), height(d), orientation(e) {}
};

class Theme {
public:
    Theme();
    virtual ~Theme();

    void Init();
    bool LoadTheme(std::string);

    Window* NewWidget(std::string, Window* = NULL);

    static Window* Widget(std::string);
    Window* GetWidget(std::string);

    void AddTextureData(std::string, int, int, int, int, int);
    void Alias(std::string, std::string);

    static ThemeData& GetData(std::string);
    static bool SetImage(std::string);
    static unsigned int ImageWidth();
    static unsigned int ImageHeight();
    static Image* GetImage();
    static unsigned int GetImageID();
private:
    static std::map<std::string, ThemeData> textures;
    static std::map<std::string, Window*> widgets;
    static Image* image;
};

#endif
