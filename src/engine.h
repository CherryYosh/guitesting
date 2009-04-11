#ifndef ENGINE_H
#define ENGINE_H

#include "defines.h"

class Display;
class Input;

class Engine{
public:
	Engine();
	~Engine();

	void Start();
	void Run();
	void Quit();

	static Engine* engine;
protected:
private:
	bool running;
	
	Input *input;
	Display *display;
};


#endif
