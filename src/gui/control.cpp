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
#include "label.h"
#include "../events/gui/guievent.h"
#include "controls.h"
#include "../theme.h"

Control::Control() : _attributes(GUI_NONE), x(0), y(0), z(-TOP_LAYER), color(), canReleaseMouse(true),
parent(NULL), root(NULL), mouseOverChild(NULL), activeChild(NULL), orientation(All), movement(3) { }

Control::Control(long t, Window* r, Control* p, LayerT l, float ix, float iy) : _attributes(t), x(ix), y(iy),
z(-TOP_LAYER), color(), parent(p), root(r), mouseOverChild(NULL), activeChild(NULL), orientation(All), canReleaseMouse(true) {
    SetLayer(l);
}

Control::~Control() {
    children.clear();
}

Control* Control::clone() {
    return new Control(*this);
}

/**
 * returns true if all of the given flags are enabled
 */
bool Control::attributes(long flags) {
    return((_attributes & flags) == flags);
}

long Control::attributes() {
    return _attributes;
}

bool Control::Contains(int tx, int ty) {
    if (tx > x && ty > y &&
	    tx < (x + width) && ty < (y + height)) {
	return true;
    }
    return false;
}

/**
 * Checks if the mouse is inside this control
 * @param mx	the x value to check
 * @param my	the y value to check
 * @returns true if the hittest passed
 */
bool Control::MouseTest(int mx, int my) {
    if (this->Contains(mx, my)) {

	if (mouseOverChild != NULL) {
	    if (mouseOverChild->MouseTest(mx, my)) {
		return true;
	    } else {
		if (mouseOverChild->CanReleaseMouse()) {
		    mouseOverChild->ReleaseMouse();
		    mouseOverChild->OnMouseLeave();
		} else {
		    return false;
		}
	    }
	}

	unsigned int i = children.size();
	while (0 < i) {
	    i--;
	    if (children[i] != mouseOverChild && children[i]->MouseTest(mx, my)) {
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

void Control::OnMousePress(unsigned short button, int mx, int my) {
    StartEvent("onMousePress");
    LockMouse();

    if (mouseOverChild != NULL) {
	activeChild = mouseOverChild;
	activeChild->OnMousePress(button, mx, my);
    } else {
	activeChild = NULL;
    }
}

void Control::OnMouseRelease(int button) {
    EndEvent("onMousePress");
    ReleaseMouse();
}

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

void Control::OnKeyPress(unsigned short unicode, int key, int mod) {
    StartEvent("onKey");
}

void Control::OnKeyRelease(int key, int mod) {
    EndEvent("onKey");
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

float Control::GetWidth() {
    return width;
}

float Control::GetHeight() {
    return height;
}

/**
 * Takes a string like %75 and sets the control to 75% of the iner width
 */
void Control::SetWidth(std::string s) {
    //TODO: try / catch on the atof incase the value is invalid.
    if (s[0] == '%') {
	float f = atof(s.substr(1, std::string::npos).c_str()) / 100.0;
	if (f > 1.0) f = 1.0;

	SetWidth(root->GetInternalWidth() * f);
    } else {
	SetWidth(atof(s.c_str())); //LUA might call this SetWidth "180" thinking I want a string, so convert to a float
    }
}

void Control::SetHeight(std::string s) {
    if (s[0] == '%') {
	float f = atof(s.substr(1, std::string::npos).c_str()) / 100.0;
	if (f > 1.0) f = 1.0;

	SetHeight(root->GetInternalWidth() * f);
    } else {
	SetHeight(atof(s.c_str())); //LUA might call this SetHeight "180" thinking I want a string, so convert to a float
    }
}

void Control::SetWidth(float w) {
    if (w > 0) width = w;
}

void Control::SetHeight(float h) {
    if (h > 0) height = h;
}

void Control::SetColor(util::Color c) {
    color = c;
}

void Control::Resize(int w, int h) {
    //Control* obj = (parent != NULL) ? parent : root; //see if this causes problems..

    size_t size = children.size();
    for (size_t i = 0; i < size; i++) {
	children[i]->Resize(w, h);
    }

    float xdelta = 0;
    float ydelta = 0;

    if (movement == 1 || movement == 3) xdelta = (GetX() - root->GetInternalX()) / root->GetInternalWidth() * w;
    if (movement == 2 || movement == 3) (GetY() - root->GetInternalY()) / root->GetInternalHeight() * h;
    Move(xdelta, ydelta);

    if (GetOrientation() == All || GetOrientation() == Vertical) {
	float wdelta = GetWidth() / root->GetInternalWidth() * w;
	SetWidth(GetWidth() + wdelta);
    }

    if (GetOrientation() == All || GetOrientation() == Horizontal) {
	float hdelta = (GetHeight() / root->GetInternalHeight()) * h;
	SetHeight(GetHeight() + hdelta);
    }

    StartEvent("onResize");
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

Control * Control::NewChild(std::string type, float x, float y, LayerT layer, OrientationT ori) {
    Control* ret;
    __M_ControlCast(ret, type.substr(0, type.find('.')));

    ret->SetName(type);
    ret->SetPosition(x, y);
    ret->SetLayer(layer);
    ret->SetOrientation(ori);
    ret->ReloadTheme();

    AddChild(ret);
    return ret;
}

void Control::AddChild(Control * c) {
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
    for (size_t i = 0; i < size; i++)
	ret += children[i]->TotalChildren();

    return ret;
}

unsigned int Control::NumChildren() {
    return children.size();
}

Control * Control::GetChild(unsigned int num) {
    if (num < children.size()) {
	return children[num];
    } else {
	return NULL;
    }
}

Control * Control::IterateChild(unsigned int num) {
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

int Control::IterateChild(Control * c) {
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
    return root->GetRotation();
}

/**
 * returns a float[16] containing the roots rotation matrix, or NULL
 */
float* Control::GetRotationfv() {
    return root->GetRotationfv();
}

/**
 * Returns a const pointer to the Root window
 */
Window * Control::GetRoot() {
    return root;
}

void Control::SetRoot(Window * nr) {
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

/**
 * Returns the depth (z + layer) of the control
 */
float Control::GetDepth() {
    return z + layer;
}

void Control::AddCallback(std::string name, Event * event) {
    //create a new event so we can safyly set the object
    GUIEvent* newEvent = static_cast<GUIEvent*> (event->clone());
    newEvent->SetObject(this);

    events.insert(std::pair<std::string, Event*>(name, newEvent));
}

void Control::SetCallbacks(std::multimap<std::string, Event*> c) {
    std::multimap<std::string, Event*>::iterator it;
    for (it = c.begin(); it != c.end(); it++) {
	AddCallback(it->first, it->second);
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

/**
 * returns all children with the given flags
 */
std::vector<Control*> Control::GetChildrenWith(long flags) {
    std::vector<Control*> ret;
    std::vector<Control*> temp;

    if (attributes(flags)) {
	ret.push_back(this);
    }

    size_t size = children.size();
    for (unsigned int i = 0; i < size; i++) {
	temp = children[i]->GetChildrenWith(flags);
	for (std::vector<Control*>::iterator it = temp.begin(); it != temp.end(); it++) {
	    ret.push_back(*it);
	}
    }

    return ret;
}

/**
 * TODO: fix for resizing in .xml
 */
void Control::ReloadTheme() {
    ThemeData data = Theme::GetData(name);

    SetWidth(data.width);
    SetHeight(data.height);

    s = data.s / float(Theme::ImageWidth());
    t = 1.0 - data.t / float(Theme::ImageHeight());
    s2 = float(data.s + data.width) / float(Theme::ImageWidth());
    t2 = 1.0 - float((data.t + data.height)) / float(Theme::ImageHeight());
}

std::string Control::GetName() {
    return name;
}

void Control::SetName(std::string n) {
    name = n;
}

OrientationT Control::GetOrientation() {
    return orientation;
}

void Control::SetOrientation(OrientationT t) {
    orientation = t;
}

/*TODO: Refactor?? */
void Control::SetMovementFlags(std::string s) {
    if (s == "none") movement = 0;
    else if (s == "x") movement = 1;
    else if (s == "y") movement = 2;
    else if (s == "both") movement = 3;
    else throw std::invalid_argument("Control::SetMovementFlags::Invalid_Flags");
}

bool Control::CanReleaseMouse() {
    return canReleaseMouse;
}

void Control::LockMouse() {
    canReleaseMouse = false;
}

void Control::ReleaseMouse() {
    canReleaseMouse = true;
}