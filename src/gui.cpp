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
#include "gui/controls.h"
#include "renderer/ogl/oglWidgetRenderer.h"
#include "theme.h"

oglWidgetRenderer* renderer;

GUI::GUI() : System() {
	MouseOverWindow = NULL;
	ActiveWindow = NULL;
	IsRecevingInput = false;
	renderer = new oglWidgetRenderer;
}

GUI::~GUI() {
	Windows.clear();
}

void GUI::Render() {
	renderer->Draw();
}

bool GUI::HitTest(float x, float y) {
	//this is a quick excape..
	if (MouseOverWindow != NULL) {
		if (MouseOverWindow->HitTest(x, y)) {
			return true;
		} else {
			MouseOverWindow = NULL;
		}
	}

	size_t size = Windows.size();
	for (unsigned int i = 0; i < size; i++) {
		if (Windows[i]->HitTest(x, y)) {
			MouseOverWindow = Windows[i];
			return true;
		}
	}

	return false;
}

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
 * @returns TODO
 */
void GUI::MakeActive(Window* w) {
	ActiveWindow = w;
	if (w == NULL)
		return;

	for (std::vector<Window*>::iterator it = Windows.begin(); it != Windows.end(); it++) {
		if (*it != w && (*it)->GetZ() >= w->GetZ()) {
			(*it)->AddDepth(-(TOP_LAYER + 1)); //TOP_LAYER is the total ammount of layers, prevents overlapping
		}
	}

	w->SetDepth(-TOP_LAYER);
	renderer->Refresh();
}

void GUI::OnKeyPress(unsigned short unicode) {
	if (ActiveWindow != NULL)
		ActiveWindow->OnKeyPress(unicode);
}

void GUI::OnMousePress(unsigned short button, int mx, int my) {
	if (MouseOverWindow != NULL) {
		MakeActive(MouseOverWindow);
		ActiveWindow->OnMousePress(button, mx, my);
	} else {
		ActiveWindow = NULL;
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

void GUI::CreateWindow( std::string name, float x, float y) {
	Theme t;
	Window* window = t.GetWindow(name);
	window->SetGUI(this);
	window->SetRenderer(renderer);
	window->Move(x,y);
	Windows.push_back(window);

	renderer->AddObject(window);
	renderer->Update(window, RENDERER_ADD);
}

void GUI::CloseWindow(Window* w) {
	std::vector<Window*>::iterator it = Windows.begin();
	while (*it != w)
		it++;
	Windows.erase(it);

	renderer->RemoveObject(w);
	renderer->Refresh();
}
