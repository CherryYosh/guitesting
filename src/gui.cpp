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

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include "gui.h"

#include "input.h"
#include "engine.h"
#include "display.h"
#include "fontmgr.h"
#include "thememgr.h"
#include "gui/controls.h"

GUI::GUI() : System() {
	MouseOverWindow = NULL;
	ActiveWindow = NULL;
	IsRecevingInput = false;

	//set up the control
	Control_Init("themes/default.theme");
}

GUI::~GUI() {
	Windows.clear();
}

//TODO: there has to be a better way to do this rendering

void GUI::Render(Shader* shader) {

	//This must be dont before anything, as it possibly binds and unbinds a VBO
	size_t size = Windows.size();
	for (unsigned int i = 0; i < size; i++) {
		Windows[i]->StepAnimation();
	}

	shader->Bind();
	shader->SetProjection(display->GetCameraOrtho());

	//We bind the theme image
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ThemeMgr_GetImage());

	Control::GUI_vbo->Bind();
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableVertexAttribArray(shader->attribute[2]);

	glVertexAttribPointer(shader->attribute[0], 2, GL_FLOAT, GL_FALSE, 32, 0);
	glVertexAttribPointer(shader->attribute[1], 2, GL_FLOAT, GL_FALSE, 32, (GLvoid*) (2 * sizeof(float)));
	glVertexAttribPointer(shader->attribute[2], 4, GL_FLOAT, GL_FALSE, 32, (GLvoid*) (4 * sizeof(float)));

	for (unsigned int i = 0; i < size; i++) {
		Windows[i]->Render(shader);
	}

	glDisableVertexAttribArray(shader->attribute[0]);
	glDisableVertexAttribArray(shader->attribute[1]);
	glDisableVertexAttribArray(shader->attribute[2]);

	Control::GUI_vbo->Unbind();
	shader->Unbind();
}

void GUI::RenderText(Shader* shader) {
	shader->Bind();
	shader->SetProjection(display->GetCameraOrtho());

	glActiveTexture(GL_TEXTURE0);
	//TODO: fix this hard coding, so that only 1 font is loaded at a time
	glBindTexture(GL_TEXTURE_2D, FontMgr_GetImage(0));

	Control::GUI_vbo->Bind();
	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableVertexAttribArray(shader->attribute[2]);

	size_t size = Windows.size();
	for (unsigned int i = 0; i < size; i++) {
		Windows[i]->RenderText(shader->attribute[0], shader->attribute[1], shader->attribute[2]);
	}

	glDisableVertexAttribArray(shader->attribute[0]);
	glDisableVertexAttribArray(shader->attribute[1]);
	glDisableVertexAttribArray(shader->attribute[2]);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader->Unbind();
}

//TODO: Change this!! active window is not what it used to be!

bool GUI::HitTest(float x, float y) {
	//this is a quick excape..
	if (MouseOverWindow != NULL) {
		if (MouseOverWindow->HitTest(x, y, display->GetCameraOrtho())) {
			return true;
		} else {
			MouseOverWindow = NULL;
		}
	}

	size_t size = Windows.size();
	for (unsigned int i = 0; i < size; i++) {
		if (Windows[i]->HitTest(x, y, display->GetCameraOrtho())) {
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

//TODO: allow for used made themes

void GUI::CreateWindowConsole(float x, float y) {
	Window* window = new Window(this);

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

	//now we move it (and all its children) and make it build its vbo
	window->Width = topbar->GetWidth();
	window->Height = topbar->GetHeight() + lsidebar->GetHeight();
	window->Move(x, y);
	Windows.push_back(window);
}

//TODO: Delete this, temp function

void GUI::CreateTW() {
	Window* window = new Window(this);

	Button* b = new Button("sidebar", window);

	b->SetCallback(boost::bind<void>(&GUI::CreateTW, this));

	window->AddChild(b, true);
	window->Width = b->GetWidth();
	window->Height = b->GetHeight();

	Windows.push_back(window);
}

//TODO: possibly a better way to do this, would need to change it from using a vector

void GUI::CloseWindow(Window* w) {
	ActiveWindow = NULL;
	MouseOverWindow = NULL;

	std::vector<Window*>::iterator it = Windows.begin();
	while (it != Windows.end()) {
		if (*it == w) {
			Windows.erase(it);
			return;
		}
		it++;
	}
}
