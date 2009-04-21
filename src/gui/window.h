#ifndef WINDOW_H
#define WINDOW_H

#include "../shader.h"
#include "../nvMatrix.h"
#include "../nvVector.h"

#include <vector>
#include <list>

#define BITSHIFT(x) 1 << x
#define BITSHIFT2(x,y)x << y

class Control;

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
	Control* Object;
	nv::vec4<float> data;
};

//enums used for window Depth
enum{ WINDOW_TOP, WINDOW_MIDDLE, WINDOW_BOTTOM };
//enums used for animation
enum{ 	TRANSLATEX = 	BITSHIFT(0), TRANSLATEY, TRANSLATEXY, TRANSLATEZ, TRANSLATEXYZ =BITSHIFT2(0x07,0), \
		ROTATEX = 		BITSHIFT(3), ROTATEY = 			BITSHIFT(4), ROTATEXY = 		BITSHIFT2(0x03, 3), ROTATEZ = 		BITSHIFT(5), ROTATEXYZ = 		BITSHIFT2(0x07,3), \
		ROTATEORGX = 	BITSHIFT(6), ROTATEORGY =  		BITSHIFT(7), ROTATEORGXY = 		BITSHIFT2(0x03, 6), ROTATEORGZ =  	BITSHIFT(8), ROTATEORGXYZ = 	BITSHIFT2(0x07,6), \
		ROTATESCREENX = BITSHIFT(9), ROTATESCREENY = 	BITSHIFT(10), ROTATESCREENXY = 	BITSHIFT2(0x03, 9), ROTATESCREENZ = BITSHIFT(11), ROTATESCREENXYZ = BITSHIFT2(0x07,9), \
		REDCHANNEL = 	BITSHIFT(12), GREENCHANNEL = 	BITSHIFT(13), RGCHANNEL = 		BITSHIFT2(0x03,12), BLUECHANNEL = 	BITSHIFT(14), RGBCHANNEL = 		BITSHIFT2(0x07,12), ALPHACHANNEL = BITSHIFT(15), RGBACHANNEL = BITSHIFT2(0x0F,12), \
		ORIGIN = 		BITSHIFT(16) };
//interpolation types
enum{ LINEAR = 0 };

class Window {
public:
	Window();
	~Window();

	virtual void AddChild( Control*, int, bool = true );
	virtual void Move( float, float );
	virtual int Close();
	virtual void UpdateVBO();
	virtual void OnKeyPress( unsigned short );
	virtual void OnMousePress( unsigned short );
	virtual bool HitTest( int, int );

	virtual void Render( Shader* );
	virtual void RenderAnimation( Shader* );
	virtual void RenderText( int, int, int );

	virtual void Animate( int, float, unsigned int, unsigned int, int, Control* = NULL );
	virtual void Animate( int, nv::vec2<float>, unsigned int, unsigned int, int, Control* = NULL );
	virtual void Animate( int, nv::vec3<float>, unsigned int, unsigned int, int, Control* = NULL );
	virtual void Animate( int, nv::vec4<float>, unsigned int, unsigned int, int, Control* = NULL );

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
