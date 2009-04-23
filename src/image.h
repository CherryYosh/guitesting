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

bool Image_Init();

ILuint Image_LoadImage( const char*, bool = false );

ILuint Image_GetImageHeight();
ILuint Image_GetImageWidth();
ILubyte* Image_GetImageData();
ILuint Image_GetImageFormat();
ILuint Image_GetBPP();
ILuint Image_GetBitsPerPixel();

inline GLuint Image_GetGLTexture();
GLuint Image_GetGLTexture( const char* filename, unsigned int* width, unsigned int* height );

void Image_BindImage( ILuint id );
void Image_DeleteImage( ILuint id );

#endif
