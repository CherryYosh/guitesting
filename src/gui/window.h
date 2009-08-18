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

#include "widget.h"

#include "../renderer/renderer.h"
#include "../events/event.h"

#include "../utils/matrix.h"
#include "../utils/vector.h"

#include <vector>
#include <list>

class Widget;
class GUI;

class Window : public Widget {
public:
    Window();
    Window(GUI*, Renderer*);
    ~Window();

    Widget* clone();

    void Move(float, float);

    void Close();

    void UpdateWidget(Widget*);

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

    void ActivateEvent(Event*);
    void DeactivateEvent(Event*);
    void StepEvents(unsigned int);

    void Rotate(float, float, float, float);
    void ReloadTheme();

    void SetBorder(int, int, int, int);

    float InternalX();
    float InternalY();
    float InternalWidth();
    float InternalHeight();

    void AddChild(Widget*);

    void SetInternalSize(int, int);

    bool GetChildAttributes(long);

    bool borderFinished, resizable;
protected:
private:
    GUI* gui;
    Renderer* renderer;

    std::vector<Event*> activeEvents;
    util::matrix4<float> rotation;

    int top, bottom, left, right;
    Widget* border;
};

#endif
