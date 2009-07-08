/* 
 * File:   image.h
 * Author: brandon
 *
 * Created on May 21, 2009, 4:29 PM
 */

#ifndef _IMAGE_H
#define	_IMAGE_H

#include <string>

class Image {
public:

	virtual bool Load(std::string) = 0;

	virtual unsigned int GetID() = 0;

	virtual unsigned int Width() = 0;

	virtual unsigned int Height() = 0;

	virtual unsigned int Format() = 0;

	virtual unsigned int BPP() = 0;
};

#endif	/* _IMAGE_H */

