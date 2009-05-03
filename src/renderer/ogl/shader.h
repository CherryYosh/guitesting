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
//a opengl shader wrapper..
#ifndef SHADER_H
#define SHADER_H

#include <GL/gl.h>

#include <string>

class Shader{
public:
	Shader();
	Shader(std::string);
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

#endif
