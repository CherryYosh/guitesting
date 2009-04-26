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

/* 
 * File:   vbo.cpp
 * Author: brandon
 * 
 * Created on April 26, 2009, 8:00 PM
 */
#include <GL/glew.h>
#include "vbo.h"

#include <iostream>
#include <cstring>

vbo::vbo() {
    _isBound = false;
    glGenBuffers( 1, &id );
}

vbo::vbo(unsigned int length, void* data ) {
    _isBound = false;
    glGenBuffers( 1, &id );

    Bind();
	SetData( 0, length, data, VBO_CREATE );
    Unbind();
}

vbo::vbo(const vbo& orig){
}

vbo::~vbo(){
}

void vbo::Bind(){
    glBindBuffer( GL_ARRAY_BUFFER, id );
    _isBound = true;
}

void vbo::Unbind(){
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    _isBound = false;
}

bool vbo::SetData(unsigned int start, unsigned int length, void* data, unsigned int type ){
    if( !_isBound )
	    return false;

    if( type == VBO_REPLACE ){
	void* ptr = glMapBufferRange( GL_ARRAY_BUFFER, start, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT );

	if( ptr != NULL ){
		memcpy( ptr, data, length );

		glFlushMappedBufferRange( GL_ARRAY_BUFFER, 0,  length );
		glUnmapBuffer( GL_ARRAY_BUFFER );
	} else {
		printf( "Error: VBO %i could not update! %i\n", id, glGetError() );
		return false;
	}
    } else if( type == VBO_INSERT ){
	return false;
    } else if( type == VBO_CREATE ){
	glBufferData( GL_ARRAY_BUFFER, length, data, GL_STREAM_DRAW );
	return true;
    }

    return true;
}

GLuint vbo::GetID(){ return id; }
