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
#ifndef CONTROL_H
#define CONTROL_H

#include "../utils/color.h"
#include "../utils/matrix.h"
#include "../utils/vector.h"
#include "../events/event.h"

#include <GL/gl.h>
#include <GL/glext.h>

#include <map>
#include <vector>
#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class Window;
class Label;

enum TypeE{ ControlType, WindowType, ButtonType, RuleType, EditboxType, LabelType, CheckboxType, SliderType };
enum LayerT{ BOTTOM_LAYER, DEFAULT_LAYER, TOP_LAYER };

/**
 * the basic control class of all GUI objects
 */
class Control {
public:
	Control();
	Control(const Control&);
	Control(TypeE, Window*, Control* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
	virtual ~Control();

	virtual Control* clone();
        virtual TypeE type();

	virtual bool HitTest(int, int);

	virtual void OnMousePress(unsigned short, int, int);
	virtual void OnMouseRelease(int);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual bool OnMouseClick(unsigned short, bool);
	virtual bool OnMouseMotion(float, float, unsigned short);

	virtual void OnKeyPress(unsigned short, int, int);
	virtual void OnKeyRelease(int, int);

	virtual void Move(float, float);
	virtual void SetPosition(float, float);

	virtual void SetEnabled(bool);
	virtual void SetFocus(bool);

	void SetColor(util::Color);
	void SetColor(float, float, float, float);
	void AddColor(util::Color);
	void AddColor(float, float, float, float);

	util::Color GetColor();
	float* GetColorv();

	virtual void AddChild(Control*);
	virtual Control* GetChild(unsigned int);
	virtual Control* IterateChild(unsigned int);
	virtual int IterateChild(Control*);

	virtual unsigned int Size();
	virtual unsigned int TotalChildren();
	virtual unsigned int NumChildren();

	virtual float GetWidth();
	virtual float GetHeight();
	virtual void SetWidth(float);
	virtual void SetHeight(float);

	virtual void SetDepth(float);
	virtual void AddDepth(float);
	virtual void SetLayer(LayerT);

	util::matrix4<float>* GetRotation();
	float* GetRotationfv();

	virtual Window* GetRoot();
	void SetRoot(Window*);

	float GetX();
	float GetY();
	float GetZ();
	float GetLayer();
	float GetDepth();

	void SetCallback(std::string, Event*);
	void SetCallbacks(std::multimap<std::string, Event*>);
	
	void StartEvent(std::string);
	void EndEvent(std::string);

        std::vector<Label*> GetTextObjs();

	float s, t, s2, t2;
protected:
	Window* root;
	Control* parent;
	Control* mouseOverChild;
	Control* activeChild;

	std::vector<Control*> children;
	std::multimap<std::string, Event*> events;

	float width, height;
	float x, y, z;
	float layer;

        TypeE _type;
        util::Color color;
private:
	bool hasFocus, isEnabled;
};
#endif
