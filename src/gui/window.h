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
#ifndef WINDOW_H
#define WINDOW_H

#include "control.h"

#include "../renderer/renderer.h"

#include "../nvMatrix.h"
#include "../nvVector.h"

#include <vector>
#include <list>

#define BITSHIFT(x) 1 << x
#define BITSHIFT2(x,y) x << y

struct AnimationType;
class Control;
class GUI;

//enums used for animation

enum {
	TRANSLATEX = BITSHIFT(0), TRANSLATEY, TRANSLATEXY, TRANSLATEZ, TRANSLATEXYZ = BITSHIFT2(0x07, 0),   \
	ROTATEX = BITSHIFT(3), ROTATEY = BITSHIFT(4), ROTATEXY = BITSHIFT2(0x03, 3), ROTATEZ = BITSHIFT(5), ROTATEXYZ = BITSHIFT2(0x07, 3),   \
	ROTATEORGX = BITSHIFT(6), ROTATEORGY = BITSHIFT(7), ROTATEORGXY = BITSHIFT2(0x03, 6), ROTATEORGZ = BITSHIFT(8), ROTATEORGXYZ = BITSHIFT2(0x07, 6),   \
	ROTATESCREENX = BITSHIFT(9), ROTATESCREENY = BITSHIFT(10), ROTATESCREENXY = BITSHIFT2(0x03, 9), ROTATESCREENZ = BITSHIFT(11), ROTATESCREENXYZ = BITSHIFT2(0x07, 9),   \
	REDCHANNEL = BITSHIFT(12), GREENCHANNEL = BITSHIFT(13), RGCHANNEL = BITSHIFT2(0x03, 12), BLUECHANNEL = BITSHIFT(14), RGBCHANNEL = BITSHIFT2(0x07, 12), ALPHACHANNEL = BITSHIFT(15), RGBACHANNEL = BITSHIFT2(0x0F, 12),   \
	ORIGIN = BITSHIFT(16)
};

//interpolation types

enum {
	LINEAR = 0
};

class Window : public Control {
public:
	Window(GUI*, Renderer*);
	~Window();

	void AddChild(Control*, bool = true);
	void Move(float, float);

	void Close();

	void UpdateControl(Control*);
	void OnKeyPress(unsigned short);
	void OnMousePress(unsigned short, int, int);
	bool OnMouseClick(unsigned short, bool);
	bool HitTest(float, float, float*);

	void Animate(int, float, unsigned int, unsigned int, int, Control* = NULL);
	void Animate(int, nv::vec2<float>, unsigned int, unsigned int, int, Control* = NULL);
	void Animate(int, nv::vec3<float>, unsigned int, unsigned int, int, Control* = NULL);
	void Animate(int, nv::vec4<float>, unsigned int, unsigned int, int, Control* = NULL);

	void StepAnimation();
	void RemoveAnimation(Control*);

	void Unproject(float, float, float*, float*, float*);

	bool IsRoot();
	unsigned int Size();
	unsigned int TotalChildren();
	unsigned int NumChildren();

	bool ReciveInput;
protected:
private:
	std::list<AnimationType> Animations;

	GUI* gui;
	Renderer* renderer;

	nv::matrix4<float> Modelview;
	nv::vec3<float> AnimationOrigin;
};

#endif
