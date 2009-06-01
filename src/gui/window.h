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
#include "../events/event.h"

#include "../utils/matrix.h"
#include "../utils/vector.h"

#include <vector>
#include <list>

class Control;
class GUI;

class Window : public Control {
public:
	Window(GUI*, Renderer*);
	~Window();

	void AddChild(Control*);
	void Move(float, float);

	void Close();

	void UpdateControl(Control*);
	void OnKeyPress(unsigned short);
	void OnMousePress(unsigned short, int, int);
	bool OnMouseClick(unsigned short, bool);
	bool HitTest(float, float);

	void Unproject(float, float, float*, float*);

	bool IsRoot();
	unsigned int Size();

	util::matrix4<float>* GetRotation();
	float* GetRotationfv();

	bool NeedsUpdate();
	void NeedsUpdate(bool);

	void SetGUI(GUI*);
	GUI* GetGUI();

	Window* GetRoot();

	void SetRenderer(Renderer*);
	Renderer* GetRenderer();

	void AddEvent(Event*);
	void RemoveEvent(Event*);
	void StepEvents(unsigned int);

	void Rotate(float,float,float,float);
protected:
private:
	GUI* gui;
	Renderer* renderer;

	std::vector<Event*> activeEvents;
	util::matrix4<float> rotation;

	bool awatingUpdate;
};

#endif
