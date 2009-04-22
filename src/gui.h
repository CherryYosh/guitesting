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

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include <string>
#include <vector>

#include "system.h"
#include "shader.h"
#include "camera.h"
#include "gui/controls.h"

class GUI : public System {
public:
	GUI();
	~GUI();

	void Render( Shader* );
	void RenderAnimation( Shader* );
	void RenderText( Shader* );

	bool HitTest( float, float );
	void Move( int, int );
	void HandelKeyPress( unsigned short );
	void HandelMousePress( unsigned short );

	void CreateWindowConsole( float, float );

protected:
	std::vector<Window*> Windows;
	Window* ActiveWindow;
	unsigned int numIndices;
private:
	bool IsRecevingInput;
	Camera* Screen;
};

#endif
