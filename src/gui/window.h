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
    Window();
    Window(GUI*, Renderer*);
    Window(const Window&);
    ~Window();

    Control* clone();

    void AddChild(Control*);
    void Move(float, float);

    void Close();

    void UpdateControl(Control*);

    void OnKeyPress(unsigned short, int, int);
    void OnMousePress(unsigned short, int, int);
    bool OnMouseClick(unsigned short, bool);
    bool MouseTest(float, float);

    void Unproject(float, float, float*, float*);

    unsigned int Size();

    util::matrix4<float>* GetRotation();
    float* GetRotationfv();

    void SetGUI(GUI*);
    GUI* GetGUI();

    void SetRenderer(Renderer*);
    Renderer* GetRenderer();

    void AddEvent(Event*);
    void RemoveEvent(Event*);
    void StepEvents(unsigned int);

    void Rotate(float, float, float, float);
    void ReloadTheme();

    void SetBorders(int, int, int, int);
    void UpdateBorders();

    void Resizable(bool);
    bool Resizable();
    void Resize(int, int);

    //TODO: find better names
    float GetInternalX();
    float GetInternalY();
    float GetInternalWidth();
    float GetInternalHeight();

    bool GetChildAttributes(long);

    void CloseButton(int, int, util::Color = "#FF000000");

protected:
private:
    GUI* gui;
    Renderer* renderer;

    std::vector<Event*> activeEvents;
    util::matrix4<float> rotation;

    bool bordersSet;
    bool resizable;
    Control *topborder, *bottomborder, *leftborder, *rightborder, *leftedge, *rightedge;
};

#endif
