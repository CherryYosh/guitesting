#ifndef WINDOW_H
#define WINDOW_H

#include "control.h"

#include <vector>

//defines for the depth
#define WINDOW_BOTTOM 	2
#define WINDOW_MIDDLE 	1
#define WINDOW_TOP 	0

struct WINDOW_VBOVertex{
	float x, y;
	float s, t;
	float spacer[4]; //keep it aligned in 32 bits
};

class Window {
public:
	Window();
	~Window();

	virtual void AddChild( Control*, int, bool );
	virtual void Move( float, float );
	virtual void Close();
	virtual void Render();
	virtual void RenderText( int, int, int );
	virtual void UpdateVBO();
	virtual void OnKeyPress( unsigned short );
	virtual bool HitTest( int, int );

	float width, height;
	bool ReciveInput;
protected:
private:
	virtual void RebuildVBO();
	float Depth;
	float x, y;
	std::vector<Control*> Children;
	unsigned int VertexPosition, VertexLength; 
	bool VertexPositionIsSet;
	Control* ActiveChild;
};

#endif
