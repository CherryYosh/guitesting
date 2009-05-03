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

#include "../nvVector.h"

#include <GL/gl.h>
#include <GL/glext.h>

#include <vector>
#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>

class Window;

struct CTRL_GUIDataT {
	std::string type;
	float s, t, s2, t2; //the uv coords
	float width, height;
};

//the basic control class of all GUI objects

class Control {
public:
	Control(std::string, Window*, Control* = NULL, float = 0, float = 0);
	virtual ~Control();

	virtual void Activate(); //called when ever the control becomes active

	virtual bool HitTest(int, int);

	virtual void OnMousePress(unsigned short, int, int);
	virtual void OnMouseRelease(int);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual bool OnMouseClick(unsigned short, bool);

	virtual void OnKeyPress(unsigned short);
	virtual void OnKeyRelease(int, int);

	virtual void Move(float, float);
	virtual void SetCallback(boost::function<void()>);
	virtual void SetWidth(float);
	virtual void SetHeight(float);
	virtual bool HasAttrib(unsigned short);
	virtual void SetEnabled(bool);
	virtual void SetFocus(bool);

	virtual void SetColor(float, float, float, float);
	virtual void SetColor(nv::vec4<float>);
	virtual void AddColor(nv::vec4<float>);
	virtual float* GetColorv();

	virtual void AddChild(Control*);
	virtual Control* GetChild( unsigned int);
	virtual unsigned int TotalChildren();
	virtual unsigned int NumChildren();


	virtual float GetWidth();
	virtual float GetHeight();

	unsigned int VertexOffset;

	float Width, Height;
	float x, y;
	float s, t, s2, t2;
protected:
	unsigned short Attributes;
	Window* Root;
	Control* Parent;
	Control* MouseOverChild;
	Control* ActiveChild;
	std::vector<Control*> Children;
	boost::function<void() > m_Callback;
private:
	void GetControlData();

	bool hasFocus, isEnabled;
	nv::vec4<float> Color;

	std::string Type;
};

void Control_Init(const char*);
#endif
