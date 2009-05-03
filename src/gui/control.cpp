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
#include <GL/glew.h>

#include "control.h"

#include "../thememgr.h"

std::vector<CTRL_GUIDataT*> guiData;

void Control_Init(const char* path) {
	unsigned int size = ThemeMgr_LoadTheme(path);
	const ThemeMgr_ThemeDataT* theme = ThemeMgr_GetTheme();

	for (unsigned int i = 0; i < size; i++) {
		//textrue data
		float x = (float) (theme->data[i]->x) / (float) theme->width;
		//heigh - y, convertes it from image space, 0,0, being top left, to texture space , 0,0 being bottome left
		float y = (float) (theme->height - theme->data[i]->y) / (float) theme->height;

		float x2 = (float) theme->data[i]->x2 / (float) theme->width;
		float y2 = (float) (theme->height - theme->data[i]->y2) / (float) theme->height;

		//add the data so it can get looked up
		CTRL_GUIDataT* temp = new CTRL_GUIDataT;

		temp->type = theme->data[i]->type;
		temp->width = theme->data[i]->x2 - theme->data[i]->x;
		temp->height = theme->data[i]->y2 - theme->data[i]->y;

		//set up the uv
		temp->s = x;
		temp->s2 = x2;
		temp->t = y;
		temp->t2 = y2;

		guiData.push_back(temp);
	}

	//time to gen the VBO's data
//	Control::GUI_vbo = new VBO();
}

//This should only be called when a new theme has been loaded or when the control is initlized

void Control::GetControlData() {
	size_t size = guiData.size();
	for (unsigned int i = 0; i < size; i++) {
		if (Type == guiData[i]->type) {
			Width = guiData[i]->width;
			Height = guiData[i]->height;

			s = guiData[i]->s;
			s2 = guiData[i]->s2;
			t = guiData[i]->t;
			t2 = guiData[i]->t2;

			return;
		}
	}
}

Control::Control(std::string t, Window* r, Control* p, float ix, float iy) {
	isEnabled = true;
	hasFocus = false;
	Type = t;
	x = ix;
	y = iy;
	VertexOffset = 1;

	SetColor(nv::vec4<float>(0.0));

	Attributes = 0;
	GetControlData();
	Parent = p;
	Root = r;

	MouseOverChild = NULL;
	ActiveChild = NULL;
}

Control::~Control() {
	Children.clear();
}

void Control::Activate() {
	//nothing needs to be done
}

bool Control::HitTest(int mx, int my) {
	if (mx > x && my > y &&
		mx < x + Width && my < y + Height) {
		if (MouseOverChild != NULL) {
			if (MouseOverChild->HitTest(mx, my)) {
				return true;
			} else {
				MouseOverChild->OnMouseLeave();
			}
		}

		size_t size = Children.size();
		for(unsigned int i = 0; i < size; i++) {
			if (Children[i] != MouseOverChild && Children[i]->HitTest(mx, my)) {
				MouseOverChild = Children[i];
				MouseOverChild->OnMouseEnter();
				return true;
			}
		}

		MouseOverChild = NULL;
		return true;
	}

	MouseOverChild = NULL;
	return false;
}

void Control::OnMousePress(unsigned short button, int mx, int my) {
	if (MouseOverChild != NULL) {
		ActiveChild = MouseOverChild;
		ActiveChild->OnMousePress(button, mx, my);
	}
}

void Control::OnMouseRelease(int button) {
	//called by input
}

bool Control::OnMouseClick(unsigned short num, bool final) {
	if (ActiveChild != NULL) {
		return ActiveChild->OnMouseClick(num, final);
	}

	return false;
}

void Control::OnKeyPress(unsigned short unicode) {
	//called by input
}

void Control::OnKeyRelease(int key, int mod) {
	//called by input
}

void Control::Move(float cx, float cy) {
	x += cx;
	y += cy;
}

void Control::SetEnabled(bool value) {
	isEnabled = value;
}

void Control::SetFocus(bool value) {
	hasFocus = value;
}

void Control::SetCallback(boost::function<void() > callback) {
	m_Callback = callback;
}

void Control::SetWidth(float w) {
	Width = w;
}

void Control::SetHeight(float h) {
	Height = h;
}

bool Control::HasAttrib(unsigned short a) {
	//will only return true if all the attributes are there
	return a == (Attributes & a);
}

float Control::GetWidth() {
	return Width;
}

float Control::GetHeight() {
	return Height;
}

void Control::SetColor(float r, float g, float b, float a) {
	Color[0] = r;
	Color[1] = g;
	Color[2] = b;
	Color[3] = a;
}

void Control::SetColor(nv::vec4<float> c) {
	Color = c;
}

void Control::AddColor(nv::vec4<float> c) {
	Color += c;
}

float* Control::GetColorv() {
	return Color._array;
}

void Control::OnMouseEnter() { }

void Control::OnMouseLeave() { }

void Control::AddChild(Control* c) {
	Children.push_back(c);
}

unsigned int Control::TotalChildren() {
	return Children.size();
}

unsigned int Control::NumChildren() {
	return Children.size();
}

Control* Control::GetChild( unsigned int num ){
	if( num < Children.size() ){
		return Children[num];
	} else {
		return NULL;
	}
}
