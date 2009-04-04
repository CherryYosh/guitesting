#include <GL/glew.h>

#include "shader.h"


#include <fstream>
#include <stdlib.h>


Shader::Shader(){
	name = "noname";

	for( int i = 0; i < 12; i++ )
		uniform[i] = 0;
}

Shader::~Shader(){
//	delete [] name;
}

//GLuint glslLoadShaderProgram( const char* vertex, const char* fragment );

bool Shader::Load( std::string shaderName ){
	name = shaderName;
	
	std::string s1, s2;
	s1 = SHADERPATH + shaderName + ".vert";
	s2 = SHADERPATH + shaderName + ".frag";

	id = glslLoadShaderProgram( s1.c_str(), s2.c_str() );
}

void Shader::Bind(){
	glUseProgram( id );
}

void Shader::Unbind(){
	glUseProgram( 0 );
}

void Shader::GetUniformLoc( GLuint uID, char* name ){
	uniform[uID] = glGetUniformLocation( id, name );
}

void Shader::GetAttributeLoc( GLuint uID, char* name ){
        attribute[uID] = glGetAttribLocation( id, name );
}

void Shader::SetProjection( float m[16] ){
	glUniformMatrix4fv( uniform[0], 1, false, m );
}

void Shader::SetModelview( float m[16] ){
	 glUniformMatrix4fv( uniform[1], 1, false, m );
}


GLuint Shader::GetID(){
	return id;
}
//===========================================================================
//out of class functions

char* ReadShaderFile( const char* filename ){
        std::fstream file( filename, std::fstream::in );
        std::size_t size; //size of the file
        char* file_data; //the shader data

        if( file == NULL ){
#ifdef _DEBUG_
                printf( "FATIAL: Could not read shader file %s\n", filename );
#endif
                return NULL;
        }

        //get file size
        file.seekg(0, std::ios::end);
        size = file.tellg();
        file.seekg(0, std::ios::beg);

        file_data = new char[size];

        file.read( file_data, size );
        file.close();

        //fix a bug and return
        file_data[size-1] = '\0';
        return file_data;
}

//loades the shaders into a program, retruning the programs id
GLuint glslLoadShaderProgram( const char* vertex, const char* fragment ){
	//a quick check
	if( vertex == NULL  || fragment == NULL ){
		printf( "ERROR: Unable to load programs, program defined as NULL" );
		//exit(-1);
		return 0;
	}

	//temp ids
	GLuint vsID, fsID, program;

	//make the vertex shader
	const char* vertex_data = ReadShaderFile( vertex );
	vsID = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( vsID, 1, &vertex_data, NULL );
	glCompileShader( vsID );
	printShaderLog( vsID );

	delete [] vertex_data;

	//make the fragment shader
	const char * fragment_data = ReadShaderFile( fragment );
	fsID = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( fsID, 1, &fragment_data, NULL );
	glCompileShader( fsID );
	printShaderLog( fsID );
	
	delete [] fragment_data;

	//make and link the program
	program = glCreateProgram();
	glAttachShader( program, vsID );
	glAttachShader( program, fsID );

	glBindFragDataLocationEXT( program, 0, "pixelColor" );

	glLinkProgram( program );
	printProgramLog( program );

	//and return, best of luck
	return program;
}
/*
//loads and compiles a shader, returning it's id
//type is eithe GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
GLuint glslLoadShader( char* shader, GLenum type ){
	GLuint id;
	const char * data = ReadShaderFile( shader );
	id = glCreateShader( type );
	glShaderSource( id, 1, &data, NULL );
	glCompileShader( id );
	printShaderLog( id );

	//remeber stop the zombie infestation at its source
	delete [] data;
	return id;
}

//same as glAttachShader, here just for formality
void glslAttachShader( GLuint program, GLuint shader ){ 
	glAttachShader( program, shader );
}

//same as glLinkProgram, here just for formality
void glslLinkProgram( GLuint program ){
	glLinkProgram( program );
	printProgramLog( program );
}
*/
/***********************************************
 * 		debugging shaders              *
 **********************************************/

void printShaderLog( GLuint shader ){
        int length;
        char *log;

	glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &length );

        if( length <= 0 )
                return;

        log = new char[length];
        if( log == NULL )
                return;

        glGetShaderInfoLog( shader, length, NULL, log );
        printf( "%s\n", log );

        delete [] log;
}

void printProgramLog( GLuint program ){
	int infologLength = 0;
    	int charsWritten  = 0;
    	char *infoLog;
    	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
   	if (infologLength > 1) {
        	infoLog = (char *)malloc(infologLength);
        	glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
        	printf("%s\n", infoLog);
        	free(infoLog);
    	}
}

char* getErrorString( GLenum error ){
	if ( error == GL_INVALID_ENUM ){ 
		return "Invalid Enum."; 
	}else if ( error == GL_INVALID_VALUE ){ 
		return "Invalid Value."; 
	}else if ( error == GL_INVALID_OPERATION ){ 
		return "Invalid Operation."; 
	}else if ( error == GL_STACK_OVERFLOW ){ 
		return "Stack Overflow."; 
	}else if ( error == GL_STACK_UNDERFLOW ){ 
		return "Stack Underflow."; 
	}else if ( error == GL_OUT_OF_MEMORY ){ 
		return "Out of memory."; 
	} else { 
		return "Undefined."; 
	}
}
