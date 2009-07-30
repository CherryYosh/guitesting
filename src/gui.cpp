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

#include "gui.h"
#include "input.h"
#include "theme.h"
#include "gui/controls.h"
#include "renderer/ogl/oglWidgetRenderer.h"
#include "renderer/ogl/oglFontRenderer.h"

oglWidgetRenderer* renderer;
oglFontRenderer* fontRenderer;

GUI::GUI() : MouseOverWindow(NULL), ActiveWindow(NULL), System() {
    renderer = new oglWidgetRenderer();
    fontRenderer = new oglFontRenderer();

    Timer* t = new Timer(0, true);
    t->SetFunction(boost::bind<void>(&GUI::UpdateWindowEvents, this, t->GetDeltaPtr()));
    t->Start();

    AddTimerToPool(t);
}

GUI::~GUI() {
    Windows.clear();
}

void GUI::Render() {
    timers.Update();

    fontRenderer->Refresh();
    fontRenderer->Draw();

    renderer->Draw();
}

bool GUI::HitTest(float x, float y) {
    //this is a quick excape..
    if (MouseOverWindow != NULL) {
	if (MouseOverWindow->MouseTest(x, y)) {
	    return true;
	} else {
	    if (MouseOverWindow->CanReleaseMouse()) {
		MouseOverWindow->ReleaseMouse();
		MouseOverWindow->OnMouseLeave();
	    } else {
		return false;
	    }
	}
    }

    size_t size = Windows.size();
    for (unsigned int i = 0; i < size; i++) {
	if (Windows[i]->MouseTest(x, y)) {
	    MouseOverWindow = Windows[i];
	    return true;
	}
    }

    MouseOverWindow = NULL;
    return false;
}

/**
 * Moves and updates the active window by x and y
 */
void GUI::Move(int x, int y) {
    if (x == 0 && y == 0)
	return;

    if (ActiveWindow != NULL) {
	ActiveWindow->Move(x, y);
	renderer->Update(ActiveWindow);
    }
}

/**
 * Makes the given window the active, also pops it to the top of the list
 * @param w the window to be made active
 */
void GUI::MakeActiveWindow(Window* w) {
    ActiveWindow = w;
    if (w == NULL)
	return;

    std::vector<Window*>::iterator it;
    for (it = Windows.begin(); it != Windows.end(); it++) {
	if (*it != w && (*it)->GetZ() >= w->GetZ()) {
	    (*it)->AddDepth(-(TOP_LAYER + 1)); //TOP_LAYER is the total ammount of layers, prevents overlapping
	}
    }

    w->SetDepth(-TOP_LAYER);
    renderer->Refresh();

    input->SetProfile("typing");
}

void GUI::OnKeyPress(unsigned short unicode, int key, int mod) {
    if (ActiveWindow != NULL)
	ActiveWindow->OnKeyPress(unicode, key, mod);
}

void GUI::OnMousePress(unsigned short button, int mx, int my) {
    if (MouseOverWindow != NULL) {
	MakeActiveWindow(MouseOverWindow);
	ActiveWindow->OnMousePress(button, mx, my);
    } else {
	MakeActiveWindow(NULL);
    }
}

bool GUI::OnMouseClick(unsigned short num, bool final) {
    if (ActiveWindow != NULL)
	return ActiveWindow->OnMouseClick(num, final);
    return false;
}

bool GUI::OnMouseMotion(float x, float y, unsigned short button) {
    HitTest(x, y);

    if (MouseOverWindow != NULL) {
	if (MouseOverWindow->OnMouseMotion(x, y, button)) {
	    renderer->Update(MouseOverWindow);
	    return true;
	}
	return false;
    }
    return false;
}

void GUI::UpdateWindowEvents(unsigned int* step) {
    size_t size = Windows.size();
    for (size_t i = 0; i < size; i++)
	Windows[i]->StepEvents(*step);
}

#ifdef CreateWindow
#undef CreateWindow
#endif

void GUI::CreateWindow( std::string name, float x, float y) {
    Window* window = Theme::Widget(name);

    if (window != NULL) {
	window->SetGUI(this);
	window->SetRenderer(renderer);
	window->Move(x, y);
	Windows.push_back(window);

	fontRenderer->AddObject(window);
	fontRenderer->Update(window, RENDERER_ADD);

	renderer->AddObject(window);
	renderer->Update(window, RENDERER_ADD);
    } else {
	printf("ERROR: Requested Widget type %s. Type not found!\n", name.c_str());
    }
}

void GUI::CloseWindow(Window* w) {
    MakeActiveWindow(NULL);
    MouseOverWindow = NULL;

    std::vector<Window*>::iterator it = Windows.begin();
    while ((*it) != w)
	it++;

    fontRenderer->RemoveObject(*it);
    renderer->RemoveObject(*it);
    Windows.erase(it);

    fontRenderer->Refresh();
    renderer->Refresh();

    delete w;
}

void GUI::AddTimerToPool(Timer* t, bool tick, bool step) {
    timers.AddTimer(t, tick, step);
}
