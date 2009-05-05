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

oglWidgetRenderer* renderer;

GUI::GUI() : System() {
	MouseOverWindow = NULL;
	ActiveWindow = NULL;
	IsRecevingInput = false;
	renderer = new oglWidgetRenderer;

	//set up the control
	Control_Init("themes/default.theme");
}

GUI::~GUI() {
//	delete [] renderer;
	Windows.clear();
}

void GUI::Render() {
	size_t size = Windows.size();
	for (unsigned int i = 0; i < size; i++) {
		Windows[i]->StepAnimation();
	}

	renderer->Draw();
}

bool GUI::HitTest(float x, float y) {
	//this is a quick excape..
	if (MouseOverWindow != NULL) {
		if (MouseOverWindow->HitTest(x, y, renderer->GetCamera()->GetOrtho())) {
			return true;
		} else {
			MouseOverWindow = NULL;
		}
	}

	size_t size = Windows.size();
	for (unsigned int i = 0; i < size; i++) {
		if (Windows[i]->HitTest(x, y, renderer->GetCamera()->GetOrtho())) {
			MouseOverWindow = Windows[i];
			return true;
		}
	}

	return false;
}

void GUI::Move(int x, int y) {
	if (x == 0 && y == 0)
		return;

	if (ActiveWindow != NULL)
		ActiveWindow->Move(x, y);
}

/**
 * Makes the given window the active, also pops it to the top of the list
 * @param w the window to be made active
 * @returns TODO
 */
void GUI::MakeActive( Window* w ){
	ActiveWindow = w;

	if( w == NULL )
		return;

	for (std::vector<Window*>::iterator it = Windows.begin(); it != Windows.end(); it++) {
		if( *it == w ){
			Windows.erase( it );
			break;
		}
	}

	Windows.push_back( w );
}

void GUI::OnKeyPress(unsigned short unicode) {
	if (ActiveWindow != NULL)
		ActiveWindow->OnKeyPress(unicode);
}

void GUI::OnMousePress(unsigned short button, int mx, int my) {
	if (MouseOverWindow != NULL) {
		MakeActive( MouseOverWindow );
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

void GUI::CreateWindowConsole(float x, float y) {
	Window* window = new Window(this, renderer);

	Rule* topbar = new Rule("topbar", window);
	Rule* lsidebar = new Rule("sidebar", window);
	Rule* rsidebar = new Rule("sidebar", window);
	Rule* bottombar = new Rule("bottombar", window);
	Label* textarea = new Label("textarea", window);
	Editbox* inputarea = new Editbox("textinput", window);
	Button* close = new Button("close", window, topbar);

	close->SetCallback(boost::bind<void>(&Window::Close, window));

	//now to position them
	close->Move(topbar->GetWidth() - (close->GetWidth() * 1.5), topbar->GetHeight() * 0.25);
	lsidebar->Move(0, topbar->GetHeight());
	rsidebar->Move(topbar->GetWidth() - rsidebar->GetWidth(), topbar->GetHeight());
	bottombar->Move(lsidebar->GetWidth(), topbar->GetHeight() + lsidebar->GetHeight() - bottombar->GetHeight());
	textarea->Move(lsidebar->GetWidth(), topbar->GetHeight());
	inputarea->Move(lsidebar->GetWidth(), topbar->GetHeight() + textarea->GetHeight());

	//now add them all
	window->AddChild(topbar, false);
	window->AddChild(bottombar, false);
	window->AddChild(lsidebar, false);
	window->AddChild(rsidebar, false);
	window->AddChild(close, false);
	window->AddChild(textarea, false);
	window->AddChild(inputarea, true);

	renderer->AddObject(window);

	//now we move it (and all its children) and make it build its vbo
	window->Width = topbar->GetWidth();
	window->Height = topbar->GetHeight() + lsidebar->GetHeight();
	window->Move(x, y);
	Windows.push_back(window);
}

void GUI::CreateTW() {
	Window* window = new Window(this, renderer);

	Checkbox* b = new Checkbox("checkbox", window);

	window->AddChild(b, true);
	window->Width = b->GetWidth();
	window->Height = b->GetHeight();

	Windows.push_back(window);
	renderer->AddObject( window );
}

void GUI::CloseWindow(Window* w) {
}
