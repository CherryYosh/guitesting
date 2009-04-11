#ifndef SYSTEM_H
#define SYSTEM_H

#ifdef _DEBUG_
#include <stdio.h>
#endif

#include "defines.h"

class Input;
class Display;

class System{
public:
	System(); 
	virtual ~System();

	virtual void Start();
	
	static Input* input;
	static Display* display;
protected:
private:
};

#endif
