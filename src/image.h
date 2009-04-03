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
