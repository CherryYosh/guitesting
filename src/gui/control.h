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

#define CTRL_INPUT 1 //does the control allow input

#ifndef NULL
#define NULL 0
#endif //null

#include "../nvVector.h"

#include <GL/gl.h>
#include <GL/glext.h>

#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>

//#include "window.h"
class Window;

struct CTRL_GUIDataT{
	std::string type;
	float s,t, s2,t2; //the uv coords
	float width, height;
};

//the basic control class of all GUI objects
class Control{
public:
	Control( Window*, std::string, float, float );
	virtual ~Control();
	virtual void Activate(); //called when ever the control becomes active

	virtual bool HitTest( int, int );

	virtual void OnMousePress( int );
	virtual void OnMouseRelease( int );
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();

	virtual void OnKeyPress( unsigned short );
	virtual void OnKeyRelease( int, int );

	virtual void Move( float, float );
	virtual void SetCallback( boost::function<int()> callback );
	virtual void SetWidth( float );
	virtual void SetHeight( float );
	virtual bool HasAttrib( unsigned short );
	virtual void SetEnabled( bool );
	virtual void SetFocus( bool );
	virtual void SetDepth( float );

	virtual void SetColor( float, float, float, float );
	virtual void SetColor( nv::vec4<float> );
	virtual void AddColor( nv::vec4<float> );
	virtual float* GetColorv();

	virtual float GetWidth();
	virtual float GetHeight();
	virtual float GetDepth();

	virtual bool IsAnimated();
	virtual void SetAnimated( bool );

	unsigned int VertexOffset;

	float x, y;
	float s, t, s2, t2;

	static GLuint GUI_vbo;
protected:
	float Depth;
	float Width, Height;
	unsigned short Attributes;
	boost::function<int()> m_Callback; //the callback function //TODO: multipul callbacks?
	Window* Parent;
	bool isAnimated;
private:
	void GetControlData();

	bool hasFocus, isEnabled;
	nv::vec4<float> Color;

	std::string Type;
};

void Control_Init( const char* );
#endif
