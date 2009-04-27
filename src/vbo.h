
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
 * File:   vbo.h
 * Author: brandon
 *
 * Created on April 26, 2009, 8:00 PM
 */

#ifndef VBO_H
#define	VBO_H

#include <GL/gl.h>
#include <GL/glext.h>

class VBO {
public:
	VBO( GLenum = GL_STREAM_DRAW );
	VBO( unsigned int, void*,GLenum = GL_STREAM_DRAW  );
	virtual ~VBO();

	void Bind();
	void Unbind();

	bool InitData( unsigned int, void* );
	bool SetData( unsigned int, unsigned int, void* );
	bool AddData( unsigned int, void*, unsigned int* = NULL );
	bool InsertData( unsigned int, unsigned int, unsigned int, void * );


	const GLuint GetID();
	const unsigned int GetSize();
private:
	GLuint id;
	GLenum type;
	bool isBound;
	unsigned int size;
};

#endif	/* _VBO_H */

