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

#include "theme.h"

#include "lua/luabase.h"
#include "gui/widgets.h"

//static values
std::map<std::string, ThemeData> Theme::textures;
std::map<std::string, Window*> Theme::widgets;
Image* Theme::image;

Theme::Theme() { }

Theme::~Theme() { }

void Theme::Init() {
	image = new devilImage();
}

bool Theme::LoadTheme(std::string themefile) {
	return LUABase::CallScript("./scripts/xmlParse.lua", themefile);
}

Window* Theme::NewWidget(std::string name, Window* data) {
	if (data != NULL)
		widgets[name] = (Window*) data->clone();
	else
		widgets[name] = new Window();

	widgets[name]->SetName(name);
	return widgets[name];
}

/** Static version of GetWidget
 * returns a copy of the given Widget
 */
Window* Theme::Widget(std::string name) {
	return(Window*) widgets[name];
}

/**
 * returns a copy of the given Widget
 */
Window* Theme::GetWidget(std::string name) {
	return(Window*) widgets[name];
}

/**
 * adds or replaces the texturedata for name, only used in Lua
 */
void Theme::AddTextureData(std::string name, int x, int y, int width, int height) {
	textures[name] = ThemeData(x, y, width, height);
}

/**
 * Creates a alias to the old data
 * NOTE: this is not a pointer, but new data
 */
void Theme::Alias(std::string newn, std::string old) {
	textures[newn] = textures[old];
}

ThemeData& Theme::GetData(std::string type) {
	return textures[type];
}

bool Theme::SetImage(std::string path) {
	return image->Load(path);
}

Image* Theme::GetImage() {
	return image;
}

unsigned int Theme::GetImageID() {
	return image->GetID();
}

unsigned int Theme::ImageWidth() {
	return image->Width();
}

unsigned int Theme::ImageHeight() {
	return image->Height();
}
