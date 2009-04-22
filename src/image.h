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
#ifndef IMAGE_H
#define IMAGE_H

#include <GL/gl.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <stdio.h>

bool InitImage();

ILuint LoadNewImage( const char* filename, bool unbind );
bool LoadImage( char* filename );

ILuint GetImageHeight();
ILuint GetImageWidth();
ILubyte* GetImageData();
ILuint GetImageFormat();
ILuint GetBPP();
ILuint GetBitsPerPixel();

inline GLuint GetGLTexture();
GLuint GetGLTexture( const char* filename, unsigned int* width, unsigned int* height );

void BindImage( ILuint id );
void DeleteImage( ILuint id );
#endif
