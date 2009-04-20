#ifndef WINDOW_H
#define WINDOW_H

#include "control.h"

#include "../shader.h"
#include "../nvMatrix.h"
#include "../nvVector.h"

#include <vector>
#include <list>

struct WINDOW_VBOVertex{
	float x, y;
	float s, t;
	float spacer[4]; //keep it aligned in 32 bits
};

struct AnimationType{
	unsigned int Type;
	unsigned int Interpolation;
	unsigned int EndTicks;
	unsigned int StartTicks;
	unsigned int LastTicks;
	unsigned int Duration; //end - start
	unsigned short id;
	nv::vec4<float> data;
};

//enums used for window Depth
enum{ WINDOW_TOP, WINDOW_MIDDLE, WINDOW_BOTTOM };
//enums used for animation
enum{ 	TRANSLATEX = 1, TRANSLATEY, TRANSLATEXY, TRANSLATEZ, TRANSLATEXYZ = 7, \
		ROTATEX = 8, ROTATEY = 16, ROTATEXY = 24, ROTATEZ = 32, ROTATEXYZ = 56, \
		REDCHANNEL = 64, GREENCHANNEL = 128, RGCHANNEL = 192, BLUECHANNEL = 256, RGBCHANNEL = 448, ALPHACHANNEL = 512, RGBACHANNEL = 960, \
		ORIGIN = 1024 };
//interpolation types
enum{ LINEAR = 0 };

class Window {
public:
	Window();
	~Window();

	virtual void AddChild( Control*, int, bool );
	virtual void Move( float, float );
	virtual void Close();
	virtual void UpdateVBO();
	virtual void OnKeyPress( unsigned short );
	virtual bool HitTest( int, int );

	virtual void Render( Shader* );
	virtual void RenderAnimation( Shader* );
	virtual void RenderText( int, int, int );

	virtual void Animate( int, float, unsigned int, unsigned int, int );
	virtual void Animate( int, nv::vec2<float>, unsigned int, unsigned int, int );
	virtual void Animate( int, nv::vec3<float>, unsigned int, unsigned int, int );
	virtual void Animate( int, nv::vec4<float>, unsigned int, unsigned int, int );

	virtual void StepAnimation();

	float Width, Height;
	bool ReciveInput;
protected:
private:
	virtual void RebuildVBO();
	float Depth;
	float x, y;

	std::vector<Control*> Children;
	std::list<AnimationType> Animations;

	unsigned int VertexPosition, VertexLength;
	bool VertexPositionIsSet;
	Control* ActiveChild;

	nv::matrix4<float> Modelview;
	nv::vec3<float> AnimationOrigin;
};

#endif
