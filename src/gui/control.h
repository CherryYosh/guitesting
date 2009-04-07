#ifndef CONTROL_H
#define CONTROL_H

#define CTRL_INPUT (1) >> 1 //does the control allow input

#ifndef NULL
#define NULL 0
#endif //null

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "../shader.h"

struct CTRL_GUIDataT{
	std::string type;
	float s,t, s2,t2; //the uv coords
	float width, height;
};

//the basic control class of all GUI objects
class Control{
public:
	Control( std::string t, float x, float y );
	virtual ~Control();
	virtual void Activate(); //called when ever the control becomes active
	virtual void Render();
	virtual bool HitTest( int mouseX, int mouseY );
	virtual void onMousePress( int button );
	virtual void onMouseRelease( int button );
	virtual void onKeyPress( unsigned short ); 
	virtual void onKeyRelease( int key, int mod );
	virtual void Move( float x, float y );
	virtual void SetCallback( boost::function<int()> callback );
	virtual void SetWidth( float );
	virtual void SetHeight( float );
	virtual bool HasAttrib( unsigned short );
	virtual void SetEnabled( bool value );
	virtual void SetFocus( bool value);
	virtual void SetDepth( float );

	virtual float GetWidth();
	virtual float GetHeight();
	virtual float GetDepth();

	//unsigned int TextureOffset;
	unsigned int VertexOffset; 

	float x, y;
	float s, t, s2, t2;

	static Shader* GUIShader;
	static Shader* TextShader;
	static GLuint GUI_vbo;
protected:
	float Depth;
	float Width, Height;
	unsigned short Attributes;
	std::string Type; //CLOSE, etc
	boost::function<int()> m_Callback; //the callback function //TODO: multipul callbacks?
private:
	void GetControlData();

	bool hasFocus, isEnabled;
};

void Control_Init( GLuint, const char* );
#endif
