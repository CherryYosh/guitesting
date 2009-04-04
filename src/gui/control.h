#ifndef CONTROL_H
#define CONTROL_H

#define CTRL_INPUT (1) >> 1 //does the control allow input

#ifndef NULL
#define NULL 0
#endif //null

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <string>
#include <boost/bind.hpp>
#include <boost/function.hpp>

#include "../shader.h"
#include "../nvMatrix.h"
#include "../nvVector.h"

//the basic control class of all GUI objects
class Control{
public:
	Control( std::string t );
	virtual ~Control();
	virtual void Activate(); //called when ever the control becomes active
	virtual void Render();
	virtual bool HitTest( int mouseX, int mouseY );
	virtual void onMousePress( int button );
	virtual void onMouseRelease( int button );
	virtual void onKeyPress( int key, int mod );
	virtual void onKeyRelease( int key, int mod );
	virtual void Move( int x, int y );
	virtual void SetCallback( boost::function<int()> callback );
	virtual void SetWidth( unsigned int );
	virtual void SetHeight( unsigned int );
	virtual bool HasAttrib( unsigned short );

	boost::function<int()> m_Callback; //the callback function //TODO: multipul callbacks?
	std::string type; //CLOSE, etc
	unsigned int vertexOffset, textureOffset;
	static Shader* _Shader;
	nv::matrix4<float> modelview;
protected:
	virtual void SetEnabled( bool value );
	virtual void SetFocuse( bool value);

	int x, y, width, height;
	unsigned short attributes;
	float scale[2];
private:
	bool hasFocus, isEnabled;
};
#endif
