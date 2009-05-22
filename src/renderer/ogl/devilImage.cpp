/* 
 * File:   devilImage.cpp
 * Author: brandon
 * 
 * Created on May 21, 2009, 8:26 PM
 */

#include "devilImage.h"

#include <IL/ilu.h>
#include <IL/ilut.h>

bool devilImage::InitDevIL(){
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

devilImage::devilImage() : _id(0), _width(0), _height(0), _format(0), _bpp(0) { }

devilImage::devilImage(std::string filename){ Load(filename); }

devilImage::devilImage(const devilImage& orig) { }

devilImage::~devilImage() { }

void devilImage::Load(std::string filename) {
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);

	if (ilLoadImage(filename.c_str()) == false) {
		ILenum error = ilGetError();
		printf("ERROR:: DevIL error (%d) %s\n", error, iluErrorString(error));

		_id = 0;
		_width = 0;
		_height = 0;
		_format = 0;
		_bpp = 0;

		ilBindImage(0);
		ilDeleteImages(1, &id);
		return;
	}

	_id = ilutGLBindTexImage();
	_width = ilGetInteger(IL_IMAGE_WIDTH);
	_height = ilGetInteger(IL_IMAGE_HEIGHT);
	_format = ilGetInteger(IL_IMAGE_FORMAT);
	_bpp = ilGetInteger(IL_IMAGE_BPP);

	ilBindImage(0);
	ilDeleteImages(1, &id);
}

unsigned int devilImage::GetID(){ return _id; }
unsigned int devilImage::Width(){ return _width; }
unsigned int devilImage::Height(){ return _height; }
unsigned int devilImage::Format(){ return _format; }
unsigned int devilImage::BPP(){ return _bpp; }