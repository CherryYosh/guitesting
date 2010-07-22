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
#include <boost/bind.hpp>

#include "display.h"

#include "mouse.h"
#include "engine.h"
#include "renderer/ogl/oglBase.h"
#include "renderer/ogl/devilImage.h"

Display::Display() : System() {
    display = this;

    context = new oglBase();
    context->CreateContext();
    context->SetupContext();

    gui = new GUI();
    camera = new Camera();
    context->SetCamera(camera);

    camera->SetProjection(45.0, WINDOWWIDTH / WINDOWHEIGHT, 1.0, 1000.0);
    camera->SetOrtho(0, WINDOWWIDTH, WINDOWHEIGHT, 0, 0, 1000);
    camera->Move(0, 0, -1);

    Mouse_Init(this);
    InitTimers();
}

Display::~Display() {
    delete camera;
    delete gui;
    Mouse_Die();
    display = NULL;
}

void Display::InitTimers() {
    Timer* FPSTimer = new Timer(5000, true);
    FPSTimer->SetFunction(boost::bind<void>(&Display::DrawFPS, this, FPSTimer->GetTicksPtr()));
    FPSTimer->Start();

    //timers.AddTimer(FPSTimer, true, false);
    timers.AddTimer(Mouse_GetTimer());
}

void Display::DrawFPS(unsigned int* data) {
    printf("FPS: %f\n", (*data) * 0.2);
}

void Display::Start() {
    timers.Update();

    Render();
}

void Display::Render() {
    context->BeginRender();

    //here we draw the gui
    gui->Render();

    context->EndRender();
}

void Display::Resize(unsigned int width, unsigned int height) {
    context->SetViewport(0, 0, width, height);
    camera->SetProjection(camera->fov, width / height, camera->zNear, camera->zFar);
    camera->SetOrtho(0, width, height, 0, 0, 1000);
}

void Display::OnMouseClick(unsigned short* num, bool final) {
    if (!gui->OnMouseClick(*num, final)) {
	Mouse_StopTimer();
    }
}

void Display::OnMouseButtonChange() {
    Mouse_SetButtonState();

    gui->OnMousePress(Mouse_GetButtonState(), Mouse_GetX(), Mouse_GetY());
}

void Display::OnMouseMotion() {
    Mouse_SetPosition();

    if (!gui->OnMouseMotion((float) Mouse_GetX(), (float) Mouse_GetY(), Mouse_GetButtonState())) {
	if (Mouse_GetButtonState(0)) { //dragging
	    gui->Move(Mouse_GetChangeX(), Mouse_GetChangeY());
	}
    }
}

void Display::OnKeyPress(SDL_keysym sym) {
	gui->OnKeyPress(sym.unicode, sym.sym, sym.mod);
}

void Display::CreateWindow(std::string name) {
    gui->CreateWindow(name);
}
