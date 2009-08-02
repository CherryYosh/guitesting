/* 
 * File:   devilImage.cpp
 * Author: brandon
 * 
 * Created on May 21, 2009, 8:26 PM
 */

#include <GL/gl.h>

#include "devilImage.h"

#include <IL/ilu.h>
#include <IL/ilut.h>

bool devilImage::InitDevIL() {
    if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION ||
	    iluGetInteger(ILU_VERSION_NUM) < ILU_VERSION ||
	    ilutGetInteger(ILUT_VERSION_NUM) < ILUT_VERSION) {
	printf("DevIL version is diffrent.. Please Update your DevIL.\n");
	return false;
    }

    //now we init, dont think its needed in linux, but oh well :P
    ilInit();
    iluInit();
    ilutRenderer(ILUT_OPENGL);

    ILenum error = ilGetError();
    if (error != IL_NO_ERROR) {
	printf("ERROR: DevIL error (%d) %s\n", error, iluErrorString(error));
	return false;
    }

    //woo that was tought
    return true;
}

devilImage::devilImage() : id(0), width(0), height(0), format(0), bpp(0) { }

devilImage::devilImage(const char* filename) {
    Load(filename);
}

devilImage::devilImage(std::string filename) {
    Load(filename);
}

devilImage::devilImage(int internalformat, int width, int height, int format, int datatype, char* data, bool mipmaping) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    if (mipmaping) {
	gluBuild2DMipmaps(GL_TEXTURE_2D, internalformat, width, height, format, datatype, data);
    } else {
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, datatype, data);
    }

    this->width = width;
    this->height = height;
    this->format = format;
    this->bpp = 0;
}

devilImage::devilImage(const devilImage& orig) : id(orig.id), width(orig.width), height(orig.height), format(orig.format), bpp(orig.bpp) { }

devilImage::~devilImage() { }

bool devilImage::Load(std::string filename) {
    ILuint tid;
    ilGenImages(1, &tid);
    ilBindImage(tid);

    if (ilLoadImage(filename.c_str()) == false) {
	ILenum error = ilGetError();
	printf("ERROR:: DevIL error (%d, %s) %s\n", error, filename.c_str(), iluErrorString(error));

	id = 0;
	width = 0;
	height = 0;
	format = 0;
	bpp = 0;

	ilBindImage(0);
	ilDeleteImages(1, &tid);
	return false;
    }

    id = ilutGLBindTexImage();
    width = ilGetInteger(IL_IMAGE_WIDTH);
    height = ilGetInteger(IL_IMAGE_HEIGHT);
    format = ilGetInteger(IL_IMAGE_FORMAT);
    bpp = ilGetInteger(IL_IMAGE_BPP);

    ilBindImage(0);
    ilDeleteImages(1, &tid);
    return true;
}

unsigned int devilImage::GetID() {
    return id;
}

unsigned int devilImage::Width() {
    return width;
}

unsigned int devilImage::Height() {
    return height;
}

unsigned int devilImage::Format() {
    return format;
}

/**
 * Returns the bytes per pixel
 */
unsigned int devilImage::BPP() {
    return bpp;
}