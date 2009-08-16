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

#include "widget.h"

#include "window.h"
#include "label.h"
#include "../events/gui/guievent.h"
#include "widgets.h"
#include "../theme.h"

Widget* Widget::NewWidget(std::string t, std::string bg, int x, int y, LayerT l, ResizeConstraintT rc, MovementConstrainT mc){
	Widget* ret;
	__M_WIDGETCAST(ret, t);

	ret->SetType(t);
	ret->SetBackground(bg);
	ret->SetLayer(l);
	ret->SetPosition(x, y);
	ret->SetResizeConstraint(rc);
	ret->ReloadTheme();

	return ret;
}


Widget::Widget() : attributes_(GUI_NONE), x(0), y(0), z(-TOP_LAYER), color(), canReleaseMouse(true),
parent(NULL), root(NULL), mouseOverChild(NULL), activeChild(NULL), resize(RESIZE_ALL), movement(MOVEMENT_ALL) { }

Widget::Widget(long attr, Window* r) : attributes_(attr), x(0), y(0), z(-TOP_LAYER), color(), 
parent(NULL), root(r), mouseOverChild(NULL), activeChild(NULL), resize(RESIZE_ALL), movement(MOVEMENT_ALL), canReleaseMouse(true) {
}

Widget::~Widget() {
	children.clear();
}

Widget* Widget::clone() {
	return new Widget(*this);
}

/**
* returns true if all of the given flags are enabled
*/
bool Widget::attributes(long flags) {
	return((attributes_ & flags) == flags);
}

long Widget::attributes() {
	return attributes_;
}

bool Widget::ContainsPoint(int tx, int ty) {
	if (tx > x && ty > y &&
		tx < (x + width) && ty < (y + height)) {
			return true;
	}
	return false;
}

/**
* Checks if the mouse is inside this Widget
* @param mx	the x value to check
* @param my	the y value to check
* @returns true if the hittest passed
*/
bool Widget::MouseTest(int mx, int my) {
	if (this->ContainsPoint(mx, my)) {

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

void Widget::OnMousePress(unsigned short button, int mx, int my) {
	StartEvent("onMousePress");
	LockMouse();

	if (mouseOverChild != NULL) {
		activeChild = mouseOverChild;
		activeChild->OnMousePress(button, mx, my);
	} else {
		activeChild = NULL;
	}
}

void Widget::OnMouseRelease(int button) {
	ReleaseMouse();
	EndEvent("onMousePress");
}

bool Widget::OnMouseClick(unsigned short num, bool final) {
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
bool Widget::OnMouseMotion(float x, float y, unsigned short button) {
	StartEvent("onDrag");

	if (activeChild != NULL)
		return activeChild->OnMouseMotion(x, y, button);
	return false;
}

void Widget::OnKeyPress(unsigned short unicode, int key, int mod) {
	StartEvent("onKey");
}

void Widget::OnKeyRelease(int key, int mod) {
	EndEvent("onKey");
}

void Widget::Move(float cx, float cy) {
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
void Widget::SetPosition(float cx, float cy) {
	x = cx;
	y = cy;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->SetPosition(cx, cy);
	}
}

float Widget::GetWidth() {
	return width;
}

float Widget::GetHeight() {
	return height;
}

/**
* Takes a string like %75 and sets the Widget to 75% of the iner width
*/
void Widget::SetWidth(std::string s) {
	//TODO: try / catch on the atof incase the value is invalid.
	if (s[0] == '%') {
		float f = atof(s.substr(1, std::string::npos).c_str()) / 100.0;
		if (f > 1.0) f = 1.0;

		if(root != NULL)
		    SetWidth(root->InternalWidth() * f);
	} else {
		SetWidth(atof(s.c_str())); //LUA might call this SetWidth "180" thinking I want a string, so convert to a float
	}
}

void Widget::SetHeight(std::string s) {
	if (s[0] == '%') {
		float f = atof(s.substr(1, std::string::npos).c_str()) / 100.0;
		if (f > 1.0) f = 1.0;

		SetHeight(root->InternalWidth() * f);
	} else {
		SetHeight(atof(s.c_str())); //LUA might call this SetHeight "180" thinking I want a string, so convert to a float
	}
}

void Widget::SetWidth(float w) {
	if (w > 0) width = w;
}

void Widget::SetHeight(float h) {
	if (h > 0) height = h;
}

void Widget::SetColor(util::Color c) {
	color = c;
}

void Widget::SetColor(float r, float g, float b, float a) {
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void Widget::AddColor(util::Color c) {
	color += c;
}

void Widget::AddColor(float r, float g, float b, float a) {
	color.r += r;
	color.g += g;
	color.b += b;
	color.a += a;
}

util::Color Widget::GetColor() {
	return color;
}

float* Widget::GetColorv() {
	return color._array;
}

void Widget::OnMouseEnter() {
	StartEvent("onHover");
}

void Widget::OnMouseLeave() {
	EndEvent("onHover");
}

void Widget::AddChild(Widget * c) {
	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		if (children[i]->ContainsPoint(c->GetX(), c->GetY())) {
			children[i]->AddChild(c);
			return;
		}
	}

	c->SetParent(this);
	c->SetRoot(this->GetRoot());
	children.push_back(c);
}

/**
* Returns the total size of the Widget, 1 + TotalChildren
*/
unsigned int Widget::Size() {
	return 1 + TotalChildren();
}

unsigned int Widget::TotalChildren() {
	size_t size = children.size();
	unsigned int ret = size;
	for (size_t i = 0; i < size; i++)
		ret += children[i]->TotalChildren();

	return ret;
}

unsigned int Widget::ChildrenSize() {
	return children.size();
}

Widget * Widget::GetChild(unsigned int num) {
	if (num < children.size()) {
		return children[num];
	} else {
		return NULL;
	}
}

Widget * Widget::IterateChildren(unsigned int num){
	if(num == 0) throw std::invalid_argument("Widget::IterateChildren::ZERO_NOT_VALID");
	if(children.size() == 0) return NULL; //quick escape, would happen anyways

	size_t size;
	Widget *root, *node;
	int i = 0, j = 0, l = 1;

	size = children.size();
	for(i = 0; i < size; i++){
		root = node = children[i];
		j = 0;

		do {
			if(l == num) 
				return node;

			j++;
			l++;
			node = root->IterateChildren(j);
		} while(node != NULL);
	}

	return NULL;
}

int Widget::IterateChildren(Widget* w){
	if(w == this) return 0;
	if(children.size() == 0) return -1;

	size_t size;
	Widget *root, *node;
	int j = 0, l = 0;

	size = children.size();
	for(size_t i = 0; i < size; i++){
		root = node = children[i];
		j = 0;

		do{
			if(node == w) 
				return l;

			l++;
			j++;
			node = root->IterateChildren(j);
		} while(node != NULL);
	}

	return -1;
}

/**
* Sets the Z for this Widget, and all children.
*/
void Widget::SetZ(float depth) {
	this->z = depth;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->SetZ(depth);
	}
}

/**
* Adds to the Z of this Widget, and all of its children
*/
void Widget::AddZ(float depth) {
	this->z += depth;

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		children[i]->AddZ(depth);
	}
}

void Widget::SetLayer(LayerT l) {
	layer = l;
}

/**
* returns a pointer to the roots rotation matrix, or NULL
*/
util::matrix4<float>* Widget::GetRotation() {
	return root->GetRotation();
}

/**
* returns a float[16] containing the roots rotation matrix, or NULL
*/
float* Widget::GetRotationfv() {
	return root->GetRotationfv();
}

/**
* Returns a const pointer to the Root window
*/
Window * Widget::GetRoot() {
	return root;
}

void Widget::SetRoot(Window * nr) {
	root = nr;
}

Widget* Widget::GetParent(){
	return parent;
}

void Widget::SetParent(Widget* w){
	parent = w;
}

float Widget::GetX() {
	return x;
}

float Widget::GetY() {
	return y;
}

float Widget::GetZ() {
	return z;
}

float Widget::GetLayer() {
	return layer;
}

/**
* Returns the depth (z + layer) of the Widget
*/
float Widget::GetDepth() {
	return z + layer;
}

void Widget::AddEvent(std::string name, Event * event) {
	//create a new event so we can safyly set the object
	GUIEvent* newEvent = static_cast<GUIEvent*> (event->clone());
	newEvent->SetObject(this);

	events.insert(std::pair<std::string, Event*>(name, newEvent));
}

void Widget::SetEvents(std::multimap<std::string, Event*> c) {
	events.clear();

	std::multimap<std::string, Event*>::iterator it;
	for (it = c.begin(); it != c.end(); it++) {
		AddEvent(it->first, it->second);
	}
}

void Widget::StartEvent(std::string str) {
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
			root->ActivateEvent(it->second);
		}
	}
}

void Widget::EndEvent(std::string str) {
	if (events.count(str) > 0) {
		std::multimap<std::string, Event*>::iterator it;
		for (it = events.equal_range(str).first; it != events.equal_range(str).second; ++it) {
			it->second->End();
			root->DeactivateEvent(it->second);
		}
	}
}

/**
* returns all children with the given flags
*/
std::vector<Widget*> Widget::GetChildrenWith(long flags) {
	std::vector<Widget*> ret;
	std::vector<Widget*> temp;

	if (attributes(flags)) {
		ret.push_back(this);
	}

	size_t size = children.size();
	for (unsigned int i = 0; i < size; i++) {
		temp = children[i]->GetChildrenWith(flags);

		for (std::vector<Widget*>::iterator it = temp.begin(); it != temp.end(); it++) {
			ret.push_back(*it);
		}
	}

	return ret;
}

void Widget::ReloadTheme() {
	ThemeData data = Theme::GetData(background);

	SetWidth(data.width);
	SetHeight(data.height);

	s = data.s / float(Theme::ImageWidth());
	t = 1.0 - data.t / float(Theme::ImageHeight());
	s2 = float(data.s + data.width) / float(Theme::ImageWidth());
	t2 = 1.0 - float((data.t + data.height)) / float(Theme::ImageHeight());
}

std::string Widget::GetType() {
	return type;
}

void Widget::SetType(std::string t) {
	type = t;
}

std::string Widget::GetBackground(){
	return background;
}

void Widget::SetBackground(std::string bg){
	background = bg;
}

ResizeConstraintT Widget::GetResizeConstraint() {
	return resize;
}

void Widget::SetResizeConstraint(ResizeConstraintT t) {
	resize = t;
}

bool Widget::CanReleaseMouse() {
	return canReleaseMouse;
}

void Widget::LockMouse() {
	canReleaseMouse = false;
}

void Widget::ReleaseMouse() {
	canReleaseMouse = true;
}

void Widget::SetSize(int w, int h){
    if(w < 0 || h < 0) throw std::invalid_argument("Width or height is negitave");

    float wp, hp;
    size_t size = children.size();
    for(size_t i = 0; i < size; i++){
		wp = (children[i]->GetResizeConstraint() == RESIZE_ALL || children[i]->GetResizeConstraint() == RESIZE_VERTICAL) ? children[i]->GetWidth() / GetWidth() : 0;
		hp = (children[i]->GetResizeConstraint() == RESIZE_ALL || children[i]->GetResizeConstraint() == RESIZE_HORIZONTAL) ? children[i]->GetHeight() / GetHeight() : 0;

		printf("Resizing: %i %f %f, %f %f, %f %f\nfinal: %f %f\n", i, wp, hp, GetWidth(), GetHeight(), children[i]->GetWidth(), children[i]->GetHeight(), w * wp, h*hp);

		children[i]->SetSize(w * wp, h * hp);
    }

	SetWidth(w);
	SetHeight(h);
}

void Widget::AdjustSize(int w, int h){
	SetSize(GetWidth() + w, GetHeight() + h);
}