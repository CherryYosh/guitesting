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
/* The window class
* 	Binds several Widgets together, if one moves they all move
*
* James Brandon Stevenson
*
*/
#include "window.h"

#include "widgets.h"
#include "../gui.h"
#include "../camera.h"
#include "../events/gui/close.h"
#include "../events/gui/colorchange.h"
#include <SDL/SDL.h>

Window::Window() : gui(NULL), renderer(NULL), activeEvents(), resizable(false), bordersSet(false), Widget(GUI_CONTAINER, this) {
	rotation.make_identity();
}

Window::Window(GUI* p, Renderer* r) : gui(p), renderer(r), activeEvents(), bordersSet(false), Widget(GUI_CONTAINER, this) {
	rotation.make_identity();
}

Window::~Window() { }

Widget* Window::clone() {
	return new Window(*this);
}

void Window::AddChild(Widget *child){ //cant do default because we need it to overload
	AddChild(child, false);
}

void Window::AddChild(Widget *child, bool border) {
	float delta = 0;
	if ((delta = (child->GetX() + child->GetWidth()) - InternalWidth()) > 0) {
		Resize(delta, 0);
	}
	if ((delta = (child->GetY() + child->GetHeight()) - InternalHeight()) > 0) {
		Resize(0, delta);
	}

	//inital position of the widget is a offset
	if (!border && bordersSet) child->Move(leftborder->GetWidth(), topborder->GetHeight());

	Widget::AddChild(child);
}

void Window::Move(float xChange, float yChange) {
	//allows for correct movement while rotated
	util::vec4<float> delta = util::vec4<float>(xChange, yChange, 0.0, 1.0);
	delta = inverse(rotation) * delta;

	x += delta.x;
	y += delta.y;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->Move(delta.x, delta.y);
	}
}

void Window::Close() {
	gui->CloseWindow(this);
}

void Window::UpdateWidget(Widget* w) {
	if (w == NULL) return;
	renderer->Update(w, RENDERER_REFRESH);
}

/**
* Returns the total size of the Widget, in this case the same as TotalChildren
*/
unsigned int Window::Size() {
	return TotalChildren();
}

/**
*
* The functions finds the mouse coords in object space, thus working with 3d rotation,
* and figures out if they are inside the window.
*
* @param mx the mouse's x coordinate
* @param my the mouse's y coordinate
*/
bool Window::MouseTest(float mx, float my) {
	Unproject(mx, my, &mx, &my);
	return Widget::MouseTest(mx, my);
}

void Window::OnKeyPress(unsigned short unicode, int key, int mod) {
	if (activeChild != NULL)
		activeChild->OnKeyPress(unicode, key, mod);
}

void Window::OnMousePress(unsigned short button, int mx, int my) {
	LockMouse();

	if (mouseOverChild != NULL) {
		activeChild = mouseOverChild;
		activeChild->OnMousePress(button, mx, my);
	} else {
		activeChild = NULL;
	}
}

bool Window::OnMouseClick(unsigned short num, bool final) {
	if (activeChild != NULL) {
		return activeChild->OnMouseClick(num, final);
	}
	return false;
}

void Window::Unproject(float winx, float winy, float* ox, float* oy) {
	int* view = renderer->GetViewport();

	util::vec4<float> in = util::vec4<float>(((winx - view[0]) * 2.0) / view[2] - 1.0,
		-(((winy - view[1]) * 2.0) / view[3] - 1.0),
		(2 * (z * 0.001)) - 1,
		1.0);

	util::matrix4<float> pm;
	pm = (*renderer->GetCamera()->GetOrtho()) * rotation;

	util::vec4<float> ret = inverse(pm) * in;

	if (ret.w == 0.0)
		return;

	*ox = ret.x / ret.w;
	*oy = ret.y / ret.w;
}

util::matrix4<float>* Window::GetRotation() {
	return &rotation;
}

float* Window::GetRotationfv() {
	return rotation._array;
}

void Window::SetGUI(GUI* g) {
	gui = g;
}

GUI* Window::GetGUI() {
	return gui;
}

void Window::SetRenderer(Renderer* r) {
	renderer = r;
}

Renderer* Window::GetRenderer() {
	return renderer;
}

void Window::ActivateEvent(Event* e) {
	activeEvents.push_back(e);
}

void Window::DeactivateEvent(Event* e) {
	std::vector<Event*>::iterator it;
	for (it = activeEvents.begin(); it != activeEvents.end(); it++) {
		if (*it == e) {
			activeEvents.erase(it);
			return;
		}
	}
}

void Window::ReloadTheme() {
	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->ReloadTheme();
	}
}

void Window::StepEvents(unsigned int step) {
	size_t size = activeEvents.size();
	for (size_t i = 0; i < size; i++) {
		activeEvents[i]->Step(step);
	}
}

void Window::Rotate(float a, float x, float y, float z) {
	rotation.rotate(a, x, y, z);
}

void Window::SetBorders(int top, int bottom, int left, int right) {
	topborder = NewChild("rule.top", 0, 0, DEFAULT_LAYER, Vertical);
	topborder->SetHeight(top);

	bottomborder = NewChild("rule.bottom", 0, GetHeight(), DEFAULT_LAYER, Vertical);
	bottomborder->SetHeight(bottom);

	leftborder = NewChild("rule.left", 0, top, DEFAULT_LAYER, Horizontal);
	leftborder->SetWidth(left);

	rightborder = NewChild("rule.right", GetWidth() - right, top, DEFAULT_LAYER, Horizontal);
	rightborder->SetWidth(right);

	if (resizable) {
		leftedge = NewChild("edge.left", 0, GetHeight(), DEFAULT_LAYER, DontResize);
		leftedge->SetHeight(bottom);

		rightedge = NewChild("edge.right", 0, GetHeight(), DEFAULT_LAYER, DontResize);
		rightedge->SetHeight(bottom);
	}

	bordersSet = true;
	UpdateBorders();
}

void Window::UpdateBorders() {
	if (bordersSet) {
		topborder->SetPosition(GetX(), GetY()); // just to make sure
		topborder->SetWidth(GetWidth());

		if (resizable) {
			leftedge->SetPosition(GetX(), GetY() + GetHeight() - leftedge->GetHeight());

			bottomborder->SetPosition(GetX() + leftedge->GetWidth(), GetY() + GetHeight() - bottomborder->GetHeight());
			bottomborder->SetWidth(GetWidth() - leftedge->GetWidth() - rightedge->GetWidth());

			rightedge->SetPosition(bottomborder->GetX() + bottomborder->GetWidth(), GetY() + GetHeight() - rightedge->GetHeight());
		} else {
			bottomborder->SetPosition(GetX(), GetY() + GetHeight() - bottomborder->GetHeight());
			bottomborder->SetWidth(GetWidth());
		}

		leftborder->SetPosition(GetX(), GetY() + topborder->GetHeight());
		leftborder->SetHeight(GetHeight() - bottomborder->GetHeight() - topborder->GetHeight());

		rightborder->SetPosition(GetX() + GetWidth() - rightborder->GetWidth(), GetY() + topborder->GetHeight());
		rightborder->SetHeight(GetHeight() - bottomborder->GetHeight() - topborder->GetHeight());
	}
}

void Window::Resizable(bool v) {
	resizable = v;
}

bool Window::Resizable() {
	return resizable;
}

void Window::Resize(int wdelta, int hdelta) {
	size_t size = children.size();
	for (size_t i = 0; i < size; i++) {
		children[i]->Resize(wdelta, hdelta);
	}

	SetWidth(GetWidth() + wdelta);
	SetHeight(GetHeight() + hdelta);
	UpdateBorders();
}

float Window::InternalX() {
	if (bordersSet) {
		return GetX() + leftborder->GetWidth();
	} else return GetX();
}

float Window::InternalY() {
	if (bordersSet) {
		return GetY() + topborder->GetHeight();
	} else return GetY();
}

float Window::InternalWidth() {
	if (bordersSet) {
		return GetWidth() - leftborder->GetWidth() - rightborder->GetWidth();
	} else return GetWidth();
}

float Window::InternalHeight() {
	if (bordersSet) {
		return GetHeight() - topborder->GetHeight() - bottomborder->GetHeight();
	} else return GetHeight();
}

bool Window::GetChildAttributes(long flags) {
	if (activeChild != NULL) return activeChild->attributes(flags);
	else return false;
}

void Window::CloseButton(int x, int y, util::Color color) {
	//HACK!!!
	bool t = bordersSet;
	bordersSet = false;
	Widget* c = NewChild("button.close", x, y, TOP_LAYER, DontResize);
	bordersSet = t;

	ColorChangeEvent* ce = new ColorChangeEvent(c);
	ce->SetColor(color);

	c->AddEvent("onClick", new CloseEvent(c)); //OnClick we close
	c->AddEvent("onHover", ce); //onHover we turn to color
}