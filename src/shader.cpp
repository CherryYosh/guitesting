/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */
#include <GL/glew.h>

#include "shader.h"

#include <fstream>
#include <cstdlib>
#include <cstring>

#define SHADERPATH "./shaders/" //for ease

//reading and loading
char* ReadShaderFile( std::string filename );
GLuint glslLoadShaderProgram( std::string vertex, std::string fragment );

//debugging
void printShaderLog( GLuint shader );
void printProgramLog( GLuint program );
const char* getErrorString( GLenum error );


Shader::Shader(){
	name = "noname";

	//set the values, since we might wind up doing 1000's of these
	//I figure memset will be faster then a for loop
	memset( uniform, 0x00, 12 * sizeof( GLuint ));
	memset( attribute, 0x00, 12 * sizeof( GLuint ));
}

Shader::~Shader(){
}

bool Shader::Load( std::string shaderName ){
	name = shaderName;

	std::string s1, s2;
	s1 = SHADERPATH + shaderName + ".vert";
	s2 = SHADERPATH + shaderName + ".frag";

	id = glslLoadShaderProgram( s1.c_str(), s2.c_str() );
	return true;
}

void Shader::Bind(){
	glUseProgram( id );
}

void Shader::Unbind(){
	glUseProgram( 0 );
}

void Shader::GetUniformLoc( GLuint uID, std::string name ){
	uniform[uID] = glGetUniformLocation( id, name.c_str() );
}

void Shader::GetAttributeLoc( GLuint uID, std::string name ){
        attribute[uID] = glGetAttribLocation( id, name.c_str() );
}

void Shader::SetProjection( float m[16] ){
	glUniformMatrix4fv( uniform[0], 1, 0, m );
}

void Shader::SetModelview( float m[16] ){
	 glUniformMatrix4fv( uniform[1], 1, 0, m );
}


GLuint Shader::GetID(){
	return id;
}
//===========================================================================
//out of class functions

char* ReadShaderFile( std::string filename ){
        std::fstream file( filename.c_str(), std::fstream::in );
        std::size_t size; //size of the file
        char* file_data; //the shader data

        if( file == NULL ){
#ifdef _DEBUG_
                printf( "FATIAL: Could not read shader file %s\n", filename.c_str() );
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
GLuint glslLoadShaderProgram( std::string vertex, std::string fragment ){
	//a quick check
	if( vertex.empty() || fragment.empty() ){
		printf( "ERROR: Unable to load programs, program defined as NULL" );
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

const char* getErrorString( GLenum error ){
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
