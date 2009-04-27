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
#ifndef GUI_H
#define GUI_H

#include <vector>

#include "system.h"

#include "shader.h"
#include "gui/window.h"

class GUI : public System {
public:
	GUI();
	~GUI();

	void Render( Shader* );
	void RenderText( Shader* );

	bool HitTest( float, float );
	void Move( int, int );

	void OnKeyPress( unsigned short );
	void OnMousePress( unsigned short, int, int );

	void CreateWindowConsole( float, float );

protected:
	std::vector<Window*> Windows;
	Window* MouseOverWindow;
	Window* ActiveWindow;
private:
	bool IsRecevingInput;
};

#endif
