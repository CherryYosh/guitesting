//a opengl shader wrapper..

#ifndef SHADER_H
#define SHADER_H

//#define GL_GLEXT_PROTOTYPES
//#include <GL/gl.h>
//#include <GL/glext.h>

#include <string>

#define SHADERPATH "./shaders/" //for ease

class Shader{
public:
	Shader();
	~Shader();
	
	bool Load( std::string shaderName );
	void Bind();
	void Unbind();
	void GetUniformLoc( GLuint uID, std::string name );
	void GetAttributeLoc( GLuint uID, std::string name );
	void SetProjection( float m[16] );
	void SetModelview( float m[16] );
	GLuint GetID();
	
	GLuint uniform[12];
	GLuint attribute[12];
private:
	std::string name;
	GLuint id;
};

//reading and loading
char* ReadShaderFile( const char* filename );
GLuint glslLoadShaderProgram( const char* vertex, const char* fragment );

//debugging
void printShaderLog( GLuint shader );
void printProgramLog( GLuint program );
char* getErrorString( GLenum error );

#endif
