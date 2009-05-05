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
//============================
// the display driver class
// handels all opengl and drawing code

#ifndef DISPLAY_H
#define DISPLAY_H

#include "system.h"

#include "gui.h"
#include "timer.h"
#include "camera.h"
#include "timerpool.h"

class Display : public System {
public:
	Display();
	~Display();

	void Start();

	void DrawFPS(unsigned int*);

	void Render();
	void Resize( unsigned int, unsigned int );

	void OnMouseClick( unsigned short*, bool );
	void OnMouseButtonChange();
	void OnMouseMotion();
	void OnKeyPress( SDL_keysym );
protected:
private:
	void InitTimers();

	Camera* camera;
	timerpool timers;
	GUI *gui;
};

#endif
