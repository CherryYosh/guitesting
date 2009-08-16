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
#ifndef WIDGET_H
#define WIDGET_H

#include "../utils/color.h"
#include "../utils/matrix.h"
#include "../utils/vector.h"
#include "../events/event.h"

#include <map>
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class Window;

#define GUI_NONE 0
#define GUI_HAS_TEXT 1
#define GUI_RECIEVE_KEYS 2
#define GUI_RECIEVE_MOUSE 4
#define GUI_CLICKABLE 8
#define GUI_CONTAINER 16
#define GUI_ALL ~0

enum ResizeConstraintT {
	RESIZE_NONE, RESIZE_ALL, RESIZE_HORIZONTAL, RESIZE_VERTICAL
};

enum MovementConstrainT {
	MOVEMENT_NONE, MOVEMENT_ALL, MOVEMENT_HORIZONTAL, MOVEMENT_VERTICAL
};

enum LayerT {
	BACKGROUND_LAYER, BOTTOM_LAYER, DEFAULT_LAYER, TOP_LAYER
};

/**
* the basic Widget class of all GUI objects
*/
class Widget {
public:
	static Widget* NewWidget(std::string, std::string, int = 0, int = 0, LayerT = DEFAULT_LAYER, ResizeConstraintT = RESIZE_ALL, MovementConstrainT  = MOVEMENT_NONE);

	Widget();
	Widget(long, Window* = NULL);
	virtual ~Widget();

	virtual Widget* clone();

	bool attributes(long);
	long attributes();
	std::vector<Widget*> GetChildrenWith(long);

	virtual bool ContainsPoint(int, int);
	virtual bool MouseTest(int, int);

	virtual void OnMousePress(unsigned short, int, int);
	virtual void OnMouseRelease(int);
	virtual bool OnMouseClick(unsigned short, bool);
	virtual bool OnMouseMotion(float, float, unsigned short);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();

	virtual void OnKeyPress(unsigned short, int, int);
	virtual void OnKeyRelease(int, int);

	virtual void Move(float, float);
	virtual void SetPosition(float, float);

	void SetColor(util::Color);
	void SetColor(float, float, float, float);
	void AddColor(util::Color);
	void AddColor(float, float, float, float);
	util::Color GetColor();
	float* GetColorv();

	virtual void AddChild(Widget*);
	virtual Widget* GetChild(unsigned int);
	virtual Widget* IterateChildren(unsigned int);
	virtual int IterateChildren(Widget*);

	virtual unsigned int Size();
	virtual unsigned int TotalChildren();
	virtual unsigned int ChildrenSize();

	virtual float GetWidth();
	virtual float GetHeight();
	virtual void SetWidth(std::string);
	virtual void SetWidth(float);
	virtual void SetHeight(std::string);
	virtual void SetHeight(float);

	virtual void SetZ(float);
	virtual void AddZ(float);
	virtual void SetLayer(LayerT);

	util::matrix4<float>* GetRotation();
	float* GetRotationfv();

	std::string GetType();
	void SetType(std::string);

	std::string GetBackground();
	void SetBackground(std::string);

	Window* GetRoot();
	void SetRoot(Window*);
	Widget* GetParent();
	void SetParent(Widget*);

	virtual void ReloadTheme();

	float GetX();
	float GetY();
	float GetZ();
	float GetLayer();
	float GetDepth();

	void AddEvent(std::string, Event*);
	void SetEvents(std::multimap<std::string, Event*>);

	void StartEvent(std::string);
	void EndEvent(std::string);

	ResizeConstraintT GetResizeConstraint();
	void SetResizeConstraint(ResizeConstraintT);

	bool CanReleaseMouse();
	void LockMouse();
	void ReleaseMouse();

	void SetSize(int, int);
	void AdjustSize(int, int);

	float s, t, s2, t2;
protected:
	Window* root;
	Widget* parent;
	Widget* mouseOverChild;
	Widget* activeChild;

	std::vector<Widget*> children;
	std::multimap<std::string, Event*> events;

	float width, height;
	float x, y, z;
	float layer;

	long attributes_;
	util::Color color;
private:
	std::string type;
	std::string background;
	bool canReleaseMouse;

	ResizeConstraintT resize;
	MovementConstrainT movement;
};
#endif
