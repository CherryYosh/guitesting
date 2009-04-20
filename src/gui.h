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

	bool HitTest( int, int );
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
