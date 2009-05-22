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

	virtual void Load(std::string) { }

	virtual unsigned int GetID() {
		return 0;
	}

	virtual unsigned int Width() {
		return 0;
	}

	virtual unsigned int Height() {
		return 0;
	}

	virtual unsigned int Format() {
		return 0;
	}

	virtual unsigned int BPP() {
		return 0;
	}
};

#endif	/* _IMAGE_H */

