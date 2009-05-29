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

#define CTRL_INPUT (1) << 0 //does the control allow input

#include "../nvMatrix.h"
#include "../nvVector.h"
#include "../events/event.h"

#include <GL/gl.h>
#include <GL/glext.h>

#include <map>
#include <vector>
#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class Window;

enum LayerT{ BOTTOM_LAYER, DEFAULT_LAYER, TOP_LAYER };
//the basic control class of all GUI objects

class Control {
public:
	Control(std::string, Window*, Control* = NULL, LayerT = DEFAULT_LAYER, float = 0, float = 0);
	virtual ~Control();

	virtual void OnActivate();
	virtual void OnUnactivate();

	virtual bool HitTest(int, int);

	virtual void OnMousePress(unsigned short, int, int);
	virtual void OnMouseRelease(int);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual bool OnMouseClick(unsigned short, bool);
	virtual bool OnMouseMotion(float, float, unsigned short);

	virtual void OnKeyPress(unsigned short);
	virtual void OnKeyRelease(int, int);

	virtual void Move(float, float);
	virtual void SetPosition(float, float);
	virtual bool HasAttrib(unsigned short);
	virtual void SetEnabled(bool);
	virtual void SetFocus(bool);

	virtual void SetColor(float, float, float, float);
	virtual void SetColor(nv::vec4<float>);
	virtual void AddColor(nv::vec4<float>);
	virtual nv::vec4<float> GetColor();
	virtual float* GetColorv();

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

	virtual bool IsRoot();
	virtual bool IsLeaf();

	virtual void SetDepth(float);
	virtual void AddDepth(float);
	virtual void SetLayer(LayerT);

	virtual const Window* GetRoot();
	virtual nv::matrix4<float>* GetRotation();
	virtual float* GetRotationfv();

	float GetX();
	float GetY();
	float GetZ();
	float GetLayer();
	float GetDepth();

	void SetCallback(std::string, Event*);
	void SetCallbacks(std::map<std::string, Event*>);

	int tc();

	float s, t, s2, t2;
protected:
	unsigned short Attributes;
	Window* Root;
	Control* Parent;
	Control* MouseOverChild;
	Control* ActiveChild;
	std::vector<Control*> Children;
	std::map<std::string, Event*> Callbacks;

	float Width, Height;
	float x, y, z;
	float layer;
private:
	void GetControlData();

	bool hasFocus, isEnabled;
	nv::vec4<float> Color;

	std::string Type;
};
#endif
