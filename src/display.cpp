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
#include<boost/bind.hpp>

#include "renderer/ogl/oglBase.h"

#include "display.h"

#include "image.h"
#include "mouse.h"
#include "engine.h"
#include "fontmgr.h"

oglBase contex;

Display::Display() : System() {
	display = this;

	contex.CreateContex();
	contex.SetupContex();

	//start up the fontmgr, thanks rj
	FontMgr_Init();
	FontMgr_LoadFont(0, "/usr/share/fonts/corefonts/arial.ttf", 16);

	gui = new GUI();
	camera = new Camera();
	contex.SetCamera(camera);

	gui->CreateWindowConsole(50, 50);
	gui->CreateTW();

	camera->SetProjection(45.0, 640.0 / 480.0, 1.0, 1000.0);
	camera->SetOrtho(0, 640, 480, 0, 1, 20);
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

	timers.AddTimer(FPSTimer, true, false);
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
	contex.BeginRender();

	//here we draw the gui
	gui->Render();

	contex.EndRender();
}

void Display::Resize(unsigned int width, unsigned int height) {
	contex.SetViewport(0, 0, width, height);
	camera->SetProjection(camera->fov, width / height, camera->zNear, camera->zFar);
	camera->SetOrtho(0, width, height, 0, 1, 20);
}

void Display::OnMouseClick(unsigned short* num, bool final) {
	if (!gui->OnMouseClick(*num, final)) {
		Mouse_StopTimer();
	}
}

void Display::OnMouseButtonChange() {
	Mouse_SetButtonState();

	if (Mouse_GetButtonState(0))
		gui->OnMousePress(0, Mouse_GetX(), Mouse_GetY());
}

void Display::OnMouseMotion() {
	Mouse_SetPosition();

	if (Mouse_GetButtonState(0)) { //dragging
		gui->Move(Mouse_GetChangeX(), Mouse_GetChangeY());
	} else {
		gui->HitTest((float) Mouse_GetX(), (float) Mouse_GetY());
	}
}

void Display::OnKeyPress(SDL_keysym sym) {
	gui->OnKeyPress(sym.unicode);
}