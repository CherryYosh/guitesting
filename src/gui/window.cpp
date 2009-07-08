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
 * 	Binds several Controls together, if one moves they all move
 *
 * TODO:
 * 	Fix close so that the window will stay in memory for X seconds before being handled by GC
 *
 * James Brandon Stevenson
 *
 */

#include <GL/glew.h>

#include "window.h"

#include "../events/gui/colorchange.h"

#include "controls.h"
#include "../gui.h"
#include "../camera.h"
#include <SDL/SDL.h>

Window::Window() : gui(NULL), renderer(NULL), activeEvents(), Control(WindowType, this) {
	rotation.make_identity();
}

Window::Window(GUI* p, Renderer* r) : gui(p), renderer(r), activeEvents(), Control(WindowType, this) {
	rotation.make_identity();
}

Window::Window(const Window& orig) : gui(orig.gui), renderer(orig.renderer), activeEvents(orig.activeEvents),
rotation(orig.rotation), Control(orig) { };

Window::~Window() {
	gui->CloseWindow(this);
	gui = NULL;

	renderer->RemoveObject(this);
	renderer = NULL;
	mouseOverChild = NULL;

	activeEvents.clear();
}

Control* Window::clone(){
	return new Window(*this);
}

void Window::AddChild(Control *child) {
	child->SetRoot(this);

	//need to check if we are adding something that will make it wider or higher
	//NOTE:we assume the child's position is a offset..
	int delta;
	if ((delta = (child->GetX() + child->GetWidth()) - GetWidth()) > 0) {
		SetWidth(GetWidth() + delta);
	}
	if ((delta = (child->GetY() + child->GetHeight()) - GetHeight()) > 0) {
		SetHeight(GetHeight() + delta);
	}

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		if (children[i]->HitTest(child->GetX(), child->GetY())) {
			children[i]->AddChild(child);
			return;
		}
	}

	children.push_back(child);
}

void Window::Move(float xChange, float yChange) {
	//allows for correct movement while rotated
	util::vec4<float> change = util::vec4<float>(xChange, yChange, 0.0, 1.0);
	change = inverse(rotation) * change;

	x += change.x;
	y += change.y;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->Move(change.x, change.y);
	}
}

void Window::Close() {
	gui->CloseWindow(this);
	//delete [] this;
}

void Window::UpdateControl(Control* control) {
	if (control == NULL)
		return;

	renderer->Update(control, RENDERER_REFRESH);
}

/**
 * Returns the total size of the control, in this case the same as TotalChildren
 */
unsigned int Window::Size() {
	return TotalChildren();
}

/** \brief Preforms a hit test on the window
 *
 * The functions finds the mouse coords in object space, thus working with 3d rotation,
 * and figures out if they are inside the window.
 *
 * @param mx the mouse's x coordinate
 * @param my the mouse's y coordinate
 * @param p  the projection matrix's array
 * @returns true if the hit test was succful
 * @throws nothing
 */
bool Window::HitTest(float mx, float my) {
	Unproject(mx, my, &mx, &my);

	if (mx > x && my > y &&
		mx < (x + width) && my < (y + height)) {

		StartEvent("onHover");

		if (mouseOverChild != NULL) {
			if (mouseOverChild->HitTest(mx, my)) {
				return true;
			} else {
				mouseOverChild->OnMouseLeave();
			}
		}

		//we iterate past the controls backwards so that we will get the top itmes
		//FILO order
		unsigned int i = children.size();
		while (0 < i) {
			i--;

			if (children[i] != mouseOverChild && children[i]->HitTest(mx, my)) {
				mouseOverChild = children[i];
				mouseOverChild->OnMouseEnter();
				return true;
			}
		}

		mouseOverChild = NULL;
		return true;
	}

	if (mouseOverChild != NULL) {
		mouseOverChild->OnMouseLeave();
		mouseOverChild = NULL;
	}

	return false;
}

void Window::OnKeyPress(unsigned short unicode, int key, int mod) {
	if (activeChild != NULL)
		activeChild->OnKeyPress(unicode, key, mod);
}

void Window::OnMousePress(unsigned short button, int mx, int my) {
	if (mouseOverChild != NULL) {
		activeChild = mouseOverChild;
		activeChild->OnMousePress(button, mx, my);
	}
}

bool Window::OnMouseClick(unsigned short num, bool final) {
	if (activeChild != NULL) {
		return activeChild->OnMouseClick(num, final);
	}
	return false;
}

void Window::Unproject(float winx, float winy, float* ox, float* oy) {
	GLint* view = renderer->GetViewport();

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

void Window::AddEvent(Event* e) {
	activeEvents.push_back(e);
}

void Window::RemoveEvent(Event* e) {
	std::vector<Event*>::iterator it;
	for (it = activeEvents.begin(); it != activeEvents.end(); it++) {
		if (*it == e) {
			activeEvents.erase(it);
			return;
		}
	}
}

void Window::ReloadTheme(){
	size_t size = children.size();
	for(unsigned int i = 0; i < size; i++){
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