#ifndef GUI_H
#define GUI_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include <string>
#include <vector>

#include "system.h"
#include "shader.h"
#include "gui/controls.h"

class GUI : public System {
public:
	GUI( Engine *ptEngine );
	~GUI();

	void Render( Shader* );
	void RenderText( Shader* );

	void AddControl( Control* control );
	void SetTheme( char* );
	void HitTest( int, int );
	void Move( int, int );
	void HandelKeyPress( unsigned short ); 
	
	void CreateWindowConsole( float, float );

protected:
	std::vector<Window*> Windows;
	Window* ActiveWindow;
	unsigned int numIndices;
private:
	bool isRecevingInput;
};

#endif
