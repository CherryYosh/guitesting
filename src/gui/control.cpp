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
#include "window.h"
#include "../events/gui/guievent.h"

Control::Control() : isEnabled(true), hasFocus(false), type(""), x(0), y(0), z(-TOP_LAYER), color(),
parent(NULL), root(NULL), mouseOverChild(NULL), activeChild(NULL) { }

Control::Control(const Control& orig) : isEnabled(orig.isEnabled), hasFocus(orig.hasFocus), type(orig.type),
x(orig.x), y(orig.y), z(orig.z), color(orig.color), parent(orig.parent), root(orig.root),
mouseOverChild(orig.mouseOverChild), activeChild(orig.activeChild) { }

Control::Control(std::string t, Window* r, Control* p, LayerT l, float ix, float iy) : isEnabled(true), hasFocus(false),
type(t), x(ix), y(iy), z(-TOP_LAYER), color(), parent(p), root(r), mouseOverChild(NULL), activeChild(NULL) {
	SetLayer(l);
}

Control::~Control() {
	children.clear();
}

Control* Control::clone() {
	return new Control(*this);
}

/**
 * Checks if the point is inside the controls quad
 * @param mx	the x value to check
 * @param my	the y value to check
 * @returns true if the hittest passed
 */
bool Control::HitTest(int mx, int my) {
	if (mx > x && my > y &&
		mx < x + width && my < y + height) {
		if (mouseOverChild != NULL) {
			if (mouseOverChild->HitTest(mx, my)) {
				return true;
			} else {
				mouseOverChild->OnMouseLeave();
			}
		}

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

	mouseOverChild = NULL;
	return false;
}

void Control::OnMousePress(unsigned short button, int mx, int my) {
	StartEvent("onMousePress");

	if (mouseOverChild != NULL) {
		activeChild = mouseOverChild;
		activeChild->OnMousePress(button, mx, my);
	} else {
		//if (activeChild != NULL)
		//activeChild->OnUnactivate();
		activeChild = NULL;
	}
}

void Control::OnMouseRelease(int button) { }

bool Control::OnMouseClick(unsigned short num, bool final) {
	StartEvent("onClick");

	if (activeChild != NULL) {
		return activeChild->OnMouseClick(num, final);
	}
	return false;
}

/**
 * Handels events when the mouse moves over the window
 * @param x the exact x position, in screen space
 * @param y the exact y position, in screen space
 * @param button the button that is pressed down
 * @returns false if nothing was handeled
 */
bool Control::OnMouseMotion(float x, float y, unsigned short button) {
	StartEvent("onDrag");

	if (activeChild != NULL)
		return activeChild->OnMouseMotion(x, y, button);
	return false;
}

void Control::OnKeyPress(unsigned short unicode) {
	StartEvent("onKey");
	//called by input
}

void Control::OnKeyRelease(int key, int mod) {
	EndEvent("onKey");
	//called by input
}

void Control::Move(float cx, float cy) {
	x += cx;
	y += cy;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->Move(cx, cy);
	}
}

/**
 * sets the position widgets position
 * @param cx the new x in screen space
 * @param cy the new y in screen space
 */
void Control::SetPosition(float cx, float cy) {
	x = cx;
	y = cy;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->SetPosition(cx, cy);
	}
}

void Control::SetEnabled(bool value) {
	isEnabled = value;
}

void Control::SetFocus(bool value) {
	hasFocus = value;
}

float Control::GetWidth() {
	return width;
}

float Control::GetHeight() {
	return height;
}

void Control::SetWidth(float w) {
	width = w;
}

void Control::SetHeight(float h) {
	height = h;
}

void Control::SetColor(util::Color c) {
	color = c;
}

void Control::SetColor(float r, float g, float b, float a) {
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void Control::AddColor(util::Color c) {
	color += c;
}

void Control::AddColor(float r, float g, float b, float a) {
	color.r += r;
	color.g += g;
	color.b += b;
	color.a += a;
}

util::Color Control::GetColor() {
	return color;
}

float* Control::GetColorv() {
	return color._array;
}

void Control::OnMouseEnter() {
	StartEvent("onHover");
}

void Control::OnMouseLeave() {
	EndEvent("onHover");
}

void Control::AddChild(Control* c) {
	children.push_back(c);
}

/**
 * Returns the total size of the control, 1 + TotalChildren
 */
unsigned int Control::Size() {
	return 1 + TotalChildren();
}

unsigned int Control::TotalChildren() {
	size_t size = children.size();
	unsigned int ret = size;
	for (unsigned int i = 0; i < size; i++)
		ret += children[i]->TotalChildren();

	return ret;
}

unsigned int Control::NumChildren() {
	return children.size();
}

Control* Control::GetChild(unsigned int num) {
	if (num < children.size()) {
		return children[num];
	} else {
		return NULL;
	}
}

Control* Control::IterateChild(unsigned int num) {
	if (NumChildren() == 0) {
		return NULL;
	} else {
		Control* root = NULL;
		Control* child = NULL;
		unsigned int j = 0;
		unsigned int z = 0;

		size_t size = NumChildren();
		for (unsigned int i = 0; i < size; i++) {
			root = children[i];
			child = root;
			z = 0;

			do {
				if (j == num)
					return child;

				child = root->IterateChild(z);
				j++;
				z++;
			} while (child != NULL);
		}
	}

	return NULL;
}

int Control::IterateChild(Control* c) {
	if (NumChildren() == 0) {
		return -1;
	} else if (c == this) {
		return 0;
	} else {
		Control* root = NULL;
		Control* child = NULL;
		unsigned int j = 0;
		unsigned int z = 0;

		size_t size = NumChildren();
		for (unsigned int i = 0; i < size; i++) {
			root = children[i];
			child = root;
			z = 0;

			do {
				if (child == c)
					return j;

				child = root->IterateChild(z);
				j++;
				z++;
			} while (child != NULL);
		}
	}

	return -1;
}

/**
 * Sets the Depth for this control, and all children.
 */
void Control::SetDepth(float depth) {
	this->z = depth;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->SetDepth(depth);
	}
}

/**
 * Adds to the depth of this control, and all of its children
 */
void Control::AddDepth(float depth) {
	this->z += depth;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->AddDepth(depth);
	}
}

void Control::SetLayer(LayerT l) {
	layer = l;
}

/**
 * returns a pointer to the roots rotation matrix, or NULL
 */
util::matrix4<float>* Control::GetRotation() {
	if (root == NULL)
		return NULL;

	return root->GetRotation();
}

/**
 * returns a float[16] containing the roots rotation matrix, or NULL
 */
float* Control::GetRotationfv() {
	if (root == NULL)
		return NULL;

	return root->GetRotationfv();
}

/**
 * Returns a const pointer to the Root window
 */
Window* Control::GetRoot() {
	return root;
}

void Control::SetRoot(Window* nr) {
	root = nr;
}

float Control::GetX() {
	return x;
}

float Control::GetY() {
	return y;
}

float Control::GetZ() {
	return z;
}

float Control::GetLayer() {
	return layer;
}

float Control::GetDepth() {
	return z + layer;
}

void Control::SetCallback(std::string name, Event* event) {
	//create a new event so we can safyly set the object
	GUIEvent* newEvent = static_cast<GUIEvent*> (event->clone());
	newEvent->SetObject(this);

	events.insert(std::pair<std::string, Event*>(name, newEvent));
}

void Control::SetCallbacks(std::multimap<std::string, Event*> c) {
	std::multimap<std::string, Event*>::iterator it;
	for (it = c.begin(); it != c.end(); it++) {
		SetCallback(it->first, it->second);
	}
}

void Control::StartEvent(std::string str) {
	//VALID (gui) EVENTS
	//onHover - called when the mouse is over the object
	//onKeyPress - called when a key is pressed
	//onActivate -- called when the window is made active (clicked)
	//onHide -- called when the window is hidden
	//onShow -- called when the window is made visible
	//onClick -- called when the user clicks the object
	//onResize -- called when resized;

	if (events.count(str) > 0) {
		std::multimap<std::string, Event*>::iterator it;
		for (it = events.equal_range(str).first; it != events.equal_range(str).second; ++it) {
			it->second->Begin();
			root->AddEvent(it->second);
		}
	}
}

void Control::EndEvent(std::string str) {
	if (events.count(str) > 0) {
		std::multimap<std::string, Event*>::iterator it;
		for (it = events.equal_range(str).first; it != events.equal_range(str).second; ++it) {
			it->second->End();
			root->RemoveEvent(it->second);
		}
	}
}
