
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

enum{ VBO_CREATE, VBO_REPLACE, VBO_INSERT };

class vbo {
public:
	vbo();
	vbo( unsigned int, void* );
	vbo(const vbo& orig);
	virtual ~vbo();

	void Bind();
	void Unbind();

	bool SetData( unsigned int, unsigned int, void*, unsigned int = VBO_REPLACE );

	GLuint GetID();
private:
	GLuint id;
	bool _isBound;
};

#endif	/* _VBO_H */

