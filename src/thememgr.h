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
#ifndef THEMEMGR_H
#define THEMEMGR_H

#include <string>
#include <vector>

//keep to the file
struct ThemeMgr_ImageDataT{
	unsigned int x; //the left side x
	unsigned int y; //the top side y;
	unsigned int x2; //the right x
	unsigned int y2; //the bottom y

	std::string type;
};

//keep this to file scope
struct ThemeMgr_ThemeDataT{
	unsigned int imageID;
	unsigned int width;
	unsigned int height;

	std::string Name;
	std::vector< ThemeMgr_ImageDataT* > data;
};

unsigned int ThemeMgr_LoadTheme( const char* );
unsigned int ThemeMgr_GetImage();
const ThemeMgr_ThemeDataT* ThemeMgr_GetTheme();

#endif
