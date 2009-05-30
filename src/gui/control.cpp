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

//This should only be called when a new theme has been loaded or when the control is initlized

void Control::GetControlData() {
}

Control::Control(std::string t, Window* r, Control* p, LayerT l, float ix, float iy) {
	isEnabled = true;
	hasFocus = false;
	Type = t;
	x = ix;
	y = iy;
	z = -TOP_LAYER;
	SetLayer(l);

	SetColor(util::vec4<float>(0.0));

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

void Control::OnActivate() { }

void Control::OnUnactivate() {
	HitTest(5,5);
}

/**
 * Checks if the point is inside the controls quad
 * @param mx	the x value to check
 * @param my	the y value to check
 * @returns true if the hittest passed
 */
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

		unsigned int i = Children.size();
		while (0 < i) {
			i--;
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
	} else {
		if (ActiveChild != NULL)
			ActiveChild->OnUnactivate();
		ActiveChild = NULL;
	}
}

void Control::OnMouseRelease(int button) { }

bool Control::OnMouseClick(unsigned short num, bool final) {
	if (ActiveChild != NULL) {
		return ActiveChild->OnMouseClick(num, final);
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
	if (ActiveChild != NULL)
		return ActiveChild->OnMouseMotion(x, y, button);
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

	size_t size = Children.size();
	for (unsigned int i = 0; i < size; i++) {
		Children[i]->Move(cx, cy);
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

	size_t size = Children.size();
	for (unsigned int i = 0; i < size; i++) {
		Children[i]->SetPosition(cx, cy);
	}
}

void Control::SetEnabled(bool value) {
	isEnabled = value;
}

void Control::SetFocus(bool value) {
	hasFocus = value;
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

void Control::SetWidth(float w) {
	Width = w;
}

void Control::SetHeight(float h) {
	Height = h;
}

void Control::SetColor(util::Color c){
	color = c;
}

void Control::SetColor(float r, float g, float b, float a) {
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
}

void Control::SetColor(util::vec4<float> c) {
	//Color = c;
}

void Control::AddColor(util::vec4<float> c) {
	//Color += c;
}

util::Color Control::GetColor(){
	return color;
}

float* Control::GetColorv() {
	return color._array;
}

void Control::OnMouseEnter() { }

void Control::OnMouseLeave() { }

void Control::AddChild(Control* c) {
	Children.push_back(c);
}

/**
 * Returns the total size of the control, 1 + TotalChildren
 */
unsigned int Control::Size() {
	return 1 + TotalChildren();
}

unsigned int Control::TotalChildren() {
	size_t size = Children.size();
	unsigned int ret = size;
	for (unsigned int i = 0; i < size; i++)
		ret += Children[i]->TotalChildren();

	return ret;
}

unsigned int Control::NumChildren() {
	return Children.size();
}

Control* Control::GetChild(unsigned int num) {
	if (num < Children.size()) {
		return Children[num];
	} else {
		return NULL;
	}
}

Control* Control::IterateChild(unsigned int num) {
	if (IsLeaf()) {
		return NULL;
	} else {
		Control* root = NULL;
		Control* child = NULL;
		unsigned int j = 0;
		unsigned int z = 0;

		size_t size = NumChildren();
		for (unsigned int i = 0; i < size; i++) {
			root = Children[i];
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
	if (IsLeaf()) {
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
			root = Children[i];
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

bool Control::IsRoot() {
	return false; //only windows can be roots
}

bool Control::IsLeaf() {
	return(0 == NumChildren());
}

/**
 * Sets the Depth for this control, and all children.
 */
void Control::SetDepth(float depth) {
	this->z = depth;

	size_t size = Children.size();
	for (unsigned int i = 0; i < size; i++) {
		Children[i]->SetDepth(depth);
	}
}

/**
 * Adds to the depth of this control, and all of its children
 */
void Control::AddDepth(float depth) {
	this->z += depth;

	size_t size = Children.size();
	for (unsigned int i = 0; i < size; i++) {
		Children[i]->AddDepth(depth);
	}
}

void Control::SetLayer(LayerT l) {
	layer = l;
}

/**
 * returns a pointer to the roots rotation matrix, or NULL
 */
util::matrix4<float>* Control::GetRotation() {
	if (Root == NULL)
		return NULL;

	return Root->GetRotation();
}

/**
 * returns a float[16] containing the roots rotation matrix, or NULL
 */
float* Control::GetRotationfv() {
	if (Root == NULL)
		return NULL;

	return Root->GetRotationfv();
}

/**
 * Returns a const pointer to the Root window
 */
Window* Control::GetRoot() {
	return Root;
}

void Control::SetRoot(Window* nr){
	Root = nr;
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

void Control::SetCallback(std::string name, Event* event){
	//create a new event so we can safyly set the object
	Event* newEvent = event->clone();
	newEvent->SetObject(this);

	Callbacks[name] = newEvent;
}

void Control::SetCallbacks(std::map<std::string, Event*> c){
	std::map<std::string, Event*>::iterator it;
	for(it = c.begin(); it != c.end(); it++){
		SetCallback(it->first, it->second);
	}
}
