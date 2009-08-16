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

Window::Window() : gui(NULL), renderer(NULL), activeEvents(), top(0), bottom(0),
left(0), right(0), borderFinished(false), Widget(GUI_CONTAINER, this) {
	rotation.make_identity();
}

Window::Window(GUI* p, Renderer* r) : gui(p), renderer(r), activeEvents(), top(0), bottom(0),
left(0), right(0), borderFinished(false), Widget(GUI_CONTAINER, this) {
	rotation.make_identity();
}

Window::~Window() { }

Widget* Window::clone() {
	return new Window(*this);
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

bool Window::GetChildAttributes(long flags) {
	if (activeChild != NULL) return activeChild->attributes(flags);
	else return false;
}

void Window::AddChild(Widget* w){
	float wd, hd;
	wd = (w->GetX() + w->GetWidth()) - InternalWidth(); 
	hd = (w->GetY() + w->GetHeight()) - InternalHeight(); 

	if(wd < 0) wd = 0;
	if(hd < 0) hd = 0;

	if(wd > 0 || hd > 0) AdjustSize(wd, hd);

	if(borderFinished){
	    w->Move(left, top);
	}

	w->SetRoot(this);
	Widget::AddChild(w);
}

void Window::SetBorder(int t, int b, int l, int r){
    if(t < 0 || b < 0 || l < 0 || r < 0) std::invalid_argument("invalide range");

    top = t; bottom = b; left = l; right = r;
    SetSize(left + right, top + bottom);

    border = Widget::NewWidget("rule", "background.default", 0, 0, BACKGROUND_LAYER, RESIZE_ALL);
    AddChild(border);
    border->SetSize(GetWidth(), GetHeight());
}

float Window::InternalX(){
    return x + left;
}

float Window::InternalY(){
    return y + top;
}

float Window::InternalWidth(){
    return width - left - right;
}

float Window::InternalHeight(){
    return height - top - bottom;
}

void Window::SetInternalSize(int w, int h){
    SetSize(w + left + right, h + top + bottom);
}