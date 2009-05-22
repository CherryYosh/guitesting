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

//one theme loaded at a time
//static class
//outside classes use ids..

#include <string>
#include <vector>
#include <map>

#include "renderer/ogl/devilImage.h"
#include "lua/luabase.h"

//protos, class defined in the .cpp
struct WidgetData;
struct WindowData;
struct ChildData;
class Window;

class Theme {
public:
	Theme();
	virtual ~Theme();

	void Init();
	bool LoadTheme(std::string);

	WindowData* NewWindowData(std::string, WindowData*);
	WidgetData* NewWidgetData(std::string, WidgetData*);

	Window* CreateWindow(std::string);

	ChildData* PushWidget(WindowData*, std::string);

	static void SetImage(std::string);
	static Image* GetImage();
	static unsigned int GetImageID();
private:
	static std::map<std::string, WindowData*> windows;
	static std::map<std::string, WidgetData*> widgets;
	static Image* image;
};

#endif
