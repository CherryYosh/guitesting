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

#include "gui/controls.h"

//static values
std::map<std::string, WindowData*> Theme::windows;
std::map<std::string, WidgetData*> Theme::widgets;
Image* Theme::image;

struct WidgetData {
	std::string name;
	float x, y;
	float width, height;
};

struct ChildData {
	void SetLayer(std::string l) {
		if (l == "top")
			layer = TOP_LAYER;
		else if (l == "default")
			layer = DEFAULT_LAYER;
		else if (l == "bottom")
			layer = BOTTOM_LAYER;
	}

	WidgetData* Data;
	std::string Callback;
	LayerT layer;
	float x, y, z;
};

struct WindowData {
	std::vector<ChildData*> Children;
};

Theme::Theme() { }

Theme::~Theme() { }

void Theme::Init() {
	image = new devilImage();
}

bool Theme::LoadTheme(std::string themefile) {
	return LUABase::CallScript("scripts/xmlParse.lua", themefile);
}

WindowData* Theme::NewWindowData(std::string name, WindowData* data) {
	if (data != NULL)
		return windows[name] = data;
	else
		return windows[name] = new WindowData;
}

WidgetData* Theme::NewWidgetData(std::string name, WidgetData* data) {
	if (data != NULL)
		widgets[name] = data;
	else
		widgets[name] = new WidgetData;

	widgets[name]->name = name;
	return widgets[name];
}

ChildData* Theme::PushWidget(WindowData* window, std::string widget) {
	ChildData* data = new ChildData;

	data->Data = widgets[widget];
	data->Callback = "";
	data->x = 0;
	data->y = 0;
	data->z = 0;
	data->layer = DEFAULT_LAYER;

	window->Children.push_back(data);
	return data;
}

Window* Theme::GetWindow(std::string name) {
	WindowData* data = windows[name];

	if (data == NULL) {
		printf("ERROR! creatwindow\n");
		return NULL;
	}

	Window* ret = new Window(NULL, NULL);
	Control* child;
	WidgetData* wd;
	ChildData* cd;

	double wRecp = 1.0 / image->Width();
	double hRecp = 1.0 / image->Height();

	size_t size = data->Children.size();
	for (unsigned int i = 0; i < size; i++) {
		cd = data->Children[i];
		wd = cd->Data;

		__M_ControlCast(child, wd->name.substr(0, wd->name.find('.')));

		child->SetPosition(cd->x, cd->y);
		child->SetLayer(cd->layer);
		child->SetWidth(wd->width);
		child->SetHeight(wd->height);
		child->s = wd->x * wRecp;
		child->s2 = (wd->x + wd->width) * wRecp;
		child->t = 1 - (wd->y * hRecp);
		child->t2 = 1 - ((wd->y + wd->height) * hRecp);

		ret->AddChild(child);
	}
	
	return ret;
}

void Theme::SetImage(std::string path) {
	image->Load(path);
}

Image* Theme::GetImage() {
	return image;
}

unsigned int Theme::GetImageID() {
	return image->GetID();
}
