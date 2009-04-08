//============================
// the display driver class
// handels all opengl and drawing code

#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "timer.h"
#include "mouse.h" 
#include "system.h"
#include "camera.h"
#include "gui.h"

class Display : public System {
public:
	Display( Engine *ptEngine );
	~Display();

	void Start();
	void ProcessMessages();

	void Render();
	void Resize( unsigned int width, unsigned int height );

	void OnMouseButtonChange();
	void OnMouseMotion();
protected:
private:
	Camera *camera;
	Timer *timer;
	GUI *gui;
};

#endif
