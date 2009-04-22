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
#include "image.h"

//a quick check to make sure we're using the same verson
//if this returns false they need to update devIL and recompile this
bool InitImage(){
	//taken from the site
	if( ilGetInteger( IL_VERSION_NUM ) < IL_VERSION ||
		iluGetInteger( ILU_VERSION_NUM ) < ILU_VERSION ||
			ilutGetInteger( ILUT_VERSION_NUM ) < ILUT_VERSION ){
				printf( "DevIL version is diffrent.. Please Update your DevIL.\n" );
				return false;
	}

	//now we init, dont think its needed in linux, but oh well :P
	ilInit();
	iluInit();
	ilutRenderer( ILUT_OPENGL );

	ILenum error = ilGetError();
	if( error != IL_NO_ERROR ){
		printf("ERROR: DevIL error (%d) %s\n", error, iluErrorString(error));
		return false;
	}

	//woo that was tought
	return true;
}

//loads a image returning the IL id for it
ILuint LoadNewImage( const char* filename, bool unbind ){
	ILuint id;
	ilGenImages( 1, &id );
	ilBindImage( id );

	if( ilLoadImage( filename ) == false ){
		ILenum error = ilGetError();
		printf("ERROR:: DevIL error (%d) %s\n", error, iluErrorString(error));
		
		//BRRRAAAAAIIIINNNS
		ilDeleteImages( 1,  &id );
		return 0;
	}

	if( unbind )
		ilBindImage( 0 );
	return id;
}

//use this with caustion
bool LoadImage( char* filename ){
	if( ilLoadImage( filename ) == false ){
		ILenum error = ilGetError();
		printf("ERROR:: DevIL error (%d) %s\n", error, iluErrorString(error));
		return false;
	}
	return true;
}

ILuint GetImageHeight(){
	return ilGetInteger( IL_IMAGE_HEIGHT );
}

ILuint GetImageWidth(){
	return ilGetInteger( IL_IMAGE_WIDTH );
}

ILuint GetImageFormat(){
	return ilGetInteger( IL_IMAGE_FORMAT );
}

ILuint GetBPP(){
	return ilGetInteger( IL_IMAGE_BPP );
}

ILuint GetBitsPerPixel(){
	return ilGetInteger( IL_IMAGE_BITS_PER_PIXEL );
}

ILubyte* GetImageData(){
	return ilGetData();
}

GLuint GetGLTexture(){
	return ilutGLBindTexImage();
}

GLuint GetGLTexture( const char* filename, unsigned int* width, unsigned int* height ){
	ILuint temp;
	if( (temp = LoadNewImage( filename, false )) == 0 )
		return 0; //nothing else to do

	GLuint id = ilutGLBindTexImage();
	
	if(width != NULL )
		*width = GetImageWidth();
	if(height != NULL )
		*height = GetImageHeight();

	ilBindImage( 0 );
	ilDeleteImages( 1, &temp );

	return id;
}

void BindImage( ILuint id ){
	ilBindImage( 0 );
}

void DeleteImage( ILuint id ){
	ilDeleteImages( 1, &id );
}
